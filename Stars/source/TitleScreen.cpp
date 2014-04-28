#include "TitleScreen.h"
#include "FactoryManager.h"
#include "Debug.h"
#include "Configuration.h"

#include "spine/extension.h"

TitleScreen::TitleScreen() :
Page("menu.group", Configuration::GetInstance().MenuSong),
m_xButtonTitle(eButtonCommandIdOpenWorldMenu, s3eKeyAbsOk),
m_xButtonMovie(eButtonCommandIdOpenIntroMovie, s3eKeyM),
m_xPanelOptions(eButtonCommandIdOptions, s3eKeyFirst),
m_xPanelSocial(eButtonCommandIdSocial, s3eKeyFirst),
m_fAnimRot(0.0f) {
	m_xPanelOptions.StateChanged.AddListener<TitleScreen>(this, &TitleScreen::ButtonPanelStateChangedEventHandler);
	m_xPanelSocial.StateChanged.AddListener<TitleScreen>(this, &TitleScreen::ButtonPanelStateChangedEventHandler);
}


TitleScreen::~TitleScreen() {
	m_xPanelOptions.StateChanged.RemoveListener<TitleScreen>(this, &TitleScreen::ButtonPanelStateChangedEventHandler);
	m_xPanelSocial.StateChanged.RemoveListener<TitleScreen>(this, &TitleScreen::ButtonPanelStateChangedEventHandler);
}

void TitleScreen::Initialize() {
	IW_CALLSTACK_SELF;
	
	m_xCamera.SetGeometry(CIwFVec2(15.0f, 5.0f), CIwSVec2(IwGxGetScreenWidth(), IwGxGetScreenHeight()), 10.0f);
	m_xCamera.SetWorldFocus(CIwFVec2(0.0f, 0.0f));
	m_xCamera.ZoomOut();
	
	m_xBackground.Initialize();
	m_xPanelOptions.Initialize();
	m_xPanelOptions.GetMainButton().SetTexture(FactoryManager::GetTextureFactory().Create("button_options"));
	
	m_xPanelSocial.Initialize();
	m_xPanelSocial.GetMainButton().SetTexture(FactoryManager::GetTextureFactory().Create("button_achievements"));

    m_xButtonTitle.SetTexture(FactoryManager::GetTextureFactory().Create("title_button"));
    m_xButtonMovie.SetTexture(FactoryManager::GetTextureFactory().Create("button_movie"));

	PageSettings ps;
	ps.SetWorld(LevelIterator::eWorldIdEarth);
	SetBackground(
		ps.GetWorldColours().LowerLeft,
		ps.GetWorldColours().LowerRight,
		ps.GetWorldColours().UpperRight,
		ps.GetWorldColours().UpperLeft);

	float ll = -1.0f;
	float ur = 2.0f;
	
	m_xShape[0] = CIwFVec2(ll, ll);
	m_xShape[1] = CIwFVec2(ur, ll);
	m_xShape[2] = CIwFVec2(ur, ur);
	m_xShape[3] = CIwFVec2(ll, ur);
	
	m_xAnim.Load("spine/title/output/title.atlas", "spine/title/output/title.json");
	m_xAnim.SetAnimation("enter");
	m_xAnim.ConfineAnimation(m_xShape, 4);
}

void TitleScreen::OnDoLayout(const CIwSVec2& screensize) {
	CIwSVec2 screencenter(screensize.x / 2, screensize.y / 2);
	CIwRect button;
	int extents = GetScreenExtents();
	
	// main button
    button.w = extents * 0.9;
    button.h = button.w * 2 / 3;
    button.x = screencenter.x - (button.w / 2);
	button.y = extents / 6;
	m_xButtonTitle.SetPosition(button);
	
    // menu buttons
    button.w = extents / 7;
    button.h = extents / 7;
    button.x = screencenter.x - (extents / 2);
	button.y = screencenter.y + (extents / 3);
	m_xPanelOptions.GetMainButton().SetPosition(button);
    
	button.x = screencenter.x - (button.w / 2);
	m_xButtonMovie.SetPosition(button);

	button.x = screencenter.x + (extents / 2) - button.w;
	m_xPanelSocial.GetMainButton().SetPosition(button);
}

