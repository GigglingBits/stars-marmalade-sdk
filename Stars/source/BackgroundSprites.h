#ifndef __BACKGROUNDSPRITES_H__
#define __BACKGROUNDSPRITES_H__

#include <string>
#include "GameFoundation.h"

#define CLOUD_INTERVAL_MAX 1000
#define CLOUD_INTERVAL_MIN 300

class BackgroundSprites {
private:
	CIwFVec2 m_xWorldCenter; // center of world
	CIwFVec2 m_xWorldRadius; // distance vector from center to upper right corner

	GameFoundation& m_rxGame;

	uint32 m_uiNextCloudTime;

public:
	BackgroundSprites(GameFoundation& game);

	void SetGeometry(const CIwFVec2& worldsize, const CIwSVec2& viewportsize, float margin);
	void Update(uint16 frametime);

private:
	uint32 GetNextCloudTime();
	CIwFVec2 GetNextCloudPosition();
	void CreateCloud(const CIwFVec2& pos);
};

#endif
