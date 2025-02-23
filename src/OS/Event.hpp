/**
 * @file        Event.hpp
 * @author      Adam Łyskawa
 *
 * @brief       A thread safe, universal, single subscriber `Event` class template.
 * @remark      A part of the Woof Toolkit (WTK), RTOS API.
 *
 * @copyright   (c)2025 CodeDog, All rights reserved.
 */


#pragma once

#include "OS/AppThread.hpp"
#include "OS/Mutex.hpp"

#include <atomic>

namespace OS
{

/// @brief Event template class.
/// @tparam TSubscriber Subscriber class type.
/// @tparam TArg Argument type.
template<class TSubscriber, class TArg>
class Event
{
    /// @brief A caller member function that can receive the argument reference.
    using THandler = void(TSubscriber::*)(TArg&);

public:

    /// @brief Creates an unsubscribed event. Use subscribe method to subscribe to it.
    /// @param context Target thread context the event is synchronized to, default is application.
    Event(ThreadContext context = OS::application)
        : m_instance(), m_handler(), m_targetContext(context), m_mutex(), m_pending() { }

    /// @brief Creates a subscribed event.
    /// @param instance Subscriber instance.
    /// @param handler Subscriber method receiving the argument reference.
    /// @param context Target thread context the event is synchronized to, default is application.
    Event(TSubscriber* instance, THandler handler, ThreadContext context = OS::application)
        : m_instance(instance), m_handler(handler), m_targetContext(context), m_mutex(), m_pending() { }

    /// @brief Unsubscribe the handler if the instance is destroyed while the synchronization call is pending.
    ~Event() { unsubscribe(); }

    /// @brief Subscribes a handler to the event.
    /// @param instance Caller instance pointer.
    /// @param handler Caller member function that will receive the event argument reference.
    void subscribe(TSubscriber* instance, THandler handler)
    {
        OS::MutexLock(m_mutex);
        m_instance = instance;
        m_handler = handler;
    }

    /// @brief Unsubscribes handler from the event to avoid dangling pointer if the instance gets released.
    void unsubscribe()
    {
        OS::MutexLock(m_mutex);
        m_instance = nullptr;
        m_handler = nullptr;
        m_pending = false;
    }

    /// @brief Calls the subscribed handler from the configured thread.
    ///        When a previous call is still pending, the argument is overwritten,
    ///        and the new call is dropped to prevent synchronization queue overflow.
    /// @param arg Argument structure to pass.
    void call(TArg&& arg)
    {
        m_argument = std::move(arg);
        if (m_pending.exchange(true)) return;
        if (OS::AppThread::isCurrentThread(m_targetContext))
        {
            OS::MutexLock(m_mutex);
            if (m_instance && m_handler)
                (m_instance->*m_handler)(m_argument);
            m_pending = false;
        }
        else
        {
            OS::AppThread::sync(this, [](void* context) {
                auto* event = reinterpret_cast<Event<TSubscriber, TArg>*>(context);
                OS::MutexLock(m_mutex);
                if (event->m_instance && event->m_handler)
                    (event->m_instance->*(event->m_handler))(event->m_argument);
                event->m_pending = false;
            }, m_targetContext);
        }
    }

private:

    TArg m_argument;                // Stores the event argument.
    TSubscriber* m_instance;            // Pointer to the caller instance.
    THandler m_handler;             // Pointer to the member function handler.
    ThreadContext m_targetContext;  // Target thread context.
    Mutex m_mutex;                  // Protects the concurrent access to the member variables.
    std::atomic_bool m_pending;     // A flag indicating that the event is pending.

};

}
