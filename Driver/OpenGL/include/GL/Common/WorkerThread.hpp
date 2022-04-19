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
            while (true) {
                _cv.wait(lock, [this] {
                    return !_queue.empty() || _stop;
                });
                if (_stop) return;
                _queue.front().fn();
                if (_queue.front().notify) {
                    _finishedTasks.insert(_queue.front().id);
                    _taskCv.notify_all();
                }
                _queue.pop();
            }
        });
    }
    inline ~WorkerThread() {
        //std::unique_lock<std::mutex> lock(_mtx);
        _stop = true;
        _cv.notify_one();
        _thread.join();
    }
    inline void PushCommand(const std::function<void()>& a_Command, const bool a_Synchronous)
    {
        std::unique_lock<std::mutex> lock(_mtx);
        const auto commandID = _commandID++;
        _queue.push({ a_Command, commandID, a_Synchronous });
        _cv.notify_one();
        if (a_Synchronous) {
            _taskCv.wait(lock, [this, &commandID] {
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

    std::condition_variable _taskCv;
    std::set<uint64_t> _finishedTasks;

    std::thread _thread;
    bool _stop{ false };
    uint64_t _commandID{ 0 };
};
}