#ifndef __LEVELTEMPLATE_H__
#define __LEVELTEMPLATE_H__

#include <map>
#include <string>
#include "IwGeom.h"

class LevelTemplate {
public:
	typedef struct {
		std::string BodyName;
		CIwFVec2 Position;
		float Angle;
	} LevelElement;
	
	typedef std::map<std::string, LevelElement> ElementMap;

private:
	std::string m_sLevelName;
	std::string m_sBackground;
	CIwFVec2 m_xSize;
	ElementMap m_xElements;
	
public:
	void SetName(std::string levelname);
	void SetBackground(std::string name);
	void SetSize(float width, float height);
	void AddElement(std::string elementid, std::string bodyname, const CIwFVec2& position, float angle);

	std::string GetName();
	std::string GetBackground();
	CIwFVec2 GetSize();
	ElementMap& GetElements();

	void Validate();
};

#endif
