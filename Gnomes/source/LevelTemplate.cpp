#include "LevelTemplate.h"
#include "IwDebug.h"

void LevelTemplate::SetName(std::string levelname) {
	m_sLevelName = levelname;
}

std::string LevelTemplate::GetName() {
	return m_sLevelName;
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
	
void LevelTemplate::AddElement(std::string elementid, std::string bodyname, const CIwFVec2& position, float angle) {
	IwAssertMsg(MYAPP, m_xElements.find(elementid) == m_xElements.end(), ("Level '%s' contains more than one element '%s'. The level template is invalid.", m_sLevelName.c_str(), elementid.c_str()));
	LevelElement& element = m_xElements[elementid];
	element.Angle = angle;
	element.Position = position;
	element.BodyName = bodyname;
}

LevelTemplate::ElementMap& LevelTemplate::GetElements() {
	return m_xElements;
}

void LevelTemplate::Validate() {
	bool foundtarget = false;
	ElementMap::iterator it;
	for (it = m_xElements.begin(); it != m_xElements.end(); ++it) {
		if (!it->first.compare("target")) {
			foundtarget = true;
		}
	}
	IwAssertMsg(MYAPP, foundtarget, 
		("Level '%s' invalid. The level contains no target. Add a sprite with the id 'target' to the level definition.", m_sLevelName.c_str()));
}
