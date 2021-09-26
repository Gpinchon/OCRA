/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:31:29
*/
#include <Device.hpp>
#include <Queue.hpp>

#include <map>
#include <mutex>
#include <queue>

namespace OCRA::Queue {
struct Impl {
    typedef std::queue<Pass::Handle> PassQueue;
    std::map<Device::Handle, PassQueue> passQueue;
    std::mutex lock;
} auto& GetQueue()
{
    static Impl queue;
    return queue;
}
void PushPass(const Device::Handle& a_Device, const Pass::Handle& a_Pass)
{
    const std::lock_guard<std::mutex> lock(GetQueue().lock);
    GetQueue().passQueue[a_Device].push(a_Pass);
}
void FlushPass(const Device::Handle& a_Device)
{
    const std::lock_guard<std::mutex> lock(GetQueue().lock);
    for (auto& passQueue : GetQueue().passQueue) {
        while (!passQueue.second.empty()) {
            auto pass { GetQueue().passQueue.second.front() };
            GetQueue().passQueue.second.pop();
            Device::RenderPass(passQueue.first, pass);
        }
    }
}
}