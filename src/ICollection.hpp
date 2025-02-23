/**
 * @file        ICollection.hpp
 * @author      Adam Łyskawa
 *
 * @brief       An interface for a collection containing fixed size of elements.
 * @remark      A part of the Woof Toolkit (WTK).
 *
 * @copyright   (c)2025 CodeDog, All rights reserved.
 */

#pragma once

#include <cstddef>

/// @brief Provides an interface for a collection containing fixed size of elements.
/// @tparam TElement Element type.
template<typename TElement>
class ICollection
{

public:

    /// @brief Virtual destructor for proper cleanup of derived classes.
    virtual ~ICollection() = default;

    /// @returns The number of elements the collection contains.
    virtual size_t size() const = 0;

    /// @returns The pointer to the first element of the collection.
    virtual TElement* data() = 0;

    /// @returns The const pointer to the first element of the collection.
    virtual const TElement* data() const = 0;

};
