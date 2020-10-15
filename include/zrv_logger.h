#pragma once
#include "zarevo_base.h"
#include <fstream>

#if defined(ZRV_DEBUG) && !defined(ZRV_LOG)
#include <iostream>
#define ZRV_LOG std::cout
#elif !defined(ZRV_DEBUG) && !defined(ZRV_LOG)
static std::ofstream logger("log.txt", std::ios::app);
#define ZRV_LOG logger
#endif
