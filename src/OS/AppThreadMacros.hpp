/**
 * @file        AppThreadMacros.hpp
 * @author      Adam Łyskawa
 *
 * @brief       Macros used to synchronize calling instance methods from the main application thread.
 * @remark      A part of the Woof Toolkit (WTK), RTOS API.
 *
 * @copyright   (c)2025 CodeDog, All rights reserved.
 */

#pragma once

#include "AppThread.hpp"

/// @brief Synchronizes the expression with the UI frame thread.
/// @param code Expression to be synchronized.
#define FRAME_SYNC(code) OS::AppThread::sync([]{ code; }, OS::ThreadContext::frame);

/// @brief Synchronizes the expression with the main application thread.
/// @param code Expression to be synchronized.
#define SYNC_EXPR(code) OS::AppThread::sync([]{ code; });

/// @brief If called not from the application thread, schedules the static method to be called from the main application thread and exits.
/// @param method The name of the method to call from the main application thread.
#define SYNC_STATIC(method)\
if (!OS::AppThread::isCurrentThread())\
{\
    OS::AppThread::sync(method);\
    return; \
}

/// @brief If called not from the application thread, schedules the static method to be called from the main application thread and exits.
/// @param method The name of the method to call from the main application thread.
/// @param arg_t Argument type, must be representable as primitive 32-bit value!
/// @param arg Argument to pass.
#define SYNC_STATIC_WITH_ARG(method, arg_t, arg)\
if (!OS::AppThread::isCurrentThread())\
{\
    OS::AppThread::sync((void*)arg, [](void* _arg){ method((arg_t)_arg); });\
    return; \
}

/// @brief If called not from the application thread, schedules the instance method to be called from the main application thread and exits.
/// @param type The name of the class (this type).
/// @param method The name of the method to call from the main application thread.
#define SYNC_THIS(type, method)\
if (!OS::AppThread::isCurrentThread())\
{\
    OS::AppThread::sync(this, [](void* _this){ reinterpret_cast<type*>(_this)->method(); });\
    return; \
}
