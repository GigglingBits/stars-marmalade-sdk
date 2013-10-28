#include "LevelTemplate.h"
#include "Debug.h"

void LevelTemplate::SetName(std::string levelname) {
	m_sLevelName = levelname;
}

std::string LevelTemplate::GetName() {
	return m_sLevelName;
}

void LevelTemplate::SetDustRequirement(float amount) {
	m_fDustRequirement = amount;
}

void LevelTemplate::SetBackground(std::string name) {
	m_sBackground = name;
}

std::string LevelTemplate::GetBackground() {
	return m_sBackground;
}

void LevelTemplate::SetSize(float width, float height) {
	m_xSize = CIwFVec2(width, height);
}

CIwFVec2 LevelTemplate::GetSize() {
	return m_xSize;
}

float LevelTemplate::GetDustRequirement() {
	return m_fDustRequirement;
}

void LevelTemplate::AddElement(std::string bodyname, float position, uint16 delay) {
	LevelElement element;
	element.BodyName = bodyname;
	element.Position = position;
	element.Delay = delay;
	m_xElements.push(element);
}

void LevelTemplate::AddElements(float levelheight, const std::map<char, std::string>& defs, const std::vector<std::string>& map) {
	IW_CALLSTACK_SELF;
	
	// find number of lanes
	int numberoflanes = 0;
	std::vector<std::string>::const_iterator it;
	for (it = map.begin(); it != map.end(); it++) {
		int length = it->length();
		if (length > numberoflanes) {
			numberoflanes = length;
		}
	}
	
	// calculate lane width
	float worldmargin = levelheight / 10; // 10% of the world is used as margin. No objects will be born there.
	float lanewidth = (levelheight - (2 * worldmargin)) / numberoflanes;
	
	// iterate through map
	const int DELAY = 1500; // milliseconds
	int accumulateddelay = 0;
	for (it = map.begin(); it != map.end(); it++) {
		for (int lane = 0; lane < it->length(); lane ++) {
			char bodydef = it->at(lane);
			std::map<char, std::string>::const_iterator it = defs.find(bodydef);
			if (bodydef == ' ') {
				// nothing; just step over
			} else if (it != defs.end()) {
				AddElement(
					it->second,
					worldmargin + (lanewidth / 2.0f) + (lane * lanewidth),
					accumulateddelay);
				accumulateddelay = 0;
			} else {
				IwAssertMsg(MYAPP, false, ("Unrecognized body reference in lane map line %i: %c", lane, bodydef));
			}
		}
		accumulateddelay += DELAY;
	}
}

LevelTemplate::ElementQueue& LevelTemplate::GetElements() {
	return m_xElements;
}
