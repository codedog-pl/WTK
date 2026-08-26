/**
 * @file        fx_time.h
 * @author      Adam Łyskawa
 *
 * @brief       FILEX time synchronization header.
 * @remark      A part of the Woof Toolkit (WTK).
 *
 * @copyright   (c)2026 CodeDog, All rights reserved.
 *
 *                / \__
 *               (    @\___
 *               /         O
 *              /   (_____/
 *              /_____/   U
 */

#pragma once

#include "bindings.h"
#include "fx_api.h"

EXTERN_C_BEGIN

/// @brief Starts updating the file system time with the RTC.
void fx_start_fs_time_update(void);

EXTERN_C_END