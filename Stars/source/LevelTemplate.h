#ifndef __LEVELTEMPLATE_H__
#define __LEVELTEMPLATE_H__

#include <map>
#include <queue>
#include <string>
#include "IwGeom.h"

class LevelTemplate {
public:
	struct LevelElement {
		uint16 Delay;
		std::string BodyName;
		float Position;
		float Speed;
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
	void AddElement(std::string bodyname, uint16 delay, float position, float speed);
	void AddElements(float levelheight, const std::map<char, std::string>& defs, const std::vector<std::string>& map, int delay, float speed);

	std::string GetName();
	CIwFVec2 GetSize();
	float GetDustRequirement();
	ElementQueue& GetElements();
};

#endif
