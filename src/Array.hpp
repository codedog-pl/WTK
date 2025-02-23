/**
 * @file        Array.hpp
 * @author      Adam Łyskawa
 *
 * @brief       Provides an array data storage for a derived class, that is both indexable and iterable.
 * @remark      A part of the Woof Toolkit (WTK).
 *
 * @copyright   (c)2025 CodeDog, All rights reserved.
 */

#pragma once

#include <array>
#include <type_traits>
#include "ICollection.hpp"
#include "IIndexable.hpp"
#include "IIterable.hpp"

/// @brief Provides an array data storage for a derived class, that is both indexable and iterable.
/// @tparam TElement Element type.
/// @tparam TSize Number of elements.
template<typename TElement, size_t TSize>
class Array :
    public virtual ICollection<TElement>,
    public IIndexable<TElement>,
    public IIterable<Array<TElement, TSize>, TElement>
{

public:

    using this_type = Array<TElement, TSize>;
    using iterator = IndexIterator<this_type>;
    using const_iterator = IndexIterator<const this_type>;

    static constexpr size_t capacity = TSize; // The maximal number of elements.

public: // Constructors:

    /// @brief Initializes the backing array.
    Array() : m_elements(), m_sentinel() { }

    /// @brief Initializes the backing array with a copy of the other storage.
    /// @param other The other data storage.
    Array(const Array& other) : m_elements()
    {
        static_assert(std::is_trivially_copyable_v<TElement>, "TElement must be trivially copyable");
        memcpy(m_elements, other.m_elements, capacity * sizeof(TElement));
    }

    /// @brief This type is not moveable.
    Array(Array&&) = delete;

public: // IData implementation:

    /// @returns The element pointer at index, or nullptr if index out of bounds.
    inline TElement& operator[](size_t index) override
    {
        return index < capacity ? m_elements[index] : m_sentinel;
    }

    /// @returns The const element pointer at index, or nullptr if index out of bounds.
    inline const TElement& operator[](size_t index) const override
    {
        return index < capacity ? m_elements[index] : m_sentinel;
    }

    /// @returns The number of elements the backing array contains.
    size_t size() const override { return capacity; }

    /// @returns The pointer to the backing array.
    inline TElement* data() override { return &m_elements[0]; }

    /// @returns The read only pointer to the backing array.
    inline const TElement* data() const override { return &m_elements[0]; }

    /// @returns The iteraror for the first item.
    iterator begin() override
    {
        return iterator(*this, 0);
    }

    /// @returns The const iteraror for the first item.
    const_iterator begin() const override
    {
        return const_iterator(*this, 0);
    }

    /// @returns The iterator for the one over the last item.
    iterator end() override
    {
        return iterator(*this, capacity);
    }

    /// @returns The const iterator for the one over the last item.
    const_iterator end() const
    {
        return const_iterator(*this, capacity);
    }

protected:

    /// @brief Internal array of elements.
    std::array<TElement, capacity> m_elements;

    /// @brief A sentinel value for index out of range.
    TElement m_sentinel;

};
