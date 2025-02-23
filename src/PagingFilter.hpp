/**
 * @file        PageingFilter.hpp
 * @author      Adam Łyskawa
 *
 * @brief       Pageing filter for indexable collections. Header only.
 * @remark      A part of the Woof Toolkit (WTK).
 *
 * @copyright   (c)2025 CodeDog, All rights reserved.
 */

#pragma once

#include "IndexIterator.hpp"

/// @brief A filter providing index iterator over indexable collection that is divided into pages.
/// @tparam TIndexable The indexable collection type to iterate over.
template<class TIndexable>
class PagingFilter
{

public:

    using iterator = IndexIterator<TIndexable>;
    using const_iterator = IndexIterator<const TIndexable>;

public: // Constructors:

    /// @brief Creates a paging filter over an indexable collection.
    /// @param collection Indexable collection reference.
    /// @param pageSize The number of elements per page.
    PagingFilter(TIndexable& collection, size_t pageSize)
            : page(0), m_collection(collection), m_pageSize(pageSize) { }

    /// @brief This type is not copyable.
    PagingFilter(const PagingFilter&) = delete;

    /// @brief This type is not moveable.
    PagingFilter(PagingFilter&&) = delete;

public: // Iterator API:

    /// @returns The iterator for the first enumerated item.
    iterator begin()
    {
        unsigned int limit = m_collection.end().index();
        unsigned int pages = limit / m_pageSize + 1;
        unsigned int pageIndex = ((page % pages) * m_pageSize) % limit;
        return iterator(m_collection, pageIndex);
    }

    /// @returns The const iterator for the first enumerated item.
    const_iterator begin() const
    {
        unsigned int limit = m_collection.end().index();
        unsigned int pages = limit / m_pageSize + 1;
        unsigned int pageIndex = ((page % pages) * m_pageSize) % limit;
        return iterator(m_collection, pageIndex);
    }

    /// @returns The iterator for the first enumerated item that is off limit.
    iterator end()
    {
        unsigned int limit = m_collection.end().index();
        unsigned int pages = limit / m_pageSize + 1;
        unsigned int itemsLeft = limit - (((page % pages) * m_pageSize) % limit);
        unsigned int offLimitIndex = ((page % pages) * m_pageSize) % limit + (itemsLeft >= m_pageSize ? m_pageSize : itemsLeft);
        return iterator(m_collection, offLimitIndex);
    }

    /// @returns The const iterator for the first enumerated item that is off limit.
    const_iterator end() const
    {
        unsigned int limit = m_collection.end().index();
        unsigned int pages = limit / m_pageSize + 1;
        unsigned int itemsLeft = limit - (((page % pages) * m_pageSize) % limit);
        unsigned int offLimitIndex = ((page % pages) * m_pageSize) % limit + (itemsLeft >= m_pageSize ? m_pageSize : itemsLeft);
        return const_iterator(m_collection, offLimitIndex);
    }

    /// @returns The number of pages depending on the collection length and elements per page.
    size_t pages() const
    {
        return m_collection->end().index() / m_pageSize + 1;
    }

    size_t page; // Current page index for iterator begin() and end() function.

private:

    TIndexable& m_collection;   // The original m_collection reference.
    size_t m_pageSize;          // The number of elements per page. Zero disables paging iterator behavior.

};
