/**
 * @file        IndexIterator.hpp
 * @author      Adam Łyskawa
 *
 * @brief       Simple bidirectional sequential iterator for indexable collections. Header only.
 * @remark      A part of the Woof Toolkit (WTK).
 *
 * @copyright   (c)2025 CodeDog, All rights reserved.
 */

#pragma once

#include <cstddef>
#include <iterator>
#include <type_traits>

// Trait to check for value_type
template <typename, typename = void>
struct has_value_type : std::false_type {};
template <typename T>
struct has_value_type<T, std::void_t<typename T::value_type>> : std::true_type {};

// Trait to check for operator[]
template <typename, typename = void>
struct has_subscript_operator : std::false_type {};
template <typename T>
struct has_subscript_operator<T, std::void_t<decltype(std::declval<T&>()[std::declval<std::size_t>()])>> : std::true_type {};

/// @brief Iterates over indexable collection.
/// @tparam TIndexable A collection that provides both typename `value_type` and `value_type& operator[]`.
template<class TIndexable>
class IndexIterator
{

    static_assert(has_value_type<TIndexable>::value, "TIndexable must have a value_type member.");
    static_assert(has_subscript_operator<TIndexable>::value, "TIndexable must support operator[] with std::size_t.");

public: // Iterator traits:

    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = typename TIndexable::value_type;
    using pointer = value_type*;
    using reference = value_type&;

public: // API:

    /// @brief Creates the iterator for the specified collection and index.
    /// @param collection Collection pointer.
    /// @param index Element index.
    IndexIterator(TIndexable& collection, size_t index) : m_collection(collection), m_index(index) { }

    /// @brief Pre-increments the iterator.
    /// @returns Incremented iterator.
    IndexIterator& operator++()
    {
        m_index++;
        return *this;
    }

    /// @brief Post-increments the iterator.
    /// @returns Iterator before it was incremented.
    IndexIterator operator++(int)
    {
        IndexIterator iterator = *this;
        ++(*this);
        return iterator;
    }

    /// @brief Pre-decrements the iterator.
    /// @returns Decremented iterator.
    IndexIterator& operator--()
    {
        m_index--;
        return *this;
    }

    /// @brief Post-decrements the iterator.
    /// @returns Iterator before it was decremented.
    IndexIterator operator--(int)
    {
        IndexIterator iterator = *this;
        --(*this);
        return iterator;
    }

    /// @returns The element's reference.
    reference operator*()
    {
        return m_collection[m_index];
    }

    /// @returns The element's reference.
    reference operator*() const
    {
        return m_collection[m_index];
    }

    /// @returns The element's pointer.
    pointer operator->()
    {
        return &(m_collection[m_index]);
    }

    /// @returns The element's pointer.
    pointer operator->() const
    {
        return &(m_collection[m_index]);
    }

    /// @brief Equality operator.
    /// @param other The other iterator reference.
    /// @returns True if the iterators are considered equal.
    bool operator==(const IndexIterator& other) const
    {
        return &m_collection == &other.m_collection && m_index == other.m_index;
    }

    /// @brief Inequality operator.
    /// @param other The other iterator reference.
    /// @return True if the iterators are considered not equal.
    bool operator!=(const IndexIterator& other) const
    {
        return &m_collection != &other.m_collection || m_index != other.m_index;
    }

    /// @returns The index of the iterator.
    size_t index() const
    {
        return m_index;
    }

private:

    TIndexable& m_collection;   // Collection reference.
    size_t m_index;             // Current iterator index.

};
