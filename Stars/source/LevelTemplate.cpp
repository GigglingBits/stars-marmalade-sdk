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

void LevelTemplate::SetSize(float width, float height) {
	m_xSize = CIwFVec2(width, height);
}

CIwFVec2 LevelTemplate::GetSize() {
	return m_xSize;
}

float LevelTemplate::GetDustRequirement() {
	return m_fDustRequirement;
}

void LevelTemplate::StartSection(std::string icontexture, std::string bannertext) {
	if (icontexture.empty() && bannertext.empty()) {
		return;
	}
	
	LevelElement element;
	element.BodyName = "";
	element.Delay = 0;
	element.Position = 0.0f;
	element.Speed = 0.0f;
	element.SectionIcon = icontexture;
	element.SectionText = bannertext;
	m_xElements.push(element);
}

void LevelTemplate::AddElementDelay(uint16 delay) {
	if (delay > 0) {
		AddElement("", delay, 0.0f, 0.0f);
	}
}

void LevelTemplate::AddElement(std::string bodyname, uint16 delay, float position, float speed) {
	LevelElement element;
	element.BodyName = bodyname;
	element.Delay = delay;
	element.Position = position;
	element.Speed = speed;
	element.SectionIcon = "";
	element.SectionText = "";
	m_xElements.push(element);
}

void LevelTemplate::AddElements(float levelheight, const std::map<char, std::string>& defs, const std::vector<std::string>& map, int delay, float speed) {
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
	for (it = map.begin(); it != map.end(); it++) {
		bool firstelement = true;
		for (uint lane = 0; lane < it->length(); lane++) {
			char bodydef = it->at(lane);
			std::map<char, std::string>::const_iterator it = defs.find(bodydef);
			if (bodydef == ' ') {
				// nothing; just step over
			} else if (it != defs.end()) {
				AddElement(
					it->second,
						   firstelement ? delay : 0,
					worldmargin + (lanewidth / 2.0f) + (lane * lanewidth),
					speed);
				firstelement = false;
			} else {
				IwAssertMsg(MYAPP, false, ("Unrecognized body reference in lane map line %i: %c", lane, bodydef));
			}
		}
		if (firstelement) {
			// must have been empty
			AddElementDelay(delay);
		}
	}
}

LevelTemplate::ElementQueue& LevelTemplate::GetElements() {
	return m_xElements;
}
