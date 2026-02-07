#pragma once

#include <chrono>
#include <cstdint>
#include <functional>
#include <string>
#include <utility>
#include <source_location>

#if defined(_MSC_VER)
    #include <intrin.h>
    #pragma intrinsic(__rdtsc)
#elif defined(__GNUC__) || defined(__clang__)
    #include <x86intrin.h>
#endif


namespace ng::debug
{
	namespace impl
	{
		template <typename Clock>
		concept IsClock = requires (Clock c)
		{
			typename Clock::time_point;
			Clock::now();
		};
	}

	struct proc_timestamp_clock
	{
		using rep = std::uint64_t;
		using period = std::ratio<1, 1>; // Cycles, not seconds
		using duration = std::chrono::duration<rep, period>;
		using time_point = std::chrono::time_point<proc_timestamp_clock>;
		static constexpr bool is_steady = true;

		// Helper to read TSC safely with compiler barriers
		static rep rdtsc() noexcept
		{
			// Compiler barrier to prevent reordering around rdtsc
			#if defined(_MSC_VER)
				_ReadWriteBarrier();
				return _rdtsc();
			#elif defined(__GNUC__) || defined(__clang__)
				return __builtin_ia32_rdtsc();
			#else
				std::uint32_t lo, hi;
				asm volatile ("rdtsc" : "=a" (lo), "=d" (hi));
				return (static_cast<std::uint64_t>(hi) << 32) | lo;
			#endif
		}

		static time_point now() noexcept
		{
			return time_point{duration{rdtsc()}};
		}

		inline static struct calibration_data
		{
			double cycles_per_ns;
			std::uint64_t tsc_start;
			std::chrono::high_resolution_clock::time_point steady_start;

			calibration_data()
			{
				*this = calibrate();
			}

		private:
			inline static struct empty_tag_t {} empty_tag;
			calibration_data(empty_tag_t) {}

			friend class proc_timestamp_clock;
		} m_calibrationData;

		static calibration_data calibrate(std::chrono::nanoseconds duration = std::chrono::nanoseconds(50 * 1000 * 1000))
		{
			calibration_data result{ calibration_data::empty_tag };
			result.tsc_start = rdtsc();
			result.steady_start = std::chrono::high_resolution_clock::now();

			// Wait for specified duration
			auto target = result.steady_start + duration;
			while (std::chrono::high_resolution_clock::now() <= target)
			{
				// Spin-wait
				#if defined(__GNUC__) || defined(__clang__)
					__builtin_ia32_pause();
				#endif
			}

			std::uint64_t tsc_end = rdtsc();
			auto steady_end = std::chrono::high_resolution_clock::now();

			auto elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(
				steady_end - result.steady_start
			).count();
			std::uint64_t elapsed_cycles = tsc_end - result.tsc_start;

			result.cycles_per_ns = static_cast<double>(elapsed_cycles) / elapsed_ns;
			return result;
		}

		// Convert TSC duration to nanoseconds using calibration
		template<typename Duration>
		static std::chrono::nanoseconds to_nanoseconds(
			Duration d, 
			double cycles_per_ns = m_calibrationData.cycles_per_ns
		) {
			auto cycles = std::chrono::duration_cast<duration>(d).count();
			return std::chrono::nanoseconds{
				static_cast<std::int64_t>(cycles / cycles_per_ns)
			};
		}
	};

	using DefaultClock = proc_timestamp_clock;
	// using DefaultClock = std::chrono::high_resolution_clock;

	template <impl::IsClock Clock = DefaultClock>
	class Profiler;
	template <impl::IsClock Clock = DefaultClock>
	static void defaultProfilerCallback(const Profiler<Clock>& profiler) noexcept;

	/**
	 * @brief RAII timer that reports elapsed nanoseconds when stopped.
	 */
	template <impl::IsClock Clock>
	class Timer
	{
	public:
		using clock = Clock;
		using time_point = clock::time_point;

		explicit Timer(std::uint64_t* elapsed) noexcept
			: m_elapsed(elapsed)
		{
			if constexpr (std::is_same_v<clock, proc_timestamp_clock>)
				proc_timestamp_clock::m_calibrationData = proc_timestamp_clock::calibrate();

			m_start = clock::now();
		}
		~Timer() noexcept { stop(); }

		Timer(const Timer&) = delete;
		Timer& operator=(const Timer&) = delete;
		Timer(Timer&&) = delete;
		Timer& operator=(Timer&&) = delete;

