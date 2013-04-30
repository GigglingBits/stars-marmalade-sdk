#ifndef __EFFECTTEMPLATE_H__
#define __EFFECTTEMPLATE_H__

#include <string>

class EffectTemplate {
private:
	std::string m_sType;
	std::string m_sShapeId;
	std::string m_sTextureId;
	int m_iDuration;
	
public:
	void Set(std::string type, std::string shape, std::string texture, int duration);

	const std::string& GetType();
	const std::string& GetShapeId();
	const std::string& GetTextureId();
	const int GetDuration();
};

#endif
