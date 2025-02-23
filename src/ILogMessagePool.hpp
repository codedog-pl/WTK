/**
 * @file        ILogMessagePool.hpp
 * @author      Adam Łyskawa
 *
 * @brief       Declares an interface for the system log message pool.
 * @remark      A part of the Woof Toolkit (WTK).
 *
 * @copyright   (c)2025 CodeDog, All rights reserved.
 */

#pragma once

#include "LogMessage.hpp"
#include <cstddef>
#include <tuple>

/// @brief An interface for a log message pool.
class ILogMessagePool
{

public:

    /// @returns The message pool capacity.
    virtual size_t size() = 0;

    /// @param state Message state to count.
    /// @returns The number of messages in the pool having the specified state.
    virtual size_t count(LogMessage::State state) = 0;

    /// @brief Finds a message with specified state starting from the beginning of the pool array.
    /// @param state Requested message state.
    /// @returns A message pointer or `nullptr` if not found.
    virtual LogMessage* find(LogMessage::State state) = 0;

    /// @brief Finds a messsage by its buffer pointer.
    /// @param ptr Buffer pointer.
    /// @return Message pointer or `nullptr` if not found.
    virtual LogMessage* findByBuffer(const uint8_t* ptr) = 0;

    /// @brief Finds a free message in the pool and marks it as taken if found.
    /// @param severity Message severity to set, default: debug.
    /// @returns A message pointer or `nullptr` if not found.
    virtual LogMessage* grab(LogMessage::Severity severity = LogMessage::Severity::debug) = 0;

    /// @brief Adds the message to the send queue.
    /// @param message Message pointer.
    virtual void queue(LogMessage* message) = 0;

    /// @brief Sets the message state to `sent`, making it effectively "in transit".
    /// @param message Message pointer.
    virtual void send(LogMessage* message) = 0;

    /// @brief Sets the pointed message as free again.
    /// @param message Message pointer.
    virtual void toss(LogMessage* message) = 0;

    /// @returns The element pointer at index, or nullptr on index out of bounds.
    virtual const LogMessage* operator[](size_t index) const = 0;

    /// @returns The element pointer at index, or nullptr on index out of bounds.
    virtual LogMessage* operator[](size_t index) = 0;

};
