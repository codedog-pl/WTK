/**
 * @file        IIterable.hpp
 * @author      Adam Łyskawa
 *
 * @brief       An interface providing iterable collection methods (`begin` and `end`).
 * @remark      A part of the Woof Toolkit (WTK).
 *
 * @copyright   (c)2025 CodeDog, All rights reserved.
 */

#pragma once

#include "IndexIterator.hpp"

/// @brief Provides iterable collection methods (`begin` and `end`).
/// @tparam TDerived The type of collection that inherits from this.
/// @tparam TElement Element type.
template<typename TDerived, typename TElement>
class IIterable
{

public:

    /// @brief Virtual destructor for proper cleanup of derived classes.
    virtual ~IIterable() = default;

    using value_type = TElement;

    using iterator = IndexIterator<TDerived>;
    using const_iterator = IndexIterator<const TDerived>;

    /// @returns The iteraror for the first item.
    virtual iterator begin() = 0;

    /// @returns The iterator for the one over the last item.
    virtual iterator end() = 0;

    /// @returns The iteraror for the first item.
    virtual const_iterator begin() const = 0;

    /// @returns The iterator for the one over the last item.
    virtual const_iterator end() const = 0;

};
