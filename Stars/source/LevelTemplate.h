#ifndef __LEVELTEMPLATE_H__
#define __LEVELTEMPLATE_H__

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
	std::string m_sBackground;
	CIwFVec2 m_xSize;
	float m_fDustRequirement;
	ElementQueue m_xElements;
	
public:
	void SetName(std::string levelname);
	void SetBackground(std::string name);
	void SetSize(float width, float height);
	void SetDustRequirement(float amount);
	void AddElement(std::string bodyname, float position, uint16 delay);

	std::string GetName();
	std::string GetBackground();
	CIwFVec2 GetSize();
	float GetDustRequirement();
	ElementQueue& GetElements();
};

#endif
