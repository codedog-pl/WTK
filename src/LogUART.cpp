/**
 * @file        LogUART.cpp
 * @author      Adam Łyskawa
 *
 * @brief       HAL UART port debug output implementation.
 * @remark      A part of the Woof Toolkit (WTK).
 *
 * @copyright	(c)2025 CodeDog, All rights reserved.
 */

#include "LogUART.hpp"

LogUART::LogUART(UART_HandleTypeDef* huart, ILogMessagePool& pool) : m_uart(huart), m_pool(pool)
{
    HAL_UART_RegisterCallback(m_uart, HAL_UART_TX_COMPLETE_CB_ID, tx_complete);
    sendNext(); // In case if the pool already contains unsent messages.
}

LogUART::~LogUART()
{
    HAL_UART_UnRegisterCallback(m_uart, HAL_UART_TX_COMPLETE_CB_ID);
    m_uart = nullptr;
}

LogUART *LogUART::getInstance(UART_HandleTypeDef *huart, ILogMessagePool &pool)
{
    static LogUART instance(huart, pool);
    return m_instance = &instance;
}

void LogUART::send()
{
    if (m_isSending || !m_uart) return;
    auto msg = m_pool.find(LogMessage::State::qd);
    if (!msg) return;
    m_pool.send(msg);
    m_isSending = true;
    auto [buffer, length] = msg->buffer();
    HAL_UART_Transmit_DMA(m_uart, buffer, length);
}

void LogUART::tx_complete(UART_HandleTypeDef* huart)
{
    auto msg = m_instance->m_pool.findByBuffer(huart->pTxBuffPtr);
    if (!msg) return;
    msg->clear();
    m_instance->m_pool.toss(msg);
    m_instance->m_isSending = false;
    m_instance->send();
}
