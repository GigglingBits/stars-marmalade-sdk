#ifndef __BODYGENERATOR_H__
#define __BODYGENERATOR_H__

#include <list>
#include "Renderable.h"
#include "GameFoundation.h"

class BodyGenerator {
private: 
	typedef struct {
		std::string name;
		uint probability;
	} BodySpec;

	std::list<BodySpec> m_xSpecs;
	uint m_iProbabilitySum;

	CIwRect m_xAreaTimes100;
	GameFoundation& m_rxGameFoundation;

	uint32 m_lLastBodyGenenratedTime;
	uint32 m_lInterval;
	bool m_bStarted;

public:
	BodyGenerator(GameFoundation& game);

	void SetArea(const CIwFVec2& position, const CIwFVec2& size);
	void SetTimeInterval(uint32 interval);
	void Start();
	void Stop();
	void Reset();

	void Update(uint16 timestep);

	void AddBody(const std::string& name, uint probaility);

private:
	const BodySpec& GetRandomBody();
	CIwFVec2 GetRandomPosition();
};

#endif
