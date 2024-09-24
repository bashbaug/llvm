/*
// Copyright (c) 2023 Ben Ashbaugh
//
// SPDX-License-Identifier: MIT
*/

#pragma once

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

namespace sycl {
inline namespace _V1 {
namespace detail {

static constexpr char delimiter = '$';

#include <chrono>
class TimerChrono {
public:
    using clock = std::chrono::steady_clock;

    TimerChrono() {}
    inline uint64_t ticks() {
        return std::chrono::time_point_cast<std::chrono::nanoseconds>(clock::now())
            .time_since_epoch()
            .count();
    }

    inline double ticks_to_usf(uint64_t tick_delta) {
        return (double)tick_delta / 1000.0;
    }
    inline double ticks_to_nsf(uint64_t tick_delta) {
        return (double)tick_delta;
    }
    inline uint64_t ticks_to_us(uint64_t tick_delta) {
        return tick_delta / 1000;
    }
    inline uint64_t ticks_to_ns(uint64_t tick_delta) {
        return tick_delta;
    }
};

#if defined(_WIN32) || defined(_WIN64)

#include "windows.h"
#include <intrin.h>
class TimerWindows {
public:
    TimerWindows() {
        LARGE_INTEGER freq;
        QueryPerformanceFrequency(&freq);
        m_frequency = freq.QuadPart;
    }

    inline uint64_t ticks() {
        LARGE_INTEGER qpcnt;
        int rval = QueryPerformanceCounter(&qpcnt);
        return qpcnt.QuadPart;
    }

    inline double ticks_to_usf(uint64_t tick_delta) {
        return (double)tick_delta * 1000000 / m_frequency;
    }
    inline double ticks_to_nsf(uint64_t tick_delta) {
        return (double)tick_delta * 1000000000 / m_frequency;
    }
    inline uint64_t ticks_to_us(uint64_t tick_delta) {
        return tick_delta * 1000000 / m_frequency;
    }
    inline uint64_t ticks_to_ns(uint64_t tick_delta) {
        return tick_delta * 1000000000 / m_frequency;
    }

private:
    uint64_t m_frequency;
};

using Timer = TimerWindows;

#elif defined(__linux__)

#include <sched.h>
// https://stackoverflow.com/questions/42189976/calculate-system-time-using-rdtsc
// Discussion describes how clock_gettime() costs about 4 ns per call
class TimerLinux {
public:
    TimerLinux() {}

    inline uint64_t ticks() {
        struct timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        return (static_cast<uint64_t>(1000000000UL) *
                static_cast<uint64_t>(ts.tv_sec) +
                static_cast<uint64_t>(ts.tv_nsec));
    }

    inline double ticks_to_usf(uint64_t tick_delta) {
        return (double)tick_delta / 1000.0;
    }
    inline double ticks_to_nsf(uint64_t tick_delta) {
        return (double)tick_delta;
    }
    inline uint64_t ticks_to_us(uint64_t tick_delta) {
        return tick_delta / 1000;
    }
    inline uint64_t ticks_to_ns(uint64_t tick_delta) {
        return tick_delta;
    }
};

using Timer = TimerLinux;

#else

using Timer = TimerChrono;

#endif

inline Timer& getTimer()
{
    static Timer timer;
    return timer;
}

class StatsAggregator
{
public:
    StatsAggregator() = default;
    ~StatsAggregator()
    {
        //std::lock_guard<std::mutex> lock(m_Mutex);
        auto& os = std::cerr;

        if( !m_HostTimingStatsMap.empty() )
        {
            uint64_t    totalTotalNS = 0;
            size_t      longestName = 32;

            os << std::endl << "Call Profiling Results:" << std::endl;

            // Move data from the unordered map to an ordered map for better reporting.
            typedef std::map<std::string, SHostTimingStats>COrderedHostTimingStatsMap;
            COrderedHostTimingStatsMap stats;

            for (const auto& i : m_HostTimingStatsMap)
            {
                const std::string& name = tidyup(i.first);
                const SHostTimingStats& hostTimingStats = i.second;

                if( !name.empty() )
                {
                    totalTotalNS += hostTimingStats.TotalNS;
                    longestName = std::max< size_t >( name.length(), longestName );

                    stats[name] = hostTimingStats;
                }
            }

            os << std::endl << "Total Time (ns): " << totalTotalNS << std::endl;

            os << std::endl
                << std::left << std::setw(longestName) << "Function Name" << delimiter << ' '
                << std::right << std::setw( 8) << "NumCalls" << delimiter << ' '
                << std::right << std::setw( 9) << "FiltCalls" << delimiter << ' '
                << std::right << std::setw(13) << "Time (ns)" << delimiter << ' '
                //<< std::right << std::setw( 8) << "Time (%)" << delimiter << ' '
                << std::right << std::setw(13) << "Average (ns)" << delimiter << ' '
                << std::right << std::setw(13) << "Min (ns)" << delimiter << ' '
                << std::right << std::setw(13) << "Max (ns)" << std::endl;

            // Now report the data from the ordered map.
            for (const auto& i : stats)
            {
                const std::string name = i.first;
                const SHostTimingStats& hostTimingStats = i.second;

                os << std::left << std::setw(longestName) << name << delimiter << ' '
                    << std::right << std::setw( 8) << hostTimingStats.NumberOfUnfilteredCalls << delimiter << ' '
                    << std::right << std::setw( 9) << hostTimingStats.NumberOfCalls << delimiter << ' '
                    << std::right << std::setw(13) << hostTimingStats.TotalNS << delimiter << ' '
                    //<< std::right << std::setw( 7) << std::fixed << std::setprecision(2) << hostTimingStats.TotalNS * 100.0f / totalTotalNS << "% << delimiter << ' '
                    << std::right << std::setw(13) << hostTimingStats.TotalNS / (hostTimingStats.NumberOfCalls ? hostTimingStats.NumberOfCalls : 1) << delimiter << ' '
                    << std::right << std::setw(13) << (hostTimingStats.NumberOfCalls ? hostTimingStats.MinNS : 0) << delimiter << ' '
                    << std::right << std::setw(13) << hostTimingStats.MaxNS << std::endl;
            }
        }
    }

    void addRecord(const char* label, uint32_t index, uint64_t delta)
    {
        //std::lock_guard<std::mutex> lock(m_Mutex);
        SHostTimingStats&   stats = m_HostTimingStatsMap[label];

        stats.NumberOfUnfilteredCalls++;
        if (delta < 1000000) {     // 1ms
            stats.NumberOfCalls++;
            stats.TotalNS += delta;
            stats.MinNS = std::min<uint64_t>(stats.MinNS, delta);
            stats.MaxNS = std::max<uint64_t>(stats.MaxNS, delta);
        }
    }

private:
    //std::mutex m_Mutex;
    std::string tidyup(const std::string& str)
    {
        std::string ret = str;

#if 0
        const std::string remove = "sycl::_V1::";
        size_t pos = 0;;
        while ((pos = ret.find(remove, pos)) != std::string::npos) {
            ret.erase(pos, remove.length());
        }
#else
        size_t pos = ret.find('(');
        if (pos != std::string::npos) {
            ret.erase(pos);
        }
#endif

        return ret;
    }

    struct SHostTimingStats
    {
        SHostTimingStats() :
            NumberOfUnfilteredCalls(0),
            NumberOfCalls(0),
            MinNS(UINT64_MAX),
            MaxNS(0),
            TotalNS(0) {}

        uint64_t    NumberOfUnfilteredCalls;
        uint64_t    NumberOfCalls;
        uint64_t    MinNS;
        uint64_t    MaxNS;
        uint64_t    TotalNS;
    };

#if defined(USE_STRING_KEYS)
    typedef std::unordered_map<std::string, SHostTimingStats>   CHostTimingStatsMap;
#else
    typedef std::unordered_map<const char*, SHostTimingStats>   CHostTimingStatsMap;
#endif // defined(USE_STRING_KEYS)
    CHostTimingStatsMap  m_HostTimingStatsMap;
};

inline StatsAggregator& getAggregator()
{
    static StatsAggregator aggregator;
    return aggregator;
}

class ScopeProfilerNull
{
public:
    ScopeProfilerNull(const char* label) {}
    ~ScopeProfilerNull() {}
    void close() {}
};

class ScopeProfilerReal
{
public:
    ScopeProfilerReal(const char* label, uint32_t index = 0) :
        m_Label(label), m_Index(index), m_StartTicks(getTimer().ticks()), m_Closed(false)
    {
        //std::cerr << ">>> Opened record: " << m_Label << "\n";
    }
    ~ScopeProfilerReal()
    {
        close();
    }

    void close()
    {
        if (!m_Closed) {
            Timer& timer = getTimer();
            uint64_t tick_delta = timer.ticks() - m_StartTicks;
            uint64_t ns_delta = timer.ticks_to_ns(tick_delta);

            getAggregator().addRecord(m_Label, m_Index, ns_delta);
            m_Closed = true;
            //std::cerr << "<<< Closed record: " << m_Label << ", delta: " << ns_delta << "\n";
        }
    }

private:
    const char* m_Label;
    uint32_t m_Index;
    uint64_t m_StartTicks;
    bool m_Closed;
};

using ScopeProfiler = ScopeProfilerReal;

} // namespace detail
} // namespace _V1
} // namespace sycl
