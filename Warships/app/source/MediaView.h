#ifndef __MEDIAVIEW_H__
#define __MEDIAVIEW_H__

#include <string>
#include "IwGeom.h"
#include "Event.h"

class MediaView {
private:
	std::string m_sFile;
	std::string m_sFileExtensions;

public:
	bool IsFileSupported(const std::string& filename);
	void SetFile(const std::string& filename);

	virtual bool IsShowing() = 0;

	virtual void Show(const CIwVec2& pos, const CIwVec2& size) = 0;
	virtual void Hide() = 0;

protected:
	const std::string& GetFile();
	void SetFileExtensions(const std::string& ext);

private:
	std::string ToLower(const std::string& s);

public:
	Event<MediaView, int> Finished;
};

#endif
