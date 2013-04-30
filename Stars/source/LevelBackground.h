#ifndef __LEVELBACKGROUND_H__
#define __LEVELBACKGROUND_H__

#include <string>
#include "Renderable.h"
#include "GameFoundation.h"

#define CLOUD_INTERVAL_MAX 1000
#define CLOUD_INTERVAL_MIN 300

class LevelBackground : public Renderable {
private:
	CIwFVec2 m_xWorldCenter; // center of world
	CIwFVec2 m_xWorldRadius; // distance vector from center to upper right corner

	GameFoundation& m_rxGame;
	CIwTexture* m_pxBackground;

	uint32 m_uiNextCloudTime;

public:
	LevelBackground(std::string background, GameFoundation& game);

	void SetGeometry(const CIwFVec2& worldsize, const CIwSVec2& viewportsize, float margin);

private:
	void OnUpdate(const FrameData& frame);
	void OnRender(Renderer& renderer, const FrameData& frame);

	uint32 GetNextCloudTime();
	CIwFVec2 GetNextCloudPosition();

	void CreateCloud(const CIwFVec2& pos);

	void SetVerts(const CIwFVec2& offset, CIwFVec2 verts[4]);
};

#endif
