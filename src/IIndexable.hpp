/**
 * @file        IIndexable.hpp
 * @author      Adam Łyskawa
 *
 * @brief       An interface providing index operators.
 * @remark      A part of the Woof Toolkit (WTK).
 *
 * @copyright   (c)2025 CodeDog, All rights reserved.
 */

#pragma once

#include <cstddef>

/// @brief Provides index operators.
/// @tparam TElement Element type.
template<typename TElement>
class IIndexable
{

public:

    /// @brief Virtual destructor for proper cleanup of derived classes.
    virtual ~IIndexable() = default;

    /// @returns The element reference at index.
    virtual TElement& operator[](size_t index) = 0;

    /// @returns The const element reference at index.
    virtual const TElement& operator[](size_t index) const = 0;

};
