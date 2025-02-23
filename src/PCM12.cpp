/**
 * @file        PCM12.cpp
 * @author      Adam Łyskawa
 *
 * @brief       A 12-bit mono PCM sample type for STM32 DACs. Implementation.
 * @remark      A part of the Woof Toolkit (WTK).
 *
 * @copyright   (c)2025 CodeDog, All rights reserved.
 */

#include "PCM12.hpp"
#include <cmath>

PCM12& PCM12::operator=(uint16_t value)
{
    sample = value;
    return *this;
}

PCM12& PCM12::operator=(double normalized)
{
    if (normalized < -1.0) normalized = -1.0;
    else if (normalized > 1.0) normalized = 1.0;
    sample =  static_cast<uint16_t>(0x800 + static_cast<int16_t>(round(0x7ff * normalized)));
    return *this;
}

PCM12& PCM12::operator=(float normalized)
{
    if (normalized < -1.0f) normalized = -1.0f;
    else if (normalized > 1.0f) normalized = 1.0f;
    sample =  static_cast<uint16_t>(0x800 + static_cast<int16_t>(round(0x7ff * normalized)));
    return *this;
}

PCM12::operator uint16_t()
{
    return sample;
}
