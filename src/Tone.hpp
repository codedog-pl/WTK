/**
 * @file        Tone.hpp
 * @author      Adam Łyskawa
 *
 * @brief       A simple tone generator for any sample type that can be set from a `double` normalized value.
 * @remark      A part of the Woof Toolkit (WTK).
 *
 * @copyright   (c)2025 CodeDog, All rights reserved.
 */

#pragma once

#include <cstdint>
#include <cmath>

/// @brief Represents a single period of an audio tone.
/// @tparam TSample Sample type.
/// @tparam TRate Sample rate.
/// @tparam TFrequency Tone frequency in Hz.
template<typename TSample, uint32_t TRate, uint32_t TFrequency>
class Tone final
{

public:

    static constexpr size_t length = TRate / TFrequency; // Samples buffer length (in samples).

    /// @brief Waveform shape.
    enum Waveform { Sine, Square };

public:

    /// @brief Creates a tone.
    /// @param level Sound volume level in dB where 0 is full volume. Default 0.
    /// @param waveform Waveform type. Default `Sine`.
    Tone(double level = 0, Waveform waveform = Sine) : m_waveform(waveform), m_level(level), m_samples()
    {
        generate(level);
    }

    /// @brief Copies a tone.
    /// @param other A tone to copy.
    Tone(const Tone& other) : m_waveform(other.m_waveform), m_level(other.m_level), m_samples()
    {
        for (size_t i = 0; i < length; i++) m_samples[i] = other.m_samples[i];
    }

    /// @brief Not moveable.
    Tone(Tone&&) = delete;

    /// @brief Sets the sample volume in logarithmic slider percentage.
    /// @param level Positive integer values from 0 to 100.
    /// @param range Regulation range in decibells.
    void setVolume(uint8_t level, uint8_t range = 60)
    {
        double normalizedX = level > 0 ? level * 0.01 : 0.00001;
        if (normalizedX > 1.0) normalizedX = 1.0;
        if (range < 20) range = 20;
        if (range > 120) range = 120;
        double dBLevel = 0.025 * 20.0 * static_cast<double>(range) * log10(normalizedX);
        generate(m_level + dBLevel);
    }

    /// @brief Defines a copying assignment operator.
    /// @param other A tone to copy.
    /// @return This tone reference.
    Tone& operator=(const Tone& other) noexcept
    {
        if (&other == this) return *this;
        for (size_t i = 0; i < length; i++) m_samples[i] = other.m_samples[i];
        return *this;
    }

    /// @brief Returns the data buffer pointer.
    template<typename T = uint8_t*>
    const T data() { return reinterpret_cast<T>(m_samples); }

    /// @brief Returns the data buffer size in bytes.
    uint32_t size() { return static_cast<uint32_t>(length * sizeof(TSample)); }

    /// @brief Returns a specific sample reference.
    TSample& operator[](uint32_t i) { return m_samples[i % length]; }

private:

    void generate(double level = 0)
    {
        double normalized = 0;
        int32_t n = length;
        for (int32_t i = 0; i < n; ++i)
        {
            switch (m_waveform)
            {
            case Sine:
                normalized = sin(_D_PI * i / static_cast<double>(n));
                break;
            case Square:
                normalized = 1 - (((i << 1) / n) << 1);
                break;
            }
            if (level < 0) normalized *= pow(10.0, 0.05 * level);
            m_samples[i] = normalized;
        }
    }

    Waveform m_waveform;
    double m_level;
    TSample m_samples[length];                              // Samples buffer.
    static constexpr double _PI = 3.14159265358979323846;   // π
    static constexpr double _D_PI = 2.0 * _PI;              // 2π

};
