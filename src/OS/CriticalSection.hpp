/**
 * @file        CriticalSection.hpp
 * @author      Adam Łyskawa
 *
 * @brief       A minimal critical section usable identically from thread and ISR context. Header only.
 * @remark      A part of the Woof Toolkit (WTK), RTOS API.
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

#include "hal_mcu.h"

namespace OS
{

/// @brief A minimal, RTOS-independent critical section based on masking interrupts (PRIMASK).
///        Unlike `Mutex`, it works identically when entered from a thread or from an ISR,
///        so it's the only primitive that may guard state touched from both contexts.
///        Keep the protected region to a handful of plain field reads/writes -
///        never call blocking or RTOS API while it's held.
class CriticalSection final
{

public:

    /// @brief Disables interrupts, returning the previous PRIMASK state to be passed to `exit`.
    static inline uint32_t enter()
    {
        uint32_t primask = __get_PRIMASK();
        __disable_irq();
        return primask;
    }

    /// @brief Restores the interrupt state saved by a matching `enter` call.
    /// @param primask The value returned by `enter`.
    static inline void exit(uint32_t primask)
    {
        if (!primask) __enable_irq();
    }

    /// @brief RAII scoped critical section. Nestable: safe to construct while already inside one.
    class Scope final
    {

    public:

        inline Scope() : m_primask(CriticalSection::enter()) { }
        inline ~Scope() { CriticalSection::exit(m_primask); }
        Scope(const Scope&) = delete;
        Scope& operator=(const Scope&) = delete;

    private:

        uint32_t m_primask;

    };

};

}
