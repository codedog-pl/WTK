/**
 * @file        ILogOutput.hpp
 * @author      Adam Łyskawa
 *
 * @brief       System logger interface for message output.
 * @remark      A part of the Woof Toolkit (WTK).
 *
 * @copyright   (c)2025 CodeDog, All rights reserved.
 */

#pragma once

/// @brief Defines a debug output interface.
/// @remark Implementation should define how to physically send the debug text output.
class ILogOutput
{

public:

    /// @returns true if the output is currently available.
    virtual bool isAvailable(void) { return true; }

    /// @brief Starts asynchronous operation as soon as the RTOS is started.
    /// @remarks If not defined in derived class, it does nothing.
    virtual void startAsync(void) { }

    /// @brief Notifies the output that there's a new message to send.
    virtual void send() = 0;

};