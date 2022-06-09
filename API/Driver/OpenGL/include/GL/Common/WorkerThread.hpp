#pragma once

#include <functional>
#include <condition_variable>
#include <queue>
#include <set>

namespace OCRA
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
            while (!_stop) {
                _cv.wait(lock, [this] {
                    return !_queue.empty() || _stop;
                });
                std::queue<Command> queue;
                _queue.swap(queue);
                lock.unlock();
                _taskMtx.lock();
                while (!queue.empty()) {
                    queue.front().fn();
                    if (queue.front().notify)
                        _finishedTasks.insert(queue.front().id);
                    queue.pop();
                }
                _taskMtx.unlock();
                if (!_finishedTasks.empty()) _taskCv.notify_all();
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
        const auto commandID = _commandID++;
        _mtx.lock();
        _queue.push({ a_Command, commandID, a_Synchronous });
        _mtx.unlock();
        _cv.notify_one();
        if (a_Synchronous) {
            auto lock = std::unique_lock<std::mutex>(_taskMtx);
            _taskCv.wait(lock, [this, &commandID] {
                const auto done = _finishedTasks.find(commandID) != _finishedTasks.end();
                if (done) _finishedTasks.erase(commandID);
                return done;
            });
        }
    }
    //Pushes an empty synchronous command to wait for the thread to be done
    inline void Wait()
    {
        PushCommand([]{}, true);
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
}