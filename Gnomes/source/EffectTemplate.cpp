#include "EffectTemplate.h"

void EffectTemplate::Set(std::string type, std::string shape, std::string texture, int duration) {
	m_sType = type;
	m_sShapeId = shape;
	m_sTextureId = texture;
	m_iDuration = duration;
}

const std::string& EffectTemplate::GetType() {
	return m_sType;
}

const std::string& EffectTemplate::GetShapeId() {
	return m_sShapeId;
}

const std::string& EffectTemplate::GetTextureId() {
	return m_sTextureId;
}

const int EffectTemplate::GetDuration() {
	return m_iDuration;
}

