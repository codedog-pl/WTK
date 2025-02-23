/**
 * @file        DataSetT.hpp
 * @author      Adam Łyskawa
 *
 * @brief       An indexable and iterable, fixed size data set class template. Header only.
 * @remark      A part of the Woof Toolkit (WTK).
 *
 * @copyright	(c)2025 CodeDog, All rights reserved.
 */

#pragma once

#include "IndexIterator.hpp"
#include <limits>

/// @brief An indexable and iterable, fixed size data set class template.
/// @tparam TPoint Data point type.
/// @tparam TSize Number of data points in the set.
template<typename TPoint, size_t TSize>
class DataSetT
{

public:

    /// @brief A sentinel value for invalid index, when the set contains no elements [`(size_t)(-1)`].
    static constexpr size_t INVALID_INDEX = std::numeric_limits<size_t>::max();

    using value_type = TPoint;
    using iterator = IndexIterator<DataSetT>;
    using const_iterator = IndexIterator<const DataSetT>;

    /// @brief Creates an empty `DataSetT` instance.
    DataSetT() : m_lastIndex(INVALID_INDEX), m_points() { }

    /// @brief Tests if both data sets point to the same data array and have the same last index.
    /// @param other The other data set reference.
    /// @returns 1: The same data set. 0: Different data sets.
    bool operator==(DataSetT& other) const
    {
        return &m_points == &(other.m_points) && m_lastIndex == other.m_lastIndex;
    }

    /// @brief Tests if the data sets point to different data arrays or have different last indexes.
    /// @param other The other data set reference.
    /// @returns 1: The data sources are different. 0: The data sources are the same.
    bool operator!=(DataSetT& other) const
    {
        return &m_points != &(other.m_points) || m_lastIndex != other.m_lastIndex;
    }

    /// @brief Gets the point reference at specified index. No range checking is made.
    /// @param index Data point index.
    /// @returns Data point reference.
    TPoint& operator[](int index) const { return m_points[index]; }

    /// @brief Gets the point reference at specified index. No range checking is made.
    /// @param index Data point index.
    /// @returns Data point reference.
    TPoint& operator[](int index) { return m_points[index]; }

    /// @brief Returns the data set capacity as the maximum number of points that can be stored in this type.
    /// @returns
    inline static int capacity() { return TSize; }

    /// @returns The number of the data points actually stored in this instance.
    size_t length() const { return m_lastIndex + 1; }

    /// @returns The first data point reference.
    TPoint& first() const { return m_points[0]; }

    /// @returns The first data point reference.
    TPoint& first() { return m_points[0]; }

    /// @returns The last data point reference.
    TPoint& last() const { return m_points[m_lastIndex]; }

    /// @returns The last data point reference.
    TPoint& last() { return m_points[m_lastIndex]; }

    /// @returns The first point iterator.
    iterator begin() { return iterator(*this, 0); }

    /// @returns The first point iterator.
    const_iterator begin() const { return const_iterator(*this, 0); }

    /// @returns The point beyond the last point iterator.
    iterator end() { return iterator(*this, m_lastIndex + 1); }

    /// @returns The point beyond the last point iterator.
    const_iterator end() const { return const_iterator(*this, m_lastIndex + 1); }

    /// @returns A value indicating that the collection is empty.
    bool empty() const { return m_lastIndex == INVALID_INDEX; }

    /// @brief Adds a data point to the collection.
    /// @param point A data point to add.
    virtual void add(TPoint point)
    {
        if (m_lastIndex != INVALID_INDEX && m_lastIndex >= TSize - 1) return;
        m_points[++m_lastIndex] = point;
    }

    /// @brief Copies all the data points from another data set of the same type, zeroes the unused points.
    /// @param other The other data set reference.
    virtual void copyFrom(DataSetT& other)
    {
        for (size_t i = 0; i <= other.m_lastIndex; i++) m_points[i] = other.m_points[i];
        m_lastIndex = other.m_lastIndex;
        for (size_t i = m_lastIndex + 1; i < TSize; i++) m_points[i] = 0;
    }

    /// @brief Copies all the data points to the other data set of the same type, zeroes the unused points of the other set.
    /// @param other The other data set reference.
    virtual void copyTo(DataSetT& other)
    {
        for (size_t i = 0; i <= m_lastIndex; i++) other.m_points[i] = m_points[i];
        other.m_lastIndex = m_lastIndex;
        for (size_t i = other.m_lastIndex + 1; i < TSize; i++) other.m_points[i] = 0;
    }

    /// @brief Resets the collection to the empty state.
    virtual void zero()
    {
        m_lastIndex = INVALID_INDEX;
        for (size_t i = 0; i < TSize; i++) m_points[i] = 0;
    }

    /// @brief Tests if the other data set contains the same data.
    /// @param other The other data source reference.
    /// @returns 1: Both data sources contain the same data. 0: The other data source contains different data.
    bool contentEqual(DataSetT& other)
    {
        if (*this == other) return true;
        if (m_lastIndex != other.m_lastIndex) return false;
        for (size_t i = 0; i <= m_lastIndex; i++) if (m_points[i] != other.m_points[i]) return false;
        return true;
    }

private:

    size_t m_lastIndex = INVALID_INDEX; // Zero based last data point index, `INVALID_INDEX` if there are no datapoints in the set.
    TPoint m_points[TSize];             // An array to store data points.
    TPoint m_sentinel;                  // A sentinel value for invalid access.

};
