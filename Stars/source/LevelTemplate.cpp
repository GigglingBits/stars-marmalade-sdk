#include "LevelTemplate.h"
#include "IwDebug.h"

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

LevelTemplate::ElementQueue& LevelTemplate::GetElements() {
	return m_xElements;
}
