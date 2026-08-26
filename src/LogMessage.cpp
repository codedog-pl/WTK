/**
 * @file        LogMessage.cpp
 * @author      Adam Łyskawa
 *
 * @brief       Log message class. Implementation.
 * @remark      A part of the Woof Toolkit (WTK).
 *
 * @copyright   (c)2026 CodeDog, All rights reserved.
 *
 *                / \__
 *               (    @\___
 *               /         O
 *              /   (_____/
 *              /_____/   U
 */

#include "LogMessage.hpp"
#include <cstdio>
#include <cstring>

LogMessage::LogMessage()
    : m_severity(debug), m_length(0), m_offset(0), m_buffer() { memset(m_buffer, 0, size); }

LogMessage::LogMessage(Severity s)
    : m_severity(s), m_length(0), m_offset(0), m_buffer() { memset(m_buffer, 0, size); }

LogMessage::LogMessage(const LogMessage& other)
    : m_severity(other.m_severity), m_length(other.m_length), m_offset(other.m_offset), m_buffer()
{
    memcpy(m_buffer, other.m_buffer, m_length);
}

void LogMessage::clear()
{
    m_offset = 0;
    m_length = 0;
    memset(m_buffer, 0, size);
}

LogMessage* LogMessage::printf(const char* format, ...)
{
    size_t available = m_offset < size ? size - m_offset : 0;
    va_list args;
    va_start(args, format);
    int l = vsnprintf((char*)(&m_buffer[m_offset]), available, format, args);
    va_end(args);
    if (l > 0) // `vsnprintf` returns the length that would have been written, it can exceed `available` on truncation.
    {
        if ((size_t)l > available) l = (int)available;
        m_offset += l;
        m_length += l;
    }
    return this;
}

LogMessage* LogMessage::vprintf(const char* format, va_list args)
{
    size_t available = m_offset < size ? size - m_offset : 0;
    int l = vsnprintf((char*)(&m_buffer[m_offset]), available, format, args);
    if (l > 0) // `vsnprintf` returns the length that would have been written, it can exceed `available` on truncation.
    {
        if ((size_t)l > available) l = (int)available;
        m_offset += l;
        m_length += l;
    }
    return this;
}

LogMessage* LogMessage::add(char c, int count)
{
    if (m_length + count > size) return this;
    for (int i = 0; i < count; i++)
    {
        m_buffer[m_offset] = c;
        m_offset++;
        m_length++;
    }
    return this;
}

LogMessage* LogMessage::add(const char* s)
{
    size_t l = strlen(s);
    if (m_length + l > size) return this;
    memcpy(&m_buffer[m_offset], s, l);
    m_offset += l;
    m_length += l;
    return this;
}

LogMessage* LogMessage::addTimestamp()
{
    DateTimeEx timestamp;
    if (timestamp.getRTC())
        return printf(
            dateTimeFormat,
            timestamp.year, timestamp.month, timestamp.day,
            timestamp.hour, timestamp.minute, timestamp.second + timestamp.fraction
        );
    else return this->add('*');
}
