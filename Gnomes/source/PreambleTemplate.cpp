#include "PreambleTemplate.h"

void PreambleTemplate::Set(std::string text) {
	m_sText = text;
}

std::string PreambleTemplate::GetText() {
	return m_sText;
}
