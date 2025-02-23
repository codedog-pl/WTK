/**
 * @file        AppThread.hpp
 * @author      Adam Łyskawa
 *
 * @brief       Provides main application thread static methods. Header only.
 * @remark      A part of the Woof Toolkit (WTK), RTOS API.
 *
 * @copyright   (c)2025 CodeDog, All rights reserved.
 */

#pragma once

#include "StaticClass.hpp"
#include "TaskScheduler.hpp"
#include "CurrentThread.hpp"
#include "Crash.hpp"

namespace OS
{

/// @brief Provides main application thread static methods.
class AppThread final
{
    STATIC(AppThread)

public:

    static inline void init() { m_appThreadHandle = CurrentThread::get().handle(); }

    /// @brief Starts the main application thread task scheduler.
    static inline void start() { m_scheduler.start(); }

    /// @brief Calls actions scheduled to the GUI display frame.
    static inline void frame()
    {
        if (!m_frameThreadHandle) m_frameThreadHandle = CurrentThread::get().handle();
        m_scheduler.frameTick();
    }

    /// @brief Checks if the current thread is the app thread.
    /// @return 1: Called from the app thread. 0: Called from another thread.
    static inline bool isCurrentThread(ThreadContext context = application)
    {
        OS::ThreadHandle currentThread = CurrentThread::get().handle();
        switch (context)
        {
        case ThreadContext::none: return currentThread == 0;
        case ThreadContext::application: return currentThread == m_appThreadHandle;
        case ThreadContext::frame: return currentThread == m_frameThreadHandle;
        default: return currentThread != 0; // At least not ISR.
        }
    }

    /// @brief Register a function that will be called when the application thread check fails.
    /// @param action Action to call when the application thread check fails.
    static inline void registerInvalidTheadHandler(Action action) { m_invalidThread = action; }

    /// @brief If this function is called not from the app thread invalid thread handler is called
    //         or the application just crashes here.
    static inline void assertThread(ThreadContext context = application)
    {
        if (!isCurrentThread(context)) m_invalidThread ? m_invalidThread() : Crash::here();
    }

    /// @brief Synchronizes the action with the current thread if this is callled from another thread.
    /// @param action A pointer to a function that takes no arguments.
    static inline void syncIfAnotherThread(Action action)
    {
        if (isCurrentThread()) action(); else sync(action, application);
    }

    /// @brief Synchronizes the action with the current thread if this is callled from another thread.
    /// @param argument A pointer to pass to the function being called.
    /// @param action A pointer to a function that takes one pointer argument.
    static inline void syncIfAnotherThread(void* argument, BindingAction action)
    {
        if (isCurrentThread()) action(argument); else sync(argument, action, application);
    }

    /// @brief Schedules the action to be executed in the selected thread context.
    /// @param action Action that passes no argument.
    /// @param context Target thread context.
    static inline void sync(Action action, ThreadContext context = application)
    {
        m_scheduler.schedule(nullptr, action, context, 0, 0);
    }

    /// @brief Schedules the action to be executed in the selected thread context.
    /// @param argument Pointer to pass to the action.
    /// @param action Action that passes an argument pointer.
    /// @param context Target thread context.
    static inline void sync(void* argument, BindingAction action, ThreadContext context = application)
    {
        m_scheduler.schedule(argument, action, context, 0, 0);
    }

    /// @brief Schedules the action to be executed when the `time` elapses.
    /// @param time The number of RTOS ticks to wait.
    /// @param action Action that passes no argument.
    /// @param context Target thread context.
    /// @returns Unique task identifier that can be used to cancel a scheduled task.
    static inline TaskId delay(TickCount time, Action action, ThreadContext context = application)
    {
        return m_scheduler.schedule(nullptr, action, context, time, 0);
    }

    /// @brief Schedules the action to be executed when the `time` elapses.
    /// @param time The number of RTOS ticks to wait.
    /// @param argument Pointer to pass to the action.
    /// @param action Action that passes an argument pointer.
    /// @param context Target thread context.
    /// @returns Unique task identifier that can be used to cancel a scheduled task.
    static inline TaskId delay(TickCount time, void* argument, BindingAction action, ThreadContext context = application)
    {
        return m_scheduler.schedule(argument, action, context, time, 0);
    }

    /// @brief Schedules the action to be repeated when the `time` elapses with the regular `time` interval.
    /// @param time The number of RTOS ticks to wait.
    /// @param action Action that passes no argument.
    /// @param context Target thread context.
    /// @returns Unique task identifier that can be used to cancel a scheduled task.
    static inline TaskId repeat(TickCount time, Action action, ThreadContext context = application)
    {
        return m_scheduler.schedule(nullptr, action, context, time, time);
    }

    /// @brief Schedules the action to be repeated when the `time` elapses with the regular `time` interval.
    /// @param time The number of RTOS ticks to wait.
    /// @param argument Pointer to pass to the action.
    /// @param action Action that passes an argument pointer.
    /// @param context Target thread context.
    /// @returns Unique task identifier that can be used to cancel a scheduled task.
    static inline TaskId repeat(TickCount time, void* argument, BindingAction action, ThreadContext context = application)
    {
        return m_scheduler.schedule(argument, action, context, time, time);
    }

    /// @brief Cancels an active task. Thread safe.
    /// @param id Task identifier reference. Gets zeroed if task canceled.
    static inline void cancel(TaskId& taskId) { if (taskId) m_scheduler.cancel(taskId); }

private:
friend class Task;

    /// @brief Resets the delay tick semaphore.
    static inline void resetDelayTick() { m_scheduler.resetDelayTick(); }

    /// @brief Task scheduler instance for the application.
    static inline TaskScheduler m_scheduler{};

    /// @brief Application thread handle.
    static inline ThreadHandle m_appThreadHandle{};

    static inline ThreadHandle m_frameThreadHandle{};

    /// @brief A function pointer to a function that is called when application thread assertion is failed.
    static inline Action m_invalidThread{};

};

}
