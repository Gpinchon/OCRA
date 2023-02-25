#pragma once

namespace OCRA
{
enum class SwapChainPresentMode {
    Immediate, //No V-Sync
    Fifo,      //Yes V-Sync
    Mailbox,   //Present requests are queued
    MaxValue
};
}