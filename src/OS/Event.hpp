/**
 * @file        Event.hpp
 * @author      Adam Łyskawa
 *
 * @brief       A thread safe, universal, single subscriber `Event` class template.
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

#include "OS/AppThread.hpp"
#include "OS/Mutex.hpp"

#include <atomic>

/// @brief No argument structure for templates requiring the argument type.
struct NoArg {};

namespace OS
{

/// @brief An interface for an event accepting a specific type of argument.
/// @tparam TArg Argument type.
template<typename TArg>
class IEvent
{
public:
    virtual void call(TArg&& arg) = 0;
    virtual ~IEvent() = default;
};

/// @brief Event template class.
/// @tparam TSubscriber Subscriber class type.
/// @tparam TArg Argument type.
template<class TSubscriber, typename TArg>
class Event final : public IEvent<TArg>
{
    /// @brief A caller member function that can receive the argument reference.
    using THandler = void(TSubscriber::*)(TArg&&);

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
        OS::MutexLock lock(m_mutex);
        m_instance = instance;
        m_handler = handler;
    }

    /// @brief Unsubscribes handler from the event to avoid dangling pointer if the instance gets released.
    void unsubscribe()
    {
        OS::MutexLock lock(m_mutex);
        if (!m_handler) return;
        m_instance = nullptr;
        m_handler = nullptr;
        TickCount timeout = ticksPerSecond; // No event should lag over a second.
        while (m_pending.load(std::memory_order_acquire))
        {
            if (timeout-- == 0)
            {
                Crash::here(); // Pending call completion timed out!
            }
            OS::yield();
        }
    }

    /// @brief Calls the subscribed handler from the configured thread.
    ///        When a previous call is still pending, the argument is overwritten,
    ///        and the new call is dropped to prevent synchronization queue overflow.
    /// @param arg Argument structure to pass.
    void call(TArg&& arg)
    {
        m_argument = std::move(arg);
        if (m_pending.exchange(true, std::memory_order_acquire)) return;
        if (OS::AppThread::isCurrentThread(m_targetContext))
        {
            OS::MutexLock lock(m_mutex);
            if (m_instance && m_handler)
                (m_instance->*m_handler)(std::move(m_argument));
            m_pending.store(false, std::memory_order_release);
        }
        else
        {
            OS::AppThread::sync(this, [](void* context) {
                auto* event = reinterpret_cast<Event<TSubscriber, TArg>*>(context);
                OS::MutexLock lock(event->m_mutex);
                if (event->m_instance && event->m_handler)
                    (event->m_instance->*(event->m_handler))(std::move(event->m_argument));
                event->m_pending.store(false, std::memory_order_release);
            }, m_targetContext);
        }
    }

private:

    TArg m_argument;                // Stores the event argument.
    TSubscriber* m_instance;        // Pointer to the caller instance.
    THandler m_handler;             // Pointer to the member function handler.
    ThreadContext m_targetContext;  // Target thread context.
    Mutex m_mutex;                  // Protects the concurrent access to the member variables.
    std::atomic<bool> m_pending;    // A flag indicating that the event is pending.

};

}
