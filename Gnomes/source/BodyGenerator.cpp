#include "BodyGenerator.h"
#include "FactoryManager.h"

BodyGenerator::BodyGenerator(GameFoundation& game) 
	: m_rxGameFoundation(game) {
	Reset();
}

void BodyGenerator::SetArea(const CIwFVec2& position, const CIwFVec2& size) {
	// convert to int16 for better speed;
	// multiply by 100 for increased precision (e.g. 1cm);
	// could alternatively also have used IW_FIXED, but calculations seemed a bit cumbersome
	m_xAreaTimes100.x = (int32)(position.x * 100.0f);
	m_xAreaTimes100.y = (int32)(position.y * 100.0f);
	m_xAreaTimes100.w = (int32)(size.x * 100.0f);
	m_xAreaTimes100.h = (int32)(size.y * 100.0f);
}

void BodyGenerator::SetTimeInterval(uint32 interval) {
	if (interval == 0) {
		Stop();
	} else {
		m_lInterval = interval;
	}
}

void BodyGenerator::Start() {
	IwAssertMsg(MYAPP, !m_xSpecs.empty(), 
		("Can not start generating bodies without specification for the generator."));
	m_bStarted = true;
}

void BodyGenerator::Stop() {
	m_lLastBodyGenenratedTime = 0;
	m_lInterval = 0;
	m_bStarted = false;
}

void BodyGenerator::Reset() {
	Stop();
	m_xSpecs.clear();
	m_iProbabilitySum = 0;
}

void BodyGenerator::AddBody(const std::string& name, uint probaility) {
	IwAssertMsg(MYAPP, probaility > 0, 
		("Probability value invalid. Must be greater than 0."));

	BodySpec spec;
	spec.name = name;
	spec.probability = probaility;
	m_xSpecs.push_back(spec);

	m_iProbabilitySum += spec.probability;
}

const BodyGenerator::BodySpec& BodyGenerator::GetRandomBody() {
	// generate random number within probability range
	uint rand = (uint)(std::rand() % m_iProbabilitySum) + 1;
	
	// select body spec
	uint cursor = 0;
	std::list<BodySpec>::iterator it;
	for (it = m_xSpecs.begin(); it != m_xSpecs.end(); ++it) {
		cursor += it->probability;
		if (rand <= cursor) {
			return *it;
		}
	}	
	IwAssertMsg(MYAPP, false, 
		("Could not find random body spec. Probably error in algorithm. Rand: %u, probsum: %u", rand, cursor));
	return *(++m_xSpecs.begin());
}

CIwFVec2 BodyGenerator::GetRandomPosition() {
	// pick a random point inside the generator area
	uint32 x = (rand() % m_xAreaTimes100.w) + m_xAreaTimes100.x;
	uint32 y = (rand() % m_xAreaTimes100.h) + m_xAreaTimes100.y;
	return CIwFVec2(x / 100.0f, y / 100.0f);
}

void BodyGenerator::Update(uint16 timestep) {
	if (!m_bStarted) {
		return;
	}

	m_lLastBodyGenenratedTime += timestep;
	if (m_lLastBodyGenenratedTime > m_lInterval) {
		m_lLastBodyGenenratedTime -= m_lInterval;

		BodyFactory& factory = FactoryManager::GetBodyFactory();		
		std::string name = GetRandomBody().name;
		if (Body* body = factory.Create(name)) {
			body->SetPosition(GetRandomPosition());
			m_rxGameFoundation.Add(body);
		} else {
			IwAssertMsg(MYAPP, body, ("Body generator failed to create new body with name '%s'", name.c_str()));
		}
	}
}
