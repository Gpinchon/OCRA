#pragma once

#include <string>

namespace OCRA::Application
{
struct Info {
    std::string	name;
    uint32_t    applicationVersion{ 0 };
    std::string engineName;
    uint32_t    engineVersion{ 0 };
};
}