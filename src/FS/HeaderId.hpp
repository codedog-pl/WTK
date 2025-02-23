/**
 * @file        HeaderId.hpp
 * @author      Adam Łyskawa
 *
 * @brief       Short string identifying structure to use in custom file headers.
 * @remark      A part of the Woof Toolkit (WTK), File System API.
 *
 * @copyright	(c)2025 CodeDog, All rights reserved.
 */

#pragma once

#include <cstddef>
#include <cstring>

namespace FS
{

/// @brief Defines a short id header structure.
/// @tparam TSize Header length in bytes. Use `sizeof(TId)`!
/// @tparam TId String constant like `constexpr char TId[] = ...`.
template<size_t TSize, const char (&TId)[TSize]>
struct HeaderId
{

    char layout[TSize]; // File type identifier bytes.

    static constexpr size_t size = TSize; // Defined header size in bytes.

    /// @brief Creates an empty header with all its bytes set to zero.
    HeaderId() : layout() { }

    /// @brief Sets the defined file type identifier. Use before writing the file header.
    void setId(void)
    {
        std::memcpy(layout, TId, TSize);
    }

    /// @returns True if the current layout matches the defined file identifier. Use after loading the file header.
    bool validId(void) const
    {
        return std::memcmp(layout, TId, TSize) == 0;
    }

};

}