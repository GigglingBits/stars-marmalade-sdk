#include "BodyGenerator.h"
#include "FactoryManager.h"
#include "Debug.h"

BodyGenerator::BodyGenerator(GameFoundation& game) 
	: m_fXPos(0.0f), m_rxGameFoundation(game) {
	m_xTimer.Elapsed.AddListener(this, &BodyGenerator::TimerEventHandler);
}

BodyGenerator::~BodyGenerator() {
	m_xTimer.Elapsed.RemoveListener(this, &BodyGenerator::TimerEventHandler);
}

void BodyGenerator::SetPosition(float ypos) {
	m_fXPos = ypos;
}

void BodyGenerator::EnqueueBody(const std::string& body, float position, uint16 delay) {
	IW_CALLSTACK_SELF;

	BodySpec spec;
	spec.Body = body;
	spec.YPos = position;

	m_xTimer.Enqueue(delay, spec);
}

float BodyGenerator::GetCompletionDegree() {
	uint32 total = m_xTimer.GetTotalDuration();
	return total == 0 ? 1.0f : (float)m_xTimer.GetElapsedTime() / (float)total;
}

void BodyGenerator::Update(uint16 timestep) {
	m_xTimer.Update(timestep);
}

void BodyGenerator::TimerEventHandler(const EventTimer<BodySpec>& sender, const BodySpec& args) {
	IW_CALLSTACK_SELF;

	BodyFactory& factory = FactoryManager::GetBodyFactory();		
	if (Body* body = factory.Create(args.Body)) {
		body->SetPosition(CIwFVec2(m_fXPos, args.YPos));
		body->SetSpeed(CIwFVec2(-3.0f, 0.0f));
		m_rxGameFoundation.Add(body);
	} else {
		IwAssertMsg(MYAPP, body, ("Failed to create new body with name '%s'", args.Body.c_str()));
	}
}
