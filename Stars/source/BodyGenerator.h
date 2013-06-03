#ifndef __BODYGENERATOR_H__
#define __BODYGENERATOR_H__

#include <queue>
#include "Renderable.h"
#include "GameFoundation.h"
#include "EventTimer.h"

class BodyGenerator {
private: 
	GameFoundation& m_rxGameFoundation;
	float m_fXPos;

	struct BodySpec {
		std::string Body;
		float YPos;
	};
	EventTimer<BodySpec> m_xTimer;

public:
	BodyGenerator(GameFoundation& game);
	virtual ~BodyGenerator();

	void SetPosition(float xpos);
	void EnqueueBody(const std::string& body, float position, uint16 delay);
	float GetCompletionDegree();

	void Update(uint16 timestep);

private:
	void TimerEventHandler(const EventTimer<BodySpec>& sender, const BodySpec& args);
};

#endif
