#ifndef __TEXTURETEMPLATE_H__
#define __TEXTURETEMPLATE_H__

#include <string>
#include <vector>

#include "TextureFrame.h"

class TextureTemplate {
private:
	std::vector<TextureFrame> m_xFrames;
	std::string m_sSkeletonAnimation;
	
public:
	void AddFrame(const std::string& id, int healthlevel, const std::string& image, const std::string& pattern, uint32 colour, int duration, const std::string& nextid);

	bool IsSkeletonAnimation() const;
	void SetSkeletonAnimation(const std::string& animation);
	const std::string& GetSkeletonAnimation() const;

	int GetFrameCount() const;
	const TextureFrame& GetFrameInfo(int index) const;

private:
	void AddImage(const std::string& id, int healthlevel, const std::string& image);
	void AddPattern(const std::string& id, int healthlevel, const std::string& pattern);
	void AddImageAnimation(const std::string& id, int healthlevel, const std::string& image, int duration, const std::string& nextid);
	void AddColour(const std::string& id, int healthlevel, uint32 colour);
};

#endif
