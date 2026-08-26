/**
 * @file        Task.hpp
 * @author      Adam Łyskawa
 *
 * @brief       Scheduled task class. Header file.
 * @remark      A part of the Woof Toolkit (WTK), RTOS API.
 *
 * @copyright   (c)2024 CodeDog, All rights reserved.
 */

#pragma once

#include "CriticalSection.hpp"
#include "TaskControlBlock.hpp"

namespace OS
{

/// @brief Scheduled task.
class Task final
{

public:

    /// @brief Creates an empty task.
    Task() : m_tcb() { }

    /// @brief Tests if the task is not empty. Thread and ISR safe.
    inline operator bool()
    {
        CriticalSection::Scope cs;
        return m_tcb.id != 0;
    }

friend class TaskScheduler;
private:

    /// @brief Locks the task access to the calling thread or ISR.
    ///        Must be paired with a matching `unlock` call before this task is locked again.
    /// @returns Always true (kept for interface compatibility).
    inline bool lock() { m_primask = CriticalSection::enter(); return true; }

    /// @brief Unlocks the task access previously locked by `lock`.
    /// @returns Always true (kept for interface compatibility).
    inline bool unlock() { CriticalSection::exit(m_primask); return true; }

    /// @brief Acquires the task assigning an identifier if it's empty. Thread and ISR safe.
    /// @returns True if a new empty task was acquired. False otherwise.
    inline bool acquire()
    {
        CriticalSection::Scope cs;
        return acquireUnsafe();
    }

    /// @brief Acquires the task assigning an identifier if it's empty. NOT THREAD SAFE.
    /// @returns True if a new empty task was acquired. False otherwise.
    inline bool acquireUnsafe()
    {
        bool empty = m_tcb.id == 0;
        if (empty) m_tcb.id = ++m_uid;
        return empty;
    }

    /// @brief Binds an action to the task. Thread safe.
    /// @param arg Binding argument. If set, the action will be calleded with it.
    /// @param action Action to call when `run` method is called.
    /// @param context Target thread context. Default: `application`.
    /// @param time The number of RTOS ticks to wait before the task can be run. Default: 0.
    /// @param reset The number of RTOS ticks to reset the `time` value to when it elapses. Default: 0.
    /// @returns Task identifier.
    inline TaskId schedule(void* arg, OptionalBindingAction action, ThreadContext context = application, TickCount time = 0, TickCount reset = 0)
    {
        CriticalSection::Scope cs;
        return scheduleUnsafe(arg, action, context, time, reset);
    }

    /// @brief Binds an action to the task. NOT THREAD SAFE!
    /// @param arg Binding argument. If set, the action will be calleded with it.
    /// @param action Action to call when `run` method is called.
    /// @param context Target thread context. Default: `application`.
    /// @param time The number of RTOS ticks to wait before the task can be run. Default: 0.
    /// @param reset The number of RTOS ticks to reset the `time` value to when it elapses. Default: 0.
    /// @returns Task identifier.
    inline TaskId scheduleUnsafe(void* arg, OptionalBindingAction action, ThreadContext context = application, TickCount time = 0, TickCount reset = 0)
    {
        m_tcb.binding = arg;
        m_tcb.action = action;
        m_tcb.context = context;
        m_tcb.delayTicks = time;
        m_tcb.resetTicks = reset;
        return m_tcb.id;
    }

    // Processes the task:
    // If the `id` is set and the `delayTicks` is zero, and the `context` is matched, the task action is called.
    // When the `resetTicks` is zero, the task is not recurring and will be cleared.
    // Otherwise, the `delayTicks` value will be reset to the value of `resetTicks`.
    void process(ThreadContext context, size_t* immediateCount = nullptr, size_t* delayedCount = nullptr);

    /// @brief Decreases the `delayTick` for the task, optionally updates tasks counters. Thread safe.
    /// @param immediateCount An optional pointer to the immediate tasks counter.
    /// @param delayedCount An optional pointer to the delayed tasks counter.
    /// @returns True if the delay was active and was zeroed. False otherwise.
    bool delayTick(size_t* immediateCount = nullptr, size_t* delayedCount = nullptr);

    /// @brief Clears the task control block if the identifier is matched. Thread safe.
    /// @param id Task identifier reference.
    /// @param immediateCount An optional pointer to the immediate tasks counter.
    /// @param delayedCount An optional pointer to the delayed tasks counter.
    /// @returns True if the task was canceled. False otherwise.
    bool cancel(TaskId& id, size_t* immediateCount = nullptr, size_t* delayedCount = nullptr);

    TaskControlBlock m_tcb;         // Task control block.
    uint32_t m_primask = 0;         // PRIMASK state saved by `lock`, consumed by the matching `unlock`. NOT reentrant.

    static inline TaskId m_uid = 0; // Unique identifier counter.

};

}
