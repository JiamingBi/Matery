#pragma once

#include "arena.hpp"
#include "chunk.hpp"
#include "gc_graph.hpp"
#include "gcptr.hpp"
#include "heuristics.hpp"
#include "memory_allocator.hpp"
#include "memory_manager.hpp"
#include "profiler_definitions.hpp"

#if MPP_STATS == 1
#include "mpplib/utils/statistics.hpp"
#include "mpplib/utils/timer.hpp"
#endif

#if MPP_DEBUG == 1
#include "mpplib/utils/options.hpp"
#include <fstream>
#endif

#include <cstring>
#include <memory>
#include <vector>

namespace mpp {
    /**
     * @brief GarbageCollector class.
     */
    class GC : public MemoryManager
    {
    protected:
        /**
         * @brief All active GcPtr's.
         *
         * All GcPtr's that are currently used in program adds to this list, to keep track
         * of them.
         */
        std::vector<GcPtr*> s_activeGcPtrs;

        /**
         * @brief size of garbage, cleaned last time.
         */
        std::size_t s_garbageSize{ 0 };

        /**
         * @brief Required size of memory block, to save all user data in.
         */
        std::size_t s_dataSize{ 0 };

#if MPP_STATS == 1
        /**
         * @brief Used to keep track of garbage collector stats
         */
        std::unique_ptr<utils::Statistics::GcStats> m_GcStats;
#endif

#if MPP_DEBUG == 1
        /**
         * @brief Current cycle to dump objects graph
         */
        static uint32_t m_currentCycle;
#endif

    public:
        /**
         * @brief Construct a new GC object
         */
        GC();

        /**
         * @brief Collect garbage.
         *
         * This method will construct graph of all in use chunks.
         * Then it will create Heuristics object to relayout data in most efficient way.
         * After that, it will move all data to newly created arena, updating
         * corresponding gcptr's. And in the end it will destroy unused arenas.
         * @return true if everything is good, false - otherwise
         */
        bool Collect();

        /**
         * @brief Get reference to vector of currently active GcPtr's
         * @return std::vector<GcPtr*>& of currently used GcPtr's
         */
        std::vector<GcPtr*>& GetGcPtrs()
        {
            return s_activeGcPtrs;
        }

        /**
         * @brief Get the Instance of GC object
         * @return GC& Global Garbage Collector
         */
        static GC& GetInstance()
        {
            static GC gcInstance;
            return gcInstance;
        }
    };
}