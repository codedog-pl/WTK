/**
 * @file        Pool.hpp
 * @author      Adam Łyskawa
 *
 * @brief       Provides generic pre-allocated element pool API with RAII element wrapper. Header only.
 * @remark      A part of the Woof Toolkit (WTK).
 *
 * @remarks     Intended to use on embedded systems to avoid dynamic memory allocations.
 *              Pool sizes must be set at compile time.
 *
 * @copyright	(c)2025 CodeDog, All rights reserved.
 */

#pragma once

#include <functional>
#include "Array.hpp"

/// @brief Pool error codes enumeration.
enum class PoolErrorCode
{
    E_NONE,             // No error occurred.
    E_POOL_EXHAUSTED,   // Pool was exhausted. Too many items requested without putting back.
    E_INVALID_RETURN    // An invalid pointer was about to be returned to the pool.
};

/// @brief A function accepting a poll error code.
using PoolErrorHandler = std::function<void(PoolErrorCode)>;

/// @brief An interface for an element pool allowing taking items and then putting them back.
/// @tparam TElement Element type.
template<typename TElement>
class IPool
{
public:

    /// @returns A first available element from the pool or `nullptr` if the pool is exhausted.
    virtual TElement* take() = 0;

    /// @brief Returns the element to the pool.
    /// @param element Item reference.
    virtual void putBack(TElement& element) = 0;

    /// @returns The number of available items.
    virtual int available() = 0;

    /// @brief Resets the pool to the initial state.
    /// @warning Only for testing purposes, do not use when instances are used!
    virtual void reset() = 0;

    /// @brief Register an error handler that receives an error code.
    /// @param errorHandler A function accepting an error code.
    virtual void registerErrorHandler(PoolErrorHandler errorHandler) = 0;

};

/// @brief Generic element pool template.
/// @tparam TElement Item type.
/// @tparam TSize Number of items.
template<typename TElement, size_t TSize>
class Pool : public Array<TElement, TSize>, public IPool<TElement>
{

public:

    /// @returns A first available element from the pool or `nullptr` if the pool is exhausted.
    TElement* take() override
    {
        for (TElement& element : *this) if (isAvailable(element))
        {
            setAvailable(element, 0);
            return &element;
        }
        if (m_error) m_error(PoolErrorCode::E_POOL_EXHAUSTED);
        return nullptr;
    }

    /// @brief Returns the element to the pool.
    /// @param element Item pointer.
    void putBack(TElement& element) override
    {
        for (auto& i : *this) if (&element == &i)
        {
            setAvailable(element);
            return;
        }
        if (m_error) m_error(PoolErrorCode::E_INVALID_RETURN);
    }

    /// @returns The number of available items.
    int available() override
    {
        int count = 0;
        for (auto& element : *this) if (isAvailable(element)) ++count;
        return count;
    }

    /// @brief Resets the pool to the initial state.
    /// @warning Only for testing purposes, do not use when instances are used!
    void reset() override
    {
        for (auto& element : *this) setAvailable(element);
    }

    void registerErrorHandler(PoolErrorHandler errorHandler) override
    {
        m_error = errorHandler;
    }

    /// @param element Item reference.
    /// @returns True if the element is available. False otherwise.
    virtual bool isAvailable(TElement& element) const = 0;

    /// @brief Sets the element as available.
    /// @param element Item reference.
    /// @param value 1: Available (default), 0: Taken.
    virtual void setAvailable(TElement& element, bool value = 1) = 0;

protected:

    PoolErrorHandler m_error = 0; // Optional error handler function.

};

/// @brief RAII container for pool items. When object of this type goes out of scope, it's returned to the pool.
/// @tparam TElement Item type.
template<typename TElement>
class PoolItem
{
public:

    /// @brief Gets an element from the pool.
    /// @param pool Item pool reference.
    PoolItem(IPool<TElement>& pool) : m_pool(pool), m_instance(pool.take()) { }

    /// @brief This is move only type.
    PoolItem(const PoolItem&) = delete;

    /// @brief Moves a pool element.
    /// @param other Source pool element rvalue.
    PoolItem(PoolItem&& other) noexcept : m_pool(other.m_pool), m_instance(other.m_instance) { other.m_pool = nullptr; other.m_instance = nullptr; }

    /// @brief Returns the element to the pool.
    virtual ~PoolItem() { if (m_pool) m_pool->putBack(m_instance); }

    /// @returns The pool element pointer.
    TElement* ptr() const { return m_instance; }

    /// @returns The pool element reference.
    TElement& ref() const { return *m_instance; }

    /// @returns The pool element pointer.
    operator TElement* () const { return m_instance; }

    /// @returns The pool element reference.
    operator TElement& () const { return *m_instance; }

protected:

    IPool<TElement>& m_pool;   // Source pool reference.
    TElement* m_instance;      // Item pointer.

};
