#pragma once

#include <Handle.hpp>
#include <Instance.hpp>

#include <functional>
#include <condition_variable>
#include <queue>
#include <set>

#ifdef _WIN32
#include <windows.h>
#endif

OCRA_DECLARE_HANDLE(OCRA::Instance);

namespace OCRA::Instance
{
class WorkerThread
{
public:
    struct Command {
        const std::function<void()> fn;
        const uint64_t id;
        const bool notify;
    };
    inline WorkerThread() {
        _thread = std::thread([this] {
            std::unique_lock<std::mutex> lock(_mtx);
            while (true) {
                _cv.wait(lock, [this] {
                    return !_queue.empty() || _stop;
                });
                if (_stop) return;
                Command command(std::move(_queue.front()));
                _queue.pop();
                lock.unlock();
                command.fn();
                if (command.notify) {
                    _taskMtx.lock();
                    _finishedTasks.insert(command.id);
                    _taskMtx.unlock();
                    _taskCv.notify_all();
                }
                lock.lock();
            }
        });
    }
    inline ~WorkerThread() {
        _stop = true;
        _cv.notify_one();
        _thread.join();
    }
    inline void PushCommand(const std::function<void()>& a_Command, const bool a_Synchronous)
    {
        _mtx.lock();
        const auto commandID = _commandID++;
        _queue.push({ a_Command, commandID, a_Synchronous });
        _mtx.unlock();
        _cv.notify_one();
        if (a_Synchronous) {
            _taskCv.wait(std::unique_lock<std::mutex>(_taskMtx), [this, &commandID] {
                const auto done = _finishedTasks.find(commandID) != _finishedTasks.end();
                if (done) _finishedTasks.erase(commandID);
                return done;
            });
        }
    }
private:
    std::mutex _mtx;
    std::condition_variable _cv;
    std::queue<Command> _queue;

    std::mutex _taskMtx;
    std::condition_variable _taskCv;
    std::set<uint64_t> _finishedTasks;

    std::thread _thread;
    bool _stop{ false };
    uint64_t _commandID{ 0 };
};
struct Impl
{
    Impl(const Info& a_Info);
    ~Impl();
    inline void PushCommand(const std::function<void()>& a_Command, const bool a_Synchronous) {
        glThread.PushCommand(a_Command, a_Synchronous);
    }
#ifdef _WIN32
    HDC         hdc;
    HWND        hwnd;
    HGLRC       hglrc;
    WorkerThread glThread;
#endif //_WIN32
    const Info info;
    std::vector<PhysicalDevice::Handle> physicalDevices;
    static constexpr auto type{ "OpenGL" };
};
void MakeCurrent(const Handle& a_Instance, const HDC& a_HDC);
}