/**
 * @file        Scale.hpp
 * @author      Adam Łyskawa
 *
 * @brief       Provides linear value scaling. Header only.
 * @remark      A part of the Woof Toolkit (WTK).
 *
 * @copyright   (c)2025 CodeDog, All rights reserved.
 *
 *                / \__
 *               (    @\___
 *               /         O
 *              /   (_____/
 *              /_____/   U
 */

 #pragma once

#include "StaticClass.hpp"

/// @brief Provides value scaling.
class Scale
{

    STATIC(Scale)

    /// @brief Scales a value from one range to another.
    /// @param x Value to be scaled.
    /// @param x0 Begin of source range.
    /// @param x1 End of source range.
    /// @param y0 Begin of target range.
    /// @param y1 End of source range.
    /// @return Scaled value.
    static double value(double x, double x0, double x1, double y0, double y1)
    {
        return (x - x0) * (y1 - y0) / (x1 - x0) + y0;
    }

};
