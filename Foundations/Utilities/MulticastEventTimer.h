#ifndef __MULTICASTEVENTTIMER_H__
#define __MULTICASTEVENTTIMER_H__

#include <queue>
#include "MulticastEvent.h"

template<typename TPayload>
class MulticastEventTimer {
private:
	struct EventSpec {
		uint32 Delay;
		TPayload Payload;
	};

private: 
	std::queue<EventSpec> m_xPayloads;

	uint32 m_lTotalDuration;
	uint32 m_lElapsedTime;

	uint32 m_lLastTriggeredTime;

public:
	MulticastEventTimer() 
		: m_lTotalDuration(0)
		, m_lElapsedTime(0)
		, m_lLastTriggeredTime(0) {
	};

	void Enqueue(uint16 delay, const TPayload& payload) {
		m_lTotalDuration += delay;

		EventSpec spec;
		spec.Delay = delay;
		spec.Payload = payload;
		m_xPayloads.push(spec);
	}

	void Clear() {
		bool popped = false;
		while (!m_xPayloads.empty()) {
			m_xPayloads.pop();
			popped = true;
		}
		if (popped) {
			LastEventFired.Invoke(*this, 0);
		}
	}
	
	uint32 GetElapsedTime() const {
		return m_lElapsedTime;
	}

	uint32 GetTotalDuration() const {
		return m_lTotalDuration;
	}

	void Update(uint16 timestep) {
		m_lElapsedTime += timestep;

		while (!m_xPayloads.empty()) {
			const EventSpec& spec = m_xPayloads.front();
			if (m_lElapsedTime - m_lLastTriggeredTime < spec.Delay) {
				break;
			}

			m_lLastTriggeredTime += spec.Delay;
			Elapsed.Invoke(*this, spec.Payload);

			m_xPayloads.pop();

			if (m_xPayloads.empty()) {
				LastEventFired.Invoke(*this, 0);
			}
		}
	}

public:
	MulticastEvent<MulticastEventTimer, TPayload> Elapsed;
	MulticastEvent<MulticastEventTimer, int /* dummy */> LastEventFired;
};

#endif
