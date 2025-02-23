/**
 * @file        PCM16S.hpp
 * @author      Adam Łyskawa
 *
 * @brief       A 16-bit stereo PCM sample type. Header file.
 * @remark      A part of the Woof Toolkit (WTK).
 *
 * @copyright   (c)2025 CodeDog, All rights reserved.
 */

#pragma once

#include <stdint.h>

/// @brief A pack of 2 signed 16-bit values.
struct __attribute__((__packed__)) __PCM16S_S
{
    int16_t left;   // Left channel sample value.
    int16_t right;  // Right channel sample value.
};

/// @brief A union allowing to treat data as separate channels or a combined 32-bit sample value.
union __PCM16S_U
{
    __PCM16S_S channels;    // Channels separated.
    uint32_t value;         // Channels combined.
};

/// @brief PCM16 stereo sample.
struct PCM16S
{

    /// @brief Assigns a signed 16-bit sample value to both channels.
    /// @param value A signed 16-bit sample value.
    /// @return This reference.
    PCM16S& operator=(int16_t value);

    /// @brief Assigns a stereo sample from an unsigned 32-bit integer value.
    /// @param value An unsigned 32-bit integer value containing 2 signed 16-bit values for each channel.
    /// @return This reference.
    PCM16S& operator=(uint32_t value);

    /// @brief Assings a normalized value from [-1.0 .. 1.0] range as the sample value for both channels.
    /// @param normalized A normalized sample value from [-1.0 .. 1.0] range.
    /// @return This reference.
    PCM16S& operator=(double normalized);

    /// @brief Assings a normalized value from [-1.0 .. 1.0] range as the sample value for both channels.
    /// @param normalized A normalized sample value from [-1.0 .. 1.0] range.
    /// @return This reference.
    PCM16S& operator=(float normalized);

    /// @brief Gets the underlying 32-bit combined sample data.
    operator uint32_t();

    __PCM16S_U sample; // 32 bits of sample data consisting of 2 signed 16-bit integer values for left and right channel.

};
