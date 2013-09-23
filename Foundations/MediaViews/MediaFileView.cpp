#include "MediaFileView.h"
#include "s3e.h"
#include <algorithm>

MediaFileView::MediaFileView(const std::string& filename) : m_sFile(filename) {
	
}

const std::string& MediaFileView::GetFile() {
	return m_sFile;
}
