/**
 * @file        TimeSpan.cpp
 * @author      Adam Łyskawa
 *
 * @brief       A structure to store and manipulate time span values. Header file.
 * @remark      A part of the Woof Toolkit (WTK).
 *
 * @copyright	(c)2024 CodeDog, All rights reserved.
 */

#include "FloatingPointEq.hpp"
#include "TimeSpan.hpp"
#include <cmath>

TimeSpan::TimeSpan() : m_value(0) {}

TimeSpan::TimeSpan(double value) : m_value(value) {}

TimeSpan::TimeSpan(uint16_t hours, uint8_t minutes, double seconds)
    : m_value(hours * 3600.0 + minutes * 60.0 + seconds) {}

TimeSpan::TimeSpan(int days, uint8_t hours, uint8_t minutes, double seconds)
    : m_value(days * 86400.0 + hours * 3600.0 + minutes * 60.0 + seconds) {}

double TimeSpan::seconds() const { return std::fmod(std::fabs(m_value), 60.0); }

uint8_t TimeSpan::minutes() const { return (static_cast<int>(std::fabs(m_value)) / 60) % 60; }

uint8_t TimeSpan::hours() const { return (static_cast<int>(std::fabs(m_value)) / 3600) % 24; }

int TimeSpan::days() const { return static_cast<int>(std::fabs(m_value)) / 86400; }

int TimeSpan::sign() const
{
    if (!m_value) return 0;
    return std::signbit(m_value) ? -1.0 : 1.0;
}

double TimeSpan::totalSeconds() const { return std::fabs(m_value); }

double TimeSpan::totalMinutes() const { return std::fabs(m_value) / 60.0; }

double TimeSpan::totalHours() const { return std::fabs(m_value) / 3600.0; }

double TimeSpan::totalDays() const { return std::fabs(m_value) / 86400.0; }

TimeSpan& TimeSpan::operator+=(TimeSpan &other)
{
    m_value += other.m_value;
    return *this;
}

TimeSpan &TimeSpan::operator-=(TimeSpan &other)
{
    m_value -= other.m_value;
    return *this;
}

TimeSpan &TimeSpan::operator+=(double other)
{
    m_value += other;
    return *this;
}

TimeSpan &TimeSpan::operator-=(double other)
{
    m_value -= other;
    return *this;
}

bool TimeSpan::operator==(TimeSpan &other) const
{
    return eq(m_value, other.m_value);
}

bool TimeSpan::operator!=(TimeSpan &other) const
{
    return !eq(m_value, other.m_value);
}

bool TimeSpan::operator<=(TimeSpan &other) const
{
    return m_value <= other.m_value;
}

bool TimeSpan::operator>=(TimeSpan &other) const
{
    return m_value >= other.m_value;
}

bool TimeSpan::operator<(TimeSpan &other) const
{
    return m_value < other.m_value;
}

bool TimeSpan::operator>(TimeSpan &other) const
{
    return m_value > other.m_value;
}
