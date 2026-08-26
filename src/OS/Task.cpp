/**
 * @file        Task.cpp
 * @author      Adam Łyskawa
 *
 * @brief       Scheduled task class. Implementation.
 * @remark      A part of the Woof Toolkit (WTK), RTOS API.
 *
 * @copyright   (c)2024 CodeDog, All rights reserved.
 */

#include "Task.hpp"
#include "AppThread.hpp"

void OS::Task::process(ThreadContext context, size_t* immediateCount, size_t* delayedCount)
{
    TaskControlBlock tcb; // Since we leave the critical section while the task is being run, we use a snapshot of the task control block.
    {
        CriticalSection::Scope cs;
        tcb = m_tcb;
    }
    if (!tcb.id || tcb.delayTicks || tcb.context != context) return;
    if (tcb.binding) tcb.action.binding(tcb.binding);
    else tcb.action.plain();
    bool recurring;
    {
        CriticalSection::Scope cs;
        recurring = m_tcb.resetTicks != 0;
        if (recurring)
        {
            m_tcb.delayTicks = m_tcb.resetTicks;
            if (immediateCount && *immediateCount) --*immediateCount; // The task stops being immediate...
            if (delayedCount) ++*delayedCount; // ...and becomes delayed.
        }
        else
        {
            if (immediateCount && *immediateCount) --*immediateCount;
            m_tcb.clear();
        }
    }
    // RTOS calls are kept outside the critical section by design - never block or call RTOS API while interrupts are masked.
    if (recurring) AppThread::resetDelayTick(); // unblocks the delay thread.
}

bool OS::Task::delayTick(size_t* immediateCount, size_t* delayedCount)
{
    bool zeroed;
    {
        CriticalSection::Scope cs;
        bool matched = m_tcb.id != 0 && m_tcb.delayTicks != 0;
        zeroed = matched && --m_tcb.delayTicks == 0;
    }
    if (zeroed)
    {
        if (immediateCount) ++*immediateCount; // The task becomes immediate...
        if (delayedCount && *delayedCount) --*delayedCount; // ...and stops being delayed.
    }
    return zeroed;
}

bool OS::Task::cancel(TaskId& id, size_t* immediateCount, size_t* delayedCount)
{
    bool matched;
    {
        CriticalSection::Scope cs;
        matched = m_tcb.id == id;
        if (matched)
        {
            id = 0;
            if (m_tcb.delayTicks)
            {
                if (delayedCount && *delayedCount) --*delayedCount;
            }
            else
            {
                if (immediateCount && *immediateCount) --*immediateCount;
            }
            m_tcb.clear();
        }
    }
    return matched;
}
