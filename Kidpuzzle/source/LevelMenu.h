#ifndef __LEVELMENU_H__
#define __LEVELMENU_H__

#include "Page.h"
#include "IwList.h"
#include "IwUI.h"
#include "Body.h"
#include "Camera.h"
#include "Button.h"

class LevelMenu : public Page {
private:
	Camera m_xCamera;

	Texture* m_pxBackground;

public:
	LevelMenu();
	virtual ~LevelMenu();

	virtual void Initialize();

private:
	void SetupControls();
	void AddTile(const std::string& id, const std::string& image);

protected:
	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);
	virtual void OnDoLayout(const CIwSVec2& screensize);
};

#endif
