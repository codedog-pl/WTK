/**
 * @file        Range.hpp
 * @author      Adam Łyskawa
 *
 * @brief       Universal floating points numbers iterator. Header only.
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

#pragma once

#include <cmath>

/// @brief Iterates between a range of numbers, optionally with non-linear (eased) spacing.
class RangeIterator
{

    friend class Range;

public:

    /// @brief Creates the number iterator instance.
    /// @param start A value that starts the sequence.
    /// @param end A value that ends the sequence.
    /// @param numValues The number of values in the sequence.
    /// @param gamma Shaping exponent: 1.0 = linear (default).
    ///              Greater than 1.0 = values densify towards `end`.
    ///              Less than 1.0 = values densify towards `start`.
    RangeIterator(double start, double end, size_t numValues, double gamma = 1.0) :
        RangeIterator(start, end, numValues, gamma, 0) { }

    /// @brief Creates a zero iterator that doesn't produce anything.
    RangeIterator() :
        m_start(0),
        m_end(0),
        m_numValues(0),
        m_gamma(1.0),
        m_index(0) { }

    /// @brief Gets the current index of the iterator.
    /// @return Current index value.
    size_t index() const { return m_index; }

    /// @brief Implements value pre-incrementation.
    /// @return This reference.
    RangeIterator& operator++()
    {
        if (m_index < m_numValues) ++m_index;
        return *this;
    }

    /// @brief Implements value post-incrementation.
    /// @return Copy of the iterator before incrementation.
    RangeIterator operator++(int)
    {
        RangeIterator temp = *this;
        ++(*this);
        return temp;
    }

    /// @brief Tests if the instances of the iterator are equal.
    /// @param other Other iterator instance reference.
    /// @return True if the instances are equal.
    bool operator==(const RangeIterator& other) const
    {
        return m_index == other.m_index;
    }

    /// @brief Tests if the instances of the iterator are NOT equal.
    /// @param other Other iterator instance reference.
    /// @return True if the instances are NOT equal.
    bool operator!=(const RangeIterator& other) const
    {
        return !(*this == other);
    }

    /// @return The current, gamma-shaped value.
    double operator*() const
    {
        if (m_numValues <= 1) return m_end;
        double t = static_cast<double>(m_index) / static_cast<double>(m_numValues - 1);
        double shaped = (m_gamma == 1.0) ? t : (1.0 - std::pow(1.0 - t, m_gamma));
        return m_start + (m_end - m_start) * shaped;
    }

private:

    RangeIterator(double start, double end, size_t numValues, double gamma, size_t startIndex) :
        m_start(start),
        m_end(end),
        m_numValues(numValues),
        m_gamma(gamma),
        m_index(startIndex) { }

    double m_start;     // A value that starts the sequence.
    double m_end;       // A value that ends the sequence.
    size_t m_numValues; // The number of values in the sequence.
    double m_gamma;     // Shaping exponent, 1.0 = linear.
    size_t m_index;     // Current value index.

};

/// @brief Generates a sequence of `numValues` values from `start` to `end` inclusive,
///        optionally densified towards one end using a power-law easing curve.
class Range
{

public:

    /// @brief Generates a sequence of `numValues` values from `start` to `end` inclusive.
    /// @param start First generated value.
    /// @param end Last generated value.
    /// @param numValues Number of generated values.
    /// @param gamma Shaping exponent: 1.0 = equal spacing (default, matches prior behavior).
    ///              Greater than 1.0 = denser sampling near `end`.
    ///              Less than 1.0 = denser sampling near `start`.
    Range(double start, double end, size_t numValues, double gamma = 1.0) :
        m_start(start),
        m_end(end),
        m_numValues(numValues),
        m_gamma(gamma) { }

    /// @returns The first term iterator.
    RangeIterator begin() const
    {
        return RangeIterator(m_start, m_end, m_numValues, m_gamma);
    }

    /// @returns The "last + 1" term iterator.
    RangeIterator end() const
    {
        return RangeIterator(m_start, m_end, m_numValues, m_gamma, m_numValues);
    }

private:
    double m_start;     // First value.
    double m_end;       // Last value.
    size_t m_numValues; // Number of values.
    double m_gamma;     // Shaping exponent, 1.0 = linear.

};
