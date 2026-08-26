/**
 * @file        RTOS.hpp
 * @author      Adam Łyskawa
 *
 * @brief       Target RTOS bindings. Header file.
 * @remark      A part of the Woof Toolkit (WTK), RTOS API.
 *
 * @copyright   (c)2026 CodeDog, All rights reserved.
 *
 *                / \__
 *               (    @\___
 *               /         O
 *              /   (_____/
 *              /_____/   U
 */

#pragma once

#include "target.h"

#if defined(USE_AZURE_RTOS)

#include <cstdint>
#include "app_threadx.h"
#include "tx_api.h"
#include "tx_user.h"

namespace OS
{



using EventFlags = unsigned long;       // An integer containing event flags of the `EventGroup`.
using TickCount = unsigned int;         // An integer containing a number of RTOS ticks to wait.
using ThreadArg = unsigned long;        // Thread entry function argument type.
using ThreadEntry = void(*)(ThreadArg); // Thread entry function pointer type.
using ThreadHandle = TX_THREAD*;        // A pointer used to identify a RTOS thread.
using NativePriority = unsigned int;    // An integer containing numerical value of a thread priority.

/// @brief Contains the number of OS ticks per second read from the project configuration.
static constexpr double ticksPerSecond = TX_TIMER_TICKS_PER_SECOND;

}

#elif defined(USE_FREE_RTOS)

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "event_groups.h"
#include "semphr.h"

namespace OS
{

using EventFlags = EventBits_t;         // An integer containing event flags of the `EventGroup`.
using TickCount = TickType_t;           // An integer containing a number of RTOS ticks to wait.
using ThreadArg = void*;                // Thread entry function argument type.
using ThreadEntry = void(*)(ThreadArg); // Thread entry function pointer type.
using ThreadHandle = TaskHandle_t;      // A pointer used to identify a RTOS thread.
using NativePriority = uint32_t;        // An integer containing numerical value of a thread priority.

/// @brief Contains the number of OS ticks per second read from the project configuration.
static constexpr double ticksPerSecond = configTICK_RATE_Hz;

}

#endif

#if defined(USE_AZURE_RTOS) or defined(USE_FREE_RTOS)

// Common types and functions:

namespace OS
{

/// @brief Thread context type.
enum ThreadContext : uint32_t
{
    none,           // No context (uninitialized value).
    application,    // Main application thread, `dispatcherLoop` method.
    frame,          // Display thread, synchronized with display frame, `tick` method.
    nonISR          // Used to mark any thread except ISR.
};

/// @brief Task identifier integer. Zero means empty.
using TaskId = uint32_t;

/// @brief Options for the `EventGroup::wait` method.
enum WaitOptions : uint32_t
{
    waitAny,    // Wait for any flag.
    waitAll,    // Wait for all flags.
    noClear     // Do not clear flags which have been specified to wait for.
};

/// @brief A `TickCount` value indicating no timeout or infinite wait time.
static constexpr TickCount waitForever = static_cast<TickCount>(-1);

/// @brief Yields the execution of the current thread and lets the system resume other threads.
void yield(void);

/// @brief Blocks the current thread for the specified amount of RTOS ticks.
/// @param ticks The number of system ticks to wait.
void delay(TickCount ticks);

/// @brief Gets the number of system ticks since the current thread was started.
/// @returns The number of system ticks since the current thread was started.
TickCount getTick(void);

/// @brief Configured system tick period in seconds.
static constexpr double tickPeriod = 1.0 / static_cast<double>(
#if defined(USE_AZURE_RTOS)
    TX_TIMER_TICKS_PER_SECOND
#elif defined(USE_FREE_RTOS)
    configTICK_RATE_HZ
#else
    1000
#endif
);

}

#endif
