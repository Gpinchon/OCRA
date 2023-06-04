/*
 * @Author: gpinchon
 * @Date:   2021-09-26 00:00:00
 * @Last Modified by:   gpinchon
 * @Last Modified time: 2021-09-26 14:27:05
 */
#pragma once

#include <OCRA/Handles.hpp>
#include <OCRA/Structs.hpp>

#include <vector>

namespace OCRA::Queue {
void Submit(
    const Handle& a_Queue,
    const std::vector<QueueSubmitInfo>& a_SubmitInfos,
    const Fence::Handle& a_Fence = nullptr);
/**
 * @brief Waits for the queue to have completed its commands.
 * On OpenGL, this will push an empty synchronized command.
 */
void WaitIdle(const Handle& a_Queue);
}
