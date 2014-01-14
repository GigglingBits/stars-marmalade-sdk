#include "LevelProgressBar.h"
#include "FactoryManager.h"
#include "VertexStreamScreen.h"
#include "Debug.h"

LevelProgressBar::LevelProgressBar() : 
	m_pxBackground(NULL),
	m_pxStar(NULL),
	m_fProgress(0.0f),
	m_fMaxValue(1.0f) {
	SetRenderingLayer(Renderer::eRenderingLayerHud);
}

LevelProgressBar::~LevelProgressBar() {
	if (m_pxStar) {
		delete m_pxStar;
	}
	if (m_pxBackground) {
		delete m_pxBackground;
	}
	for (Icons::iterator i = m_xIcons.begin(); i != m_xIcons.end(); i++) {
		if (i->texture) {
			delete i->texture;
		}
		if (i->shape) {
			delete i->shape;
		}
	}
}

void LevelProgressBar::Initialize() {
	IW_CALLSTACK_SELF;
	m_pxBackground = FactoryManager::GetTextureFactory().Create("progressbar");
	m_pxStar = FactoryManager::GetTextureFactory().Create("progressbar_icon_star");
	
	for (Icons::iterator i = m_xIcons.begin(); i != m_xIcons.end(); i++) {
		Icon& icon = *i;
		icon.texture = FactoryManager::GetTextureFactory().Create(icon.textureid);
		if (icon.texture) {
			icon.shape = new VertexStreamScreen();
			UpdateStarShape(icon.progress, *icon.shape);
		}
	}
}

void LevelProgressBar::SetMax(float value) {
	IW_CALLSTACK_SELF;
	IwAssert(MYAPP, value >= 0.0f);

	m_fMaxValue = value;
	UpdateStarShapes();
}

float LevelProgressBar::CalcNormalProgress(float value) {
	IW_CALLSTACK_SELF;
	IwAssert(MYAPP, m_fMaxValue >= 0.0f);

	return value / m_fMaxValue;
}

void LevelProgressBar::UpdateStarShapes() {
	UpdateStarShape(m_fProgress, m_xStarShape);
	for (Icons::iterator i = m_xIcons.begin(); i != m_xIcons.end(); i++) {
		if (i->shape) {
			UpdateStarShape(i->progress, *i->shape);
		}
	}
}

void LevelProgressBar::UpdateStarShape(float progress, VertexStreamScreen& shape) {
	CIwRect rect(GetPosition());
	int size = rect.h;
	int pos = (int)(CalcNormalProgress(progress) * (float)(rect.w - size));
	shape.SetRect(
		rect.x + pos,
		rect.y, size, size);
}

void LevelProgressBar::SetIcon(float progress, const std::string& textureid) {
	IW_CALLSTACK_SELF;

	Icon i;
	i.progress = progress;
	i.textureid = textureid;
	i.texture = NULL;
	i.shape = NULL;
	m_xIcons.push_back(i);
}

void LevelProgressBar::SetProgress(float progress) {
	IW_CALLSTACK_SELF;
	IwAssert(MYAPP, progress >= 0.0f && progress <= m_fMaxValue);

	m_fProgress = progress;
	UpdateStarShape(m_fProgress, m_xStarShape);
}

void LevelProgressBar::OnDoLayout(const CIwSVec2& screensize) {
	m_xBackgroundShape.SetRect(GetPosition());
	UpdateStarShapes();
}

void LevelProgressBar::OnUpdate(const FrameData& frame) {
	IW_CALLSTACK_SELF;

	if (m_pxBackground) {
		m_pxBackground->Update(frame.GetRealDurationMs());
	}
	if (m_pxStar) {
		m_pxStar->Update(frame.GetRealDurationMs());
	}
	for (Icons::iterator i = m_xIcons.begin(); i != m_xIcons.end(); i++) {
		if (i->texture) {
			i->texture->Update(frame.GetRealDurationMs());
		}
	}
}

void LevelProgressBar::OnRender(Renderer& renderer, const FrameData& frame) {
	IW_CALLSTACK_SELF;

	if (m_pxBackground) {
		renderer.Draw(m_xBackgroundShape, *m_pxBackground);
	}
	if (m_pxStar) {
		renderer.Draw(m_xStarShape, *m_pxStar);
	}
	for (Icons::iterator i = m_xIcons.begin(); i != m_xIcons.end(); i++) {
		if (i->texture && i->shape) {
			renderer.Draw(*i->shape, *i->texture);
		}
	}
}
