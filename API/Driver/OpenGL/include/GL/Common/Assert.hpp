#pragma once

#ifdef _DEBUG
#include <cassert>
#define OCRA_ASSERT(x) assert(x)
#else
#define OCRA_ASSERT(x)
#endif
