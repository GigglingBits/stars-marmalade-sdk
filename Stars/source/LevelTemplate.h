#ifndef __LEVELTEMPLATE_H__
#define __LEVELTEMPLATE_H__

#include <map>
#include <queue>
#include <string>
#include "IwGeom.h"

class LevelTemplate {
public:
	struct LevelElement {
		std::string BodyName;
		float Position;
		uint16 Delay;
	};
	typedef std::queue<LevelElement> ElementQueue;

private:
	std::string m_sLevelName;
	CIwFVec2 m_xSize;
	float m_fDustRequirement;
	ElementQueue m_xElements;
	
public:
	void SetName(std::string levelname);
	void SetSize(float width, float height);
	void SetDustRequirement(float amount);
	void AddElement(std::string bodyname, float position, uint16 delay);
	void AddElements(float levelheight, const std::map<char, std::string>& defs, const std::vector<std::string>& map);

	std::string GetName();
	CIwFVec2 GetSize();
	float GetDustRequirement();
	ElementQueue& GetElements();
};

#endif
