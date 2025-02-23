/**
 * @file        LogMessage.hpp
 * @author      Adam Łyskawa
 *
 * @brief       Log message class. Header file.
 * @remark      A part of the Woof Toolkit (WTK).
 *
 * @copyright   (c)2025 CodeDog, All rights reserved.
 */

#pragma once

#include <cstdarg>
#include <cstddef>
#include <cstdint>
#include "DateTimeEx.hpp"

#pragma pack(push, 1)
/// @brief System log message class. The instance takes (WTK_LOG_MSG_SIZE + 12) bytes.
class LogMessage final
{

public:

    /// @brief Message severity level.
    enum Severity : uint16_t { error, warning, info, debug, detail, spam };

    /// @brief Current message state.
    enum State : uint16_t { free, taken, qd, sent };

    /// @brief Contains packed buffer pointer and size.
    using Buffer = std::pair<const uint8_t*, size_t>;

public:

    /// @brief Creates a new log message with the default (debug) severity.
    LogMessage();

    /// @brief Creates a new log message with specified severity.
    LogMessage(Severity s);

    /// @brief Copies another message.
    /// @param other The other message reference.
    LogMessage(const LogMessage& other);

    LogMessage(LogMessage&& other) = delete; // Instances should not be moved.

    /// @brief Clears the message.
    void clear();

    /// @returns True if the message is empty / unset.
    inline bool empty() { return !m_offset || !m_length; }

    /// @brief Prints a formatted message into the message buffer.
    /// @param format Text format.
    /// @param ...Variadic arguments.
    /// @returns A pointer to the message.
    LogMessage* printf(const char* format, ...);

    /// @brief Prints a formatted message into the message buffer.
    /// @param format Text format.
    /// @param args An initialized variadic argument list.
    /// @returns A pointer to the message.
    LogMessage* vprintf(const char* format, va_list args);

    /// @brief Appends a character to the message.
    /// @param c The character to append.
    /// @param count Number of characters to append. Default 1.
    /// @returns A poiner to the message.
    LogMessage* add(char c, int count = 1);

    /// @brief Appeds a string to the message.
    /// @param s String to append.
    /// @returns A pointer to the message.
    LogMessage* add(const char* s);

    /// @brief Adds an ISO8601 timestamp to the message.
    /// @returns A pointer to the message.
    LogMessage* addTimestamp();

    /// @returns Message's buffer pointer and length in bytes as pair.
    inline Buffer buffer() const { return { (const uint8_t*)&m_buffer, m_length }; }

    /// @returns Message buffer pointer.
    inline uint8_t* ptr() const { return const_cast<uint8_t*>(m_buffer); }

    /// @returns Message length in bytes.
    inline size_t length() { return m_length; }

    /// @brief Gets the message character at the specified index.
    /// @param index Character index.
    /// @returns A pointer to the specific character in the message buffer or null pointer if out of bounds.
    inline uint8_t* operator[](size_t index) { return index < m_length ? &m_buffer[index] : nullptr; }

private:

friend class ILogMessagePool;
template<int TSize> friend class LogMessagePool;

    static constexpr int size = WTK_LOG_MSG_SIZE;                       // Pre-configured message size in bytes.
    static constexpr const char* dateTimeFormat = ISO_DATE_TIME_MS_F;   // Date format for messages.

    Severity m_severity = debug;                                        // Message severity level.
    State m_state = free;                                               // Current message state.

    size_t m_length = 0;                                                // Current buffer length.
    int m_offset = 0;                                                   // Current message offset.
    uint8_t m_buffer[size]{};                                           // Message buffer.

};
#pragma pack(pop)