void TitleScreen::OnUpdate(const FrameData& frame) {
	IW_CALLSTACK_SELF;

	m_xButtonTitle.Update(frame);
	m_xPanelSocial.Update(frame);
	m_xButtonMovie.Update(frame);
	m_xPanelOptions.Update(frame);
	m_xBackground.Update(frame);

	m_xAnim.Update(frame.GetSimulatedDurationMs());
}

void TitleScreen::OnRender(Renderer& renderer, const FrameData& frame) {
	IW_CALLSTACK_SELF;

	renderer.SetViewport(m_xCamera.GetViewport());
	m_xBackground.Render(renderer, frame);
	
	int length = m_xAnim.GetVertexCount();
	CIwFVec2* xys = new CIwFVec2[length];
	CIwFVec2* uvs = new CIwFVec2[length];
	uint32* cols = new uint32[length];
	CIwFVec2 bbox[4];
	
	m_fAnimRot += 0.01;
	
	if (CIwTexture* texture = m_xAnim.GetStreams(length, xys, uvs, cols)) {
		m_xAnim.SetPosition(CIwFVec2(1.0f, 1.0f));
		m_xAnim.SetRotation(m_fAnimRot);
		
		renderer.DrawImage(texture, xys, uvs, cols, length);
		m_xAnim.GetDebugSkeletonBoundigBox(bbox);
		renderer.DrawPolygon(bbox, sizeof(bbox) / sizeof(CIwFVec2), 0xcc00ee00, 0x5544aa44);
		m_xAnim.GetDebugSkeletonOrigin(bbox);
		renderer.DrawPolygon(bbox, sizeof(bbox) / sizeof(CIwFVec2), 0xccffee00, 0x55ccaa44);
		m_xAnim.GetDebugAnimationOrigin(bbox);
		renderer.DrawPolygon(bbox, sizeof(bbox) / sizeof(CIwFVec2), 0xcc00eeff, 0x5544aacc);
	}
	
	if (CIwTexture* texture = m_xAnim.GetStreams(length, xys, uvs, cols)) {
		m_xAnim.SetRotation(m_fAnimRot);
		m_xAnim.SetPosition(CIwFVec2(-1.0f, -1.0f));
		
		renderer.DrawImage(texture, xys, uvs, cols, length);
		m_xAnim.GetDebugSkeletonBoundigBox(bbox);
		renderer.DrawPolygon(bbox, sizeof(bbox) / sizeof(CIwFVec2), 0xcc00ee00, 0x5544aa44);
		m_xAnim.GetDebugSkeletonOrigin(bbox);
		renderer.DrawPolygon(bbox, sizeof(bbox) / sizeof(CIwFVec2), 0xccffee00, 0x55ccaa44);
		m_xAnim.GetDebugAnimationOrigin(bbox);
		renderer.DrawPolygon(bbox, sizeof(bbox) / sizeof(CIwFVec2), 0xcc00eeff, 0x5544aacc);
	}
	delete [] xys;
	delete [] uvs;
	delete [] cols;
	
	renderer.DrawPolygon(m_xShape, 4, 0xcc00eeff, 0x5544aacc);

	
    // buttons
	m_xButtonTitle.Render(renderer, frame);
	m_xPanelOptions.Render(renderer, frame);
	m_xButtonMovie.Render(renderer, frame);
	m_xPanelSocial.Render(renderer, frame);
}

void TitleScreen::ButtonPressedEventHandler(const Button& sender, const Button::EventArgs& args) {
    if (&sender == &m_xButtonTitle) {
		SetCompletionState(eCompleted);
    }
}

void TitleScreen::ButtonPanelStateChangedEventHandler(const ButtonPanel& sender, const ButtonPanel::EventArgs& args) {
	if (&sender == &m_xPanelOptions) {
		m_xPanelSocial.SetEnabled(!args.IsPanelOpen);
	} else if (&sender == &m_xPanelSocial) {
		m_xPanelOptions.SetEnabled(!args.IsPanelOpen);
	}
	m_xButtonTitle.SetEnabled(!args.IsPanelOpen);
}
