/**
 * @file        fx_time.c
 * @author      Adam Łyskawa
 *
 * @brief       FILEX time synchronization.
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

#include "fx_time.h"
#include "datetime.h"
#include "hal.h"
#include "log.h"

extern RTC_HandleTypeDef hrtc; // Defined in rtc.h.

/// @brief Called in intervals defined in RTC initialization, 1s update time is enough.
/// @param _hrtc RTC handle pointer.
static void fx_rtc_callback(RTC_HandleTypeDef* _hrtc)
{
    DateTimeTypeDef dt = {};
    if (RTC_GetDateTime(&dt) == HAL_OK)
    {
        fx_system_date_set(dt.date.y, dt.date.m, dt.date.d);
        fx_system_time_set(dt.time.h, dt.time.m, dt.time.s);
    }
}

void fx_start_fs_time_update(void)
{
    HAL_RTC_RegisterCallback(&hrtc, HAL_RTC_WAKEUPTIMER_EVENT_CB_ID, fx_rtc_callback);
}
