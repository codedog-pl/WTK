/**
 * @file        HistoryList.hpp
 * @author      Adam Łyskawa
 *
 * @brief       A collection of a fixed amount of history elements
 *              that can be added, removed and iterated from the most recent one.
 *              Setting the page size allows the standard iterator to iterate over a subset of elements.
 * @remark      A part of the Woof Toolkit (WTK).
 *
 * @copyright   (c)2025 CodeDog, All rights reserved.
 */

#pragma once

#include <array>
#include <limits>
#include <new>
#include <type_traits>
#include "ICollection.hpp"
#include "IIndexable.hpp"
#include "IIterable.hpp"

 /// @brief A history (stack-like) list that is both indexable and iterable.
 /// @tparam TElement Element type.
 /// @tparam TSize Capacity.
template<typename TElement, size_t TSize>
class HistoryList :
    public ICollection<TElement>,
    public IIndexable<TElement>,
    public IIterable<HistoryList<TElement, TSize>, TElement>
{

public:

    /// @brief A sentinel value for invalid index, when the set contains no elements [`(size_t)(-1)`].
    static constexpr size_t INVALID_INDEX = std::numeric_limits<size_t>::max();

    using this_type = HistoryList<TElement, TSize>;
    using iterator = IndexIterator<this_type>;
    using const_iterator = IndexIterator<const this_type>;

    static constexpr size_t capacity = TSize; // The maximal number of elements.

public: // Constructor:

    /// @brief Creates a new empty history list.
    HistoryList() : m_length(0), m_offset(INVALID_INDEX), m_elements() { }

public: // ICollection:

    /// @returns The number of elements the backing array contains.
    size_t size() const override { return capacity; }

    /// @returns The pointer to the backing array.
    inline TElement* data() override { return &m_elements[0]; }

    /// @returns The read only pointer to the backing array.
    inline const TElement* data() const override { return &m_elements[0]; }

public: // IIndexable:

    /// @brief Gets the pointer to the element at the specified history level.
    /// @param level A history level, 0 is the most recent (current).
    /// @returns A reference to the element at the specified level.
    TElement& operator[](size_t level) override { return m_elements[(capacity + m_offset - level) % capacity]; }

    /// @brief Gets the pointer to the element at the specified history level.
    /// @param level A history level, 0 is the most recent (current).
    /// @returns A reference to the element at the specified level.
    const TElement& operator[](size_t level) const override { return m_elements[(capacity + m_offset - level) % capacity]; }

public: // IIterable:

    /// @brief Gets the iterator for the first (most recently added) item.
    /// @returns Iterator.
    iterator begin() override
    {
        return iterator(*this, 0);
    }

    /// @brief Gets the const iterator for the first (most recently added) item.
    /// @returns Iterator.
    const_iterator begin() const override
    {
        return const_iterator(*this, 0);
    }

    /// @brief Gets the iterator for the one over the last (non existing) item.
    /// @returns Iterator.
    iterator end() override
    {
        return iterator(*this, m_length);
    }

    /// @brief Gets the const iterator for the one over the last (non existing) item.
    /// @returns Iterator.
    const_iterator end() const override
    {
        return const_iterator(*this, m_length);
    }

public: // HistoryList API:

    /// @returns The current length of the list.
    inline unsigned int length() const { return m_length; }

    /// @brief Resets the history list.
    void reset()
    {
        m_length = 0;
        m_offset = INVALID_INDEX;
        trim();
    }

    /// @returns A reference to a new element.
    TElement& add(void)
    {
        ++m_length;
        if (m_length > TSize) m_length = TSize;
        m_offset = (m_offset + 1) % TSize;
        return m_elements[m_offset];
    }

    /// @returns True if there are any elements in the list.
    inline bool any() const { return m_length > 0; }

    /// @brief Removes the last added element from the list.
    /// @returns The previously added element's reference or the first (empty) element's reference.
    TElement& back()
    {
        if (m_length > 1)
        {
            resetElement(m_elements[m_offset]);
            --m_length;
            m_offset = m_offset > 0 ? m_offset - 1 : TSize - 1;
        }
        return current();
    }

    /// @returns The last added element's reference or the first (empty) element's reference.
    TElement& current() const { return m_length ? m_elements[m_offset] : m_elements[0]; }

protected: // Internal API (for storage implementation):

    /// @brief Resets the internal array element if the element's destructor can be called.
    /// @tparam U Element type for the template.
    /// @param element Internal array element reference.
    template <typename U>
    typename std::enable_if<!std::is_trivially_destructible<U>::value>::type
        resetElement(U& element) {
        element.~U();
        new (&element) U();
    }

    /// @brief Resets the internal array element if the element's destructor cannot be called.
    /// @tparam U Element type for the template.
    /// @param element Internal array element reference.
    template <typename U>
    typename std::enable_if<std::is_trivially_destructible<U>::value>::type
        resetElement(U& element) {
        new (&element) U();
    }

    /// @brief Trims the unset elements by calling the default constructor on them.
    void trim()
    {
        for (size_t i = m_length; i < capacity; i++) resetElement(m_elements[i]);
    }

    /// @returns Collection metadata pointer.
    void* metadata()
    {
        return &m_length;
    }

    /// @returns Collection metadata const pointer.
    const void* metadata() const
    {
        return &m_length;
    }

    /// @returns Elements data pointer.
    void* elements()
    {
        return &m_elements[0];
    }

    /// @returns Elements data const pointer.
    const void* elements() const
    {
        return &m_elements[0];
    }

    /// @returns The number of bytes taken by the elements.
    size_t elementsSize() const
    {
        return m_length * sizeof(TElement);
    }

protected: // Data:

    size_t m_length;                        // List length.
    size_t m_offset;                        // Current element offset.
    std::array<TElement, TSize> m_elements; // Elements array.

    /// @brief The number of bytes the "header" part of the class take (bytes before elements).
    /// @remark It works, because there is no padding for `size_t` and `int` for ARM32.
    static constexpr size_t metadataSize = sizeof(m_length) + sizeof(m_offset);

    /// @returns The total number of bytes used by the elements structure.
    static constexpr size_t maxElementsSize = sizeof(std::array<TElement, capacity>);

};
