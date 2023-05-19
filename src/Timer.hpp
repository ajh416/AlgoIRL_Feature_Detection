#include <ctime>
#include <iostream>
#include <chrono>
#include <string>

class Timer {
public:
	Timer(const std::string& name = "Timer") {
		m_Name = name;
		m_StartPoint = std::chrono::high_resolution_clock::now();
	}

	~Timer() {
		if (!m_Stopped)
			Stop();
	}

	void Stop() {
		auto end_timepoint = std::chrono::high_resolution_clock::now();
		uint64_t start = std::chrono::time_point_cast<std::chrono::milliseconds>(m_StartPoint).time_since_epoch().count();
		uint64_t end = std::chrono::time_point_cast<std::chrono::milliseconds>(end_timepoint).time_since_epoch().count();
		std::cout << "Timer " << m_Name << " finished in " << (end - start) << "ms\n";
		m_Stopped = true;
	}

private:
	std::string m_Name;
	std::chrono::time_point<std::chrono::high_resolution_clock> m_StartPoint;
	bool m_Stopped = false;
};