		std::uint64_t stop() noexcept
		{
			if (!m_stopped)
			{
				const auto now = clock::now();

				if constexpr (std::is_same_v<clock, proc_timestamp_clock>)
					// *m_elapsed = proc_timestamp_clock::to_nanoseconds(now - m_start).count();
					*m_elapsed = proc_timestamp_clock::to_nanoseconds(now - m_start, 1).count();
				else
					*m_elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(now - m_start).count();

				m_stopped = true;
			}

			return *m_elapsed;
		}

		void reset() noexcept
		{
			m_start = clock::now();
			m_elapsed = 0;
			m_stopped = false;
		}

		[[nodiscard]] std::uint64_t elapsed() const noexcept { return *m_elapsed; }
		[[nodiscard]] bool running() const noexcept { return !m_stopped; }

	private:
		time_point m_start{};
		std::uint64_t* m_elapsed;
		bool m_stopped = false;
	};

	/**
	 * @brief RAII profiler that calls a callback with
	 * info about execution time of a scope.
	 */
	template <impl::IsClock Clock>
	class Profiler
	{
	public:
		using Callback = std::function<void(const Profiler<Clock>&)>;

		Profiler(
			std::string name,
			Callback callback = &defaultProfilerCallback<Clock>,
			const std::source_location& srcloc = std::source_location::current()
		) noexcept
				: m_name(std::move(name)),
				m_callback(std::move(callback)),
				m_function_name(srcloc.function_name()),
				m_elapsed(0), m_timer(&m_elapsed)
		{}

		~Profiler() noexcept { finalize(); }

		Profiler(const Profiler&) = delete;
		Profiler& operator=(const Profiler&) = delete;
		Profiler(Profiler&&) = delete;
		Profiler& operator=(Profiler&&) = delete;

		void finalize() noexcept
		{
			if (m_invoked)
				return;

			m_timer.stop();
			m_invoked = true;

			if (m_callback)
				m_callback(*this);
		}

		[[nodiscard]] const std::string& name() const noexcept { return m_name; }
		[[nodiscard]] const std::string& functionName() const noexcept { return m_function_name; }
		[[nodiscard]] std::uint64_t elapsed() const noexcept { return m_timer.elapsed(); }

	private:
		std::string m_name;
		std::string m_function_name;
		Callback m_callback;
		std::uint64_t m_elapsed = 0;
		Timer<Clock> m_timer;
		bool m_invoked = false;
	};


	template <impl::IsClock Clock> requires(std::is_same_v<Clock, proc_timestamp_clock>)
	static void defaultProfilerCallback(const Profiler<Clock>& profiler) noexcept
	{
		geode::log::debug("[PROFILER: {}] {} took {} cycles",
			profiler.functionName(), profiler.name(), profiler.elapsed()
		);
	}

	template <impl::IsClock Clock>
	static void defaultProfilerCallback(const Profiler<Clock>& profiler) noexcept
	{
		geode::log::debug("[PROFILER: {}] {} took {} ns",
			profiler.functionName(), profiler.name(), profiler.elapsed()
		);
	}
}

#ifdef NID_DEBUG_BUILD
	// internal
	#define _iNID_PROFILER_MACRO_CAT_IMPL(lhs, rhs)      lhs##rhs
	#define _iNID_PROFILER_MACRO_CAT(lhs, rhs)           _iNID_PROFILER_MACRO_CAT_IMPL(lhs, rhs)
	#define _iNID_PROFILER_MACRO_MAKER_ARGS(...)         { __VA_ARGS__ }
	#define _iNID_PROFILER_MACRO_MAKER_LINE(clock, line) ng::debug::Profiler<clock> _iNID_PROFILER_MACRO_CAT(profiler, line)
	#define _iNID_PROFILER_MACRO_MAKER_VAR(line)         _iNID_PROFILER_MACRO_MAKER_LINE(ng::debug::DefaultClock, line)

	#define NID_PROFILER(...) _iNID_PROFILER_MACRO_MAKER_VAR(__LINE__) _iNID_PROFILER_MACRO_MAKER_ARGS(__VA_ARGS__)
	#define NID_PROFILER_CCLOCK(clock) _iNID_PROFILER_MACRO_MAKER_LINE(clock, __LINE__) _iNID_PROFILER_MACRO_MAKER_ARGS
#else
	#define NID_PROFILER(name, callback) do {} while (0)
	#define NID_PROFILER_CCLOCK(clock) do {} while (0)
#endif
