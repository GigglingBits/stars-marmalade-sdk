#include "PreambleTemplate.h"

void PreambleTemplate::SetText(const std::string& text) {
	m_sText = text;
}

const std::string& PreambleTemplate::GetText() {
	return m_sText;
}

void PreambleTemplate::SetFile(const std::string& filename) {
	m_sFile = filename;
}

const std::string& PreambleTemplate::GetFile() {
	return m_sFile;
}

void PreambleTemplate::SetTextureId(const std::string& textureid) {
	m_sTexture = textureid;
}

const std::string& PreambleTemplate::GetTextureId() {
	return m_sTexture;
}
