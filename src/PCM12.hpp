/**
 * @file        PCM12.hpp
 * @author      Adam Łyskawa
 *
 * @brief       A 12-bit mono PCM sample type for STM32 DACs. Header file.
 * @remark      A part of the Woof Toolkit (WTK).
 *
 * @copyright   (c)2025 CodeDog, All rights reserved.
 */

#pragma once

#include <cstdint>

/// @brief 12-bit audio sample.
struct PCM12
{

    /// @brief Assigns the sample value from an integer.
    /// @param value 16-bit integer value.
    /// @return This reference.
    PCM12& operator=(uint16_t value);

    /// @brief Asigns the sample value from a normalized [-1.0 .. 1.0] value. 0.0 will be mapped at 0x800 offset. -1.0 at 0x001.
    /// @param normalized A normalized sample value from [-1.0 .. 1.0] range.
    /// @return This reference.
    PCM12& operator=(double normalized);

    /// @brief Asigns the sample value from a normalized [-1.0 .. 1.0] value. 0.0 will be mapped at 0x800 offset. -1.0 at 0x001.
    /// @param normalized A normalized sample value from [-1.0 .. 1.0] range.
    /// @return This reference.
    PCM12& operator=(float normalized);

    /// @brief Converts the sample to a 16-bit integer.
    operator uint16_t();

    uint16_t sample; // 12 bits of sample data right aligned into unsigned 16-bit integer.

};
