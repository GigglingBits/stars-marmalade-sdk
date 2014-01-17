#include "LevelBackground.h"
#include "Debug.h"
#include <cmath>
#include "FactoryManager.h"

LevelBackground::LevelBackground(GameFoundation& game) : m_rxGame(game) {
	m_xWorldCenter = CIwFVec2::g_Zero;
	m_xWorldRadius = CIwFVec2::g_Zero;

	m_uiNextCloudTime = 0;
}

void LevelBackground::SetGeometry(const CIwFVec2& worldsize, const CIwSVec2& viewportsize, float margin) {
	// space, wich needs a background
	CIwFVec2 worldsizev(
		std::fabs(worldsize.x), 
		std::fabs(worldsize.y));

	// find geometry of bg image, so that world is 100% covered
	// while maintaining the aspect ratio of the viewport
	float aspectratio = (float)viewportsize.x / (float)viewportsize.y;
	float width = worldsizev.y * aspectratio;
	CIwFVec2 aspectcorrection;
	if (width > worldsizev.x) {
		// make wider
		aspectcorrection.x = (width - worldsizev.x) / 2.0f;							
		aspectcorrection.y = 0.0f;
	} else {
		// make taller
		aspectcorrection.x = 0.0f;
		aspectcorrection.y = ((worldsizev.x / aspectratio) - worldsizev.y) / 2.0f;	
	}

	// defining margin with viewport aspect ratio 
	CIwFVec2 marginv = CIwFVec2(margin, margin);
	if (aspectratio >= 0.0f) {
		marginv.x *= aspectratio;
	} else {
		marginv.y /= aspectratio;
	}

	// definite geometry
	m_xWorldCenter = worldsize / 2.0f;
	m_xWorldRadius = m_xWorldCenter + aspectcorrection + marginv;
}

uint32 LevelBackground::GetNextCloudTime() {
	return (rand() % (CLOUD_INTERVAL_MAX - CLOUD_INTERVAL_MIN)) + CLOUD_INTERVAL_MIN;
}

CIwFVec2 LevelBackground::GetNextCloudPosition() {
	float rand1k = (((float)(rand() % 1000)) / 500.0f) - 1.0f; // range: -1 .. 1
	float radius = m_xWorldRadius.GetLength();

	CIwFVec2 pos(radius, radius * rand1k);
	pos += m_xWorldCenter;
	return pos;
}

void LevelBackground::CreateCloud(const CIwFVec2& pos) {

	std::string distance;
	CIwFVec2 velocity;
	Renderer::RenderingLayer layer;
	switch (rand() % 3) {
		case 0:
			distance = std::string("sky");
			velocity = CIwFVec2(-0.5f, 0.0f);
			layer = Renderer::eRenderingLayerHorizonVeryFar;
			break;
		case 1:
			distance = std::string("far");
			velocity = CIwFVec2(-1.5f, 0.0f);
			layer = Renderer::eRenderingLayerHorizonFar;
			break;
		case 2:
		default:
			distance = std::string("near");
			velocity = CIwFVec2(-3.0f, 0.0f);
			layer = Renderer::eRenderingLayerHorizonClose;
			break;
	}

	std::string variation;
	switch (rand() % 4) {
		case 0:
			variation = std::string("01");
			break;
		case 1:
			variation = std::string("02");
			break;
		case 2:
			variation = std::string("03");
			break;
		case 3:
		default:
			variation = std::string("04");
			break;
	}

	std::string cloudname(std::string("cloud").append(variation).append(distance));
	LocalEffect* effect = FactoryManager::GetEffectFactory().Create(cloudname);
	if (effect) {
		effect->SetRenderingLayer(layer);
		effect->SetVelocity(velocity);
		effect->SetPosition(pos);
		effect->SetFadeTime(-1);
		m_rxGame.Add(effect);
		//IwTrace(MYAPP, ("Cloud at %.2f / %.2f", pos.x, pos.y));
	}
}

void LevelBackground::Update(uint16 frametime) {
	if (m_uiNextCloudTime > frametime) {
		// no cloud yet
		m_uiNextCloudTime -= frametime;
	} else {
		// its time to create a could
		CIwFVec2 pos = GetNextCloudPosition();
		CreateCloud(pos);

		// define time for next cloud
		m_uiNextCloudTime = GetNextCloudTime();
		IwTrace(MYAPP, ("Next cloud in %ims", m_uiNextCloudTime));
	}
}
