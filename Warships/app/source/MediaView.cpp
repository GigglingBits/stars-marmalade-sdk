#include "MediaView.h"
#include "s3e.h"
#include <algorithm>

void MediaView::SetFileExtensions(const std::string& ext) {
	m_sFileExtensions = ToLower(ext);
}

bool MediaView::IsFileSupported(const std::string& filename) {
	std::string extension = filename.substr(filename.find_last_of(".") + 1);
	return m_sFileExtensions.find(extension) != std::string::npos;
}

void MediaView::SetFile(const std::string& filename) {
	m_sFile = filename;
}

const std::string& MediaView::GetFile() {
	return m_sFile;
}

std::string MediaView::ToLower(const std::string& s) {
	std::string lower(s);
	std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
	return lower;
}
