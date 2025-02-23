/**
 * @file        LogMessagePool.hpp
 * @author      Adam Łyskawa
 *
 * @brief       Implements the system log message pool with a static storage. Header only.
 * @remark      A part of the Woof Toolkit (WTK).
 *
 * @copyright   (c)2025 CodeDog, All rights reserved.
 */

#pragma once

#include "ILogMessagePool.hpp"
#include <new>
#include <tuple>

/// @brief Provides a preallocated log message pool that works as a queue.
/// @tparam TNum Pool capacity.
template<int TSize>
class LogMessagePool final : public ILogMessagePool
{

public:

    LogMessagePool() : m_messages() { }

    /// @returns The message pool capacity.
    size_t size() override { return TSize; }

    /// @param state Message state to count.
    /// @returns The number of messages in the pool having the specified state.
    size_t count(LogMessage::State state) override
    {
        size_t result = 0;
        for (LogMessage& m : m_messages) if (m.m_state == state) ++result;
        return result;
    }

    /// @brief Finds a message with specified state starting from the beginning of the pool array.
    /// @param state Requested message state.
    /// @returns A message pointer or `nullptr` if not found.
    LogMessage* find(LogMessage::State state) override
    {
        for (LogMessage& m : m_messages)
        {
            if (m.m_state == state) return &m;
        }
        return nullptr;
    }

    /// @brief Finds a messsage by its buffer pointer.
    /// @param ptr Buffer pointer.
    /// @return Message pointer or `nullptr` if not found.
    LogMessage* findByBuffer(const uint8_t* ptr) override
    {
        for (LogMessage& m : m_messages)
        {
            if (m.ptr() == ptr) return &m;
        }
        return nullptr;
    }

    /// @brief Finds a free message in the pool and marks it as taken if found.
    /// @returns A message pointer or `nullptr` if not found.
    LogMessage* grab(LogMessage::Severity severity = LogMessage::Severity::debug) override
    {
        auto msg = find(LogMessage::State::free);
        if (msg)
        {
            msg->m_state = LogMessage::State::taken;
            msg->m_severity = severity;
        }
        return msg;
    }

    /// @brief Adds the message to the send queue.
    /// @param message Message pointer.
    void queue(LogMessage* message) override
    {
        if (message && message->m_state == LogMessage::State::taken) message->m_state = LogMessage::State::qd;
    }

    /// @brief Sets the message state to `sent`, making it effectively "in transit".
    /// @param message Message pointer.
    void send(LogMessage* message) override
    {
        if (message && message->m_state == LogMessage::State::qd) message->m_state = LogMessage::State::sent;
    }

    /// @brief Sets the pointed message as free again.
    /// @param message Message pointer.
    void toss(LogMessage* message) override
    {
        if (message) message->m_state = LogMessage::State::free;
    }

    /// @returns The element pointer at index, or nullptr on index out of bounds.
    inline const LogMessage* operator[](size_t index) const override { return index < TSize ? &m_messages[index] : nullptr; }

    /// @returns The element pointer at index, or nullptr on index out of bounds.
    inline LogMessage* operator[](size_t index) override { return index < TSize ? &m_messages[index] : nullptr; }

private:

    LogMessage m_messages[TSize];   // Messages array.

};
