/**
 * @file        HardcodedSequence.hpp
 * @author      Adam Łyskawa
 *
 * @brief       A set of macros to use hardcoded sequences.
 * @remark      A part of the Woof Toolkit (WTK).
 *
 * @copyright	(c)2026 CodeDog, All rights reserved.
 */

#pragma once

/// @brief Defines the counter base for generating subsequent numbers.
/// @param base A unique indentifier related to the sequence.
#define HARDCODED_SEQUENCE(base) enum { base = __COUNTER__ + 1U };

/// @brief Generates the next number in the hardcoded sequence.
/// @param base A unique indentifier related to the sequence.
#define SEQ(base) (__COUNTER__ - base)
