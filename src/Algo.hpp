/**
 * @file        Algo.hpp
 * @author      Adam Łyskawa
 *
 * @brief       Provides reusable common math algorithms.
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
#include <cstddef>

/// @brief Provides requsable common math algorithms.
class Algo
{

    STATIC(Algo)

    /// @brief Projects a value `x` from `[x0..x1]` range to `[y0..y1]` range.
    /// @param x Value to be projected.
    /// @param x0 Begin of source range.
    /// @param x1 End of source range.
    /// @param y0 Begin of target range.
    /// @param y1 End of source range.
    /// @return Projected value.
    static double projectValue(double x, double x0, double x1, double y0, double y1)
    {
        return (x - x0) * (y1 - y0) / (x1 - x0) + y0;
    }

    /// @brief Projects a value `x` from `[x0..x1]` range to `[y0..y1]` range.
    /// @param x Value to be projected.
    /// @param x0 Begin of source range.
    /// @param x1 End of source range.
    /// @param y0 Begin of target range.
    /// @param y1 End of source range.
    /// @return Projected value.
    static float projectValue(float x, float x0, float x1, float y0, float y1)
    {
        return (x - x0) * (y1 - y0) / (x1 - x0) + y0;
    }

    /// @brief Result of interpolation containing the value and range indicator.
    /// @tparam T The value type.
    template<typename T>
    struct InterpolationResult
    {
        T value;        // The interpolated or boundary value.
        char range;     // Range indicator: '<', '>', '=', or 0.
    };

    /// @brief Performs linear interpolation on an array of elements with `x` and `y` properties.
    /// @note  The elements must be sorted ascending by the `x` property.
    /// @tparam TElement The element type.
    /// @tparam N The array size.
    /// @param array The array reference.
    /// @param getX Function to get `x` value from an element.
    /// @param getY Function to get `y` value from an element.
    /// @param x The input `x` value to interpolate for.
    /// @return `InterpolationResult` containing the value and range indicator.
    /// @remark If x is outside the array's range, returns the first or last y value.
    template<typename TElement, size_t N>
    static auto interpolate(
        const TElement (&array)[N],
        auto getX,
        auto getY,
        decltype(getX(array[0])) x) -> InterpolationResult<decltype(getY(array[0]))>
    {
        using ValueType = decltype(getY(array[0]));
        InterpolationResult<ValueType> result{};

        if (N == 0) return result; // Empty array case

        float dy = getY(array[N - 1]) - getY(array[0]);
        char leftRange = dy == 0.0f ? 0 : dy > 0 ? '<' : '>';
        char rightRange = dy == 0.0f ? 0 : dy > 0 ? '>' : '<';

        // Check if x is before first element
        if (x < getX(array[0]))
        {
            result.value = getY(array[0]);
            result.range = leftRange;
            return result;
        }

        // Check if x is exactly the first element
        if (x == getX(array[0]))
        {
            result.value = getY(array[0]);
            result.range = '=';
            return result;
        }

        // Check if x is after last element
        if (x > getX(array[N - 1]))
        {
            result.value = getY(array[N - 1]);
            result.range = rightRange;
            return result;
        }

        // Check if x is exactly the last element
        if (x == getX(array[N - 1]))
        {
            result.value = getY(array[N - 1]);
            result.range = '=';
            return result;
        }

        // Find the interval where x lies
        for (size_t i = 1; i < N; ++i)
        {
            if (x <= getX(array[i]))
            {
                if (x == getX(array[i]))
                {
                    result.value = getY(array[i]);
                    result.range = '=';
                }
                else
                {
                    result.value = static_cast<ValueType>(projectValue(
                        x,
                        getX(array[i - 1]),
                        getX(array[i]),
                        getY(array[i - 1]),
                        getY(array[i])
                    ));
                    result.range = 0; // Actual interpolation
                }
                return result;
            }
        }

        // Should never reach here if array is properly ordered
        result.value = getY(array[N - 1]);
        result.range = '>';
        return result;
    }

};
