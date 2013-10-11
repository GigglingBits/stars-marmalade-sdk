#include "LevelBackground.h"
#include "Debug.h"
#include <cmath>
#include "FactoryManager.h"

LevelBackground::LevelBackground(std::string background, GameFoundation& game) : m_rxGame(game) {
	m_pxBackground = (CIwTexture*)IwGetResManager()->GetResNamed(background.c_str(), "CIwTexture");

	m_xWorldCenter = CIwFVec2::g_Zero;
	m_xWorldRadius = CIwFVec2::g_Zero;

	SetRederingLayer(Renderer::eRenderingLayerBackground);

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

void LevelBackground::SetVerts(const CIwFVec2& offset, CIwFVec2 verts[4]) {
	// perspective corrected geometry
	CIwFVec2& center = m_xWorldCenter;
	CIwFVec2& radius = m_xWorldRadius;

	// construct corrected verts
	verts[0] = center - radius;
	verts[1] = CIwFVec2(center.x + radius.x, center.y - radius.y);
	verts[2] = center + radius;
	verts[3] = CIwFVec2(center.x - radius.x, center.y + radius.y);
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
		distance = "sky";
		velocity = CIwFVec2(-0.5f, 0.0f);
		layer = Renderer::eRenderingLayerHorizonVeryFar;
		break;
	case 1:
		distance = "far";
		velocity = CIwFVec2(-1.5f, 0.0f);
		layer = Renderer::eRenderingLayerHorizonFar;
		break;
	case 2:
	default:
		distance = "near";
		velocity = CIwFVec2(-3.0f, 0.0f);
		layer = Renderer::eRenderingLayerHorizonClose;
		break;
	}

	std::string variation;
	switch (rand() % 3) {
	case 0:
		variation = "01";
		break;
	case 1:
		variation = "02";
		break;
	case 2:
	default:
		variation = "03";
		break;
	}

	std::string cloudname("cloud" + variation + distance);
	LocalEffect* effect = FactoryManager::GetEffectFactory().Create(cloudname);
	if (effect) {
		effect->SetRederingLayer(layer);
		effect->SetVelocity(velocity);
		effect->SetPosition(pos);
		m_rxGame.Add(effect);
		//IwTrace(MYAPP, ("Cloud at %.2f / %.2f", pos.x, pos.y));
	}
}

void LevelBackground::OnUpdate(const FrameData& frame) {
	uint16 frametime = frame.GetSimulatedDurationMs();
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

void LevelBackground::OnRender(Renderer& renderer, const FrameData& frame) {
	IW_CALLSTACK_SELF;

	//const uint32 HAZE = 0x44ffffff;

	CIwFVec2 screencenter = renderer.GetScreenCenterWorldSpace();
	CIwFVec2 offset = screencenter - m_xWorldCenter;

	const int count = 4;
	CIwFVec2 verts[count];

	if (m_pxBackground) {
		SetVerts(offset, verts);
		renderer.DrawImage(m_pxBackground, verts, count); 
		//renderer.DrawPolygon(verts, count, 0x00000000, HAZE); 
	}

	//renderer.DebugDrawCoords(m_xWorldCenter);
	//renderer.DebugDrawCoords(m_xWorldCenter + offset);
	//renderer.DebugDrawCoords(m_xWorldCenter + m_xWorldRadius);
	//renderer.DebugDrawCoords(m_xWorldCenter - m_xWorldRadius);
}
