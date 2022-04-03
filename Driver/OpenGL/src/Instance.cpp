#include <GL/glew.h>
#include <GL/wglew.h>

#include <Instance.hpp>

#include <GL/PhysicalDevice.hpp>


#include <windows.h>
#include <winuser.h>
#include <stdexcept>
#include <string>

#include <condition_variable>
#include <queue>
#include <set>

namespace OCRA::Instance
{
class WorkerThread
{
public:
    struct Command {
        Command(const std::function<void()>& a_FN, const uint64_t& a_ID, const bool a_Notify)
            : fn(a_FN)
            , id(a_ID)
            , notify(a_Notify)
        {}
        void operator()() { return fn(); }
        const std::function<void()> fn;
        const uint64_t id;
        const bool notify;
    };
    WorkerThread() {
        _thread = std::thread([this] {
            std::unique_lock<std::mutex> lock(_mtx);
            while (!_stop) {
                _cv.wait(lock, [this] {
                    return !_queue.empty() || _stop;
                });
                if (_queue.empty()) continue;
                auto command = std::move(_queue.front());
                _queue.pop();
                lock.unlock();
                command();
                if (command.notify) {
                    _finishedTasks.insert(command.id);
                    _taskCv.notify_all();
                }
                lock.lock();
            }
        });
    }
    void PushCommand(const std::function<void()>& a_Command, const bool a_Synchronous)
    {
        std::unique_lock<std::mutex> lock(_mtx);
        const auto commandID = _commandID++;
        _queue.push({ a_Command, commandID, a_Synchronous });
        _cv.notify_one();
        if (a_Synchronous) {
            _taskCv.wait(lock, [this, commandID] {
                const auto done = _finishedTasks.find(commandID) != _finishedTasks.end();
                _finishedTasks.erase(commandID);
                return done;
            });
        }
    }
    void Stop() {
        _stop = true;
        _cv.notify_one();
        _thread.join();
    }
private:
    std::mutex _mtx;
    std::condition_variable _cv;
    std::queue<Command> _queue;

    std::condition_variable _taskCv;
    std::set<uint64_t> _finishedTasks;

    std::thread _thread;
    bool _stop{ false };
    uint64_t _commandID{ 0 };
};
struct Impl
{
	Impl(const Info& a_Info)
		: info(a_Info)
	{
#ifdef _WIN32
        WNDCLASSA wndclass{};
        wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
        wndclass.lpfnWndProc = DefWindowProcA;
        wndclass.hInstance = GetModuleHandle(0);
        wndclass.lpszClassName = "Dummy_OpenGL_Window";

        //register window class
        if (!RegisterClassA(&wndclass)) throw std::runtime_error("Could not register window class");

        hwnd = CreateWindowExA(
            0,
            wndclass.lpszClassName,
            "Dummy OpenGL Window",
            0,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            0,
            0,
            wndclass.hInstance,
            0);

        //device context handle
        hdc = GetDC(hwnd);

        PIXELFORMATDESCRIPTOR pfd{};
        pfd.nSize = sizeof(pfd);
        pfd.nVersion = 1;
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        pfd.cColorBits = 32;
        pfd.cAlphaBits = 8;
        pfd.iLayerType = PFD_MAIN_PLANE;
        pfd.cDepthBits = 24;
        pfd.cStencilBits = 8;
        int pixel_format = ChoosePixelFormat(hdc, &pfd);
        if (!pixel_format) throw std::runtime_error("Failed to find a suitable pixel format.");
        if (!SetPixelFormat(hdc, pixel_format, &pfd)) throw std::runtime_error("Failed to set the pixel format.");
        const auto hglrcTemp = wglCreateContext(hdc);
        if (hglrcTemp == nullptr) throw std::runtime_error("Failed to create a dummy OpenGL rendering context.");
        if (!wglMakeCurrent(hdc, hglrcTemp)) throw std::runtime_error("Failed to activate dummy OpenGL rendering context.");

        //LET'S GET STARTED !
        if (wglewInit() != GLEW_OK) throw std::runtime_error("Cound not initialize WGLEW");
        if (!WGLEW_ARB_create_context) throw std::runtime_error("Modern context creation not supported !");
        if (!WGLEW_ARB_create_context_robustness) throw std::runtime_error("Robust context creation not supported !");
        int attribs[] =
        {
            WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
            WGL_CONTEXT_MINOR_VERSION_ARB, 3,
            WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_ROBUST_ACCESS_BIT_ARB,
            0
        };
        hglrc = wglCreateContextAttribsARB(hdc, 0, attribs); //commands execution context
        if (hglrc == nullptr) throw std::runtime_error("Failed to create a modern OpenGL rendering context.");
        wglMakeCurrent(nullptr, nullptr);
        wglDeleteContext(hglrcTemp);
#endif
        glThread.PushCommand([this] {
            wglMakeCurrent(hdc, hglrc);
            glewExperimental = TRUE;
            if (glewInit() != GLEW_OK) throw std::runtime_error("Cound not initialize GLEW");
        }, true);
    }
    ~Impl()
    {
#ifdef _WIN32
        glThread.PushCommand([this] {
            wglMakeCurrent(nullptr, nullptr);
            wglDeleteContext(hglrc);
        }, false);
        glThread.Stop();
        ReleaseDC(hwnd, hdc);
        DestroyWindow(hwnd);
        UnregisterClassA("Dummy_OpenGL_Window", GetModuleHandle(0));
#endif //_WIN32
    }
#ifdef _WIN32
    HDC         hdc;
    HWND        hwnd;
    HGLRC       hglrc;
    WorkerThread glThread;
#endif //_WIN32
	const Info info;
	std::vector<PhysicalDevice::Handle> physicalDevices;
	const std::string type{ "OpenGL" };
};
Handle Create(
	const Info& a_Info,
	const AllocationCallback* a_Allocator)
{
    auto instance = Handle(new Impl(a_Info));
    instance->physicalDevices = { PhysicalDevice::Create(instance) };
	return instance;
}
const std::string& GetType(const Handle& a_Instance)
{
	return a_Instance->type;
}
const Info& GetInfo(const Handle& a_Instance)
{
	return a_Instance->info;
}
const std::vector<PhysicalDevice::Handle>& EnumeratePhysicalDevices(const Instance::Handle& a_Instance)
{
	return a_Instance->physicalDevices;
}
void PushCommand(const Handle& a_Instance, const std::function<void()>& a_Command, const bool a_Synchronous)
{
    a_Instance->glThread.PushCommand(a_Command, a_Synchronous);
}
void MakeCurrent(const Handle& a_Instance, const HDC& a_HDC)
{
    a_Instance->glThread.PushCommand([hdc = a_HDC, hglrc = a_Instance->hglrc]{ wglMakeCurrent(hdc, hglrc); }, false);
}
}