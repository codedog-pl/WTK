/**
 * @file        fx_time.h
 * @author      Adam Łyskawa
 * @brief       FILEX time synchronization header.
 *
 * @copyright   (c)2024 CodeDog, All rights reserved.
 */

#pragma once

#include "bindings.h"
#include "fx_api.h"

EXTERN_C_BEGIN

/// @brief Starts updating the file system time with the RTC.
void fx_start_fs_time_update(void);

EXTERN_C_END