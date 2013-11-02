#include "LevelHud.h"
#include "Debug.h"
#include "FactoryManager.h"

LevelHud::LevelHud(GameFoundation& game) :
	m_rxGame(game),
	m_xButtonBlock(eButtonCommandIdStarBlock, s3eKey1),
	m_xButtonAttack(eButtonCommandIdStarAttack, s3eKey2),
	m_pxVial(NULL),
	m_bIsEnabled(true) {
	m_xButtonBlock.PressedEvent.AddListener(this, &LevelHud::ButtonPressedEventHandler);
	m_xButtonBlock.ReleasedEvent.AddListener(this, &LevelHud::ButtonReleasedEventHandler);
	m_xButtonAttack.PressedEvent.AddListener(this, &LevelHud::ButtonPressedEventHandler);
	m_xButtonAttack.ReleasedEvent.AddListener(this, &LevelHud::ButtonReleasedEventHandler);
}

LevelHud::~LevelHud() {
	if (m_pxVial) {
		delete m_pxVial;
	}
	m_xButtonBlock.PressedEvent.RemoveListener(this, &LevelHud::ButtonPressedEventHandler);
	m_xButtonBlock.ReleasedEvent.RemoveListener(this, &LevelHud::ButtonReleasedEventHandler);
	m_xButtonAttack.PressedEvent.RemoveListener(this, &LevelHud::ButtonPressedEventHandler);
	m_xButtonAttack.ReleasedEvent.RemoveListener(this, &LevelHud::ButtonReleasedEventHandler);
}

void LevelHud::Initialize() {
	m_pxVial = FactoryManager::GetTextureFactory().Create("stardustvial");
	m_xButtonBlock.SetTexture(FactoryManager::GetTextureFactory().Create("button_action_block"));
	m_xButtonAttack.SetTexture(FactoryManager::GetTextureFactory().Create("button_action_attack"));
	
	m_xProgressBar.Initialize();

	m_xDustQueue.Initialize();
	m_xDustQueue.SetRederingLayer(Renderer::eRenderingLayerHud3);
	
	m_xDustCollector.Initialize();
	m_xDustCollector.SetRederingLayer(Renderer::eRenderingLayerHud3);
	
	m_xQueuedAmount.SetBackground("number_back");
	m_xQueuedAmount.SetRederingLayer(Renderer::eRenderingLayerHud);
	m_xQueuedAmount.SetColour(0xffccfaff);
	m_xQueuedAmount.SetFont(Renderer::eFontTypeSmall);
	
	m_xCollectedAmount.SetBackground("number_back");
	m_xCollectedAmount.SetRederingLayer(Renderer::eRenderingLayerHud);
	m_xCollectedAmount.SetColour(0xffccfaff);
	m_xCollectedAmount.SetFont(Renderer::eFontTypeSmall);
}

void LevelHud::SetEnabled(bool enabled) {
	if (m_bIsEnabled != enabled) {
		m_xButtonAttack.SetEnabled(enabled);
		m_xButtonBlock.SetEnabled(enabled);
	}
	
	m_bIsEnabled = enabled;
}

void LevelHud::SetLevelProgress(float progress) {
	m_xProgressBar.SetProgress(progress);
}

void LevelHud::OnDoLayout(const CIwSVec2& screensize) {
	int extent = GetScreenExtents();
	
	int spacing = extent / 40;
	
	int buttonwidth = extent / 6;
	int buttonheight = extent / 9;

	int progressbarheight = extent / 10;

	int dustvialheight = extent / 3;
	int dustvialwidth = extent / 4;
	
	// action buttons (right)
	CIwRect rect;
	rect.w = buttonwidth;
	rect.h = buttonheight;
	rect.x = screensize.x - buttonwidth;
	rect.y = screensize.y - (2 * buttonheight) - (2 * spacing);
	m_xButtonBlock.SetPosition(rect);
	rect.y += rect.h + spacing;
	m_xButtonAttack.SetPosition(rect);
	
	// progress bar
	int x = spacing, y = spacing;
	int w = screensize.x - (extent / 10) - (extent / 30) - (2 * spacing), h = progressbarheight;
	rect.Make(x, y, w, h);
	m_xProgressBar.SetPosition(rect);

	// dust vial
	m_xVialShape.SetRect(spacing, screensize.y - dustvialheight, dustvialwidth, dustvialheight);
	
	// dust queue (left)
	int collectorcorrection = dustvialwidth / 10;
	int collectorheight = dustvialheight * 2 / 3;
	x = spacing + collectorcorrection;
	y = screensize.y - collectorheight;
	w = (dustvialwidth / 2) - collectorcorrection;
	h = collectorheight;
	rect.Make(x, y, w, h);
	m_xDustQueue.SetPosition(rect);

	// dust collector (right)
	x = spacing + (dustvialwidth / 2);
	rect.Make(x, y, w, h);
	m_xDustCollector.SetPosition(rect);
	
	// queued dust amount
	x = spacing + collectorcorrection;
	y = screensize.y - dustvialheight + (extent / 20);
	w = (dustvialwidth / 2) - collectorcorrection;
	h = dustvialheight - collectorheight - (extent / 15);
	m_xQueuedAmount.SetPosition(x, y);
	m_xQueuedAmount.SetSize(w, h);
	
	// collected dust amount
	x = spacing + (dustvialwidth / 2);
	m_xCollectedAmount.SetPosition(x, y);
	m_xCollectedAmount.SetSize(w, h);
}

void LevelHud::OnUpdate(const FrameData& frame) {
	IW_CALLSTACK_SELF;

	// buttons
	m_xButtonBlock.Update(frame);
	m_xButtonAttack.Update(frame);
	
	// progress indicator
	m_xProgressBar.Update(frame);

	// vial
	if (m_pxVial) {
		m_pxVial->Update(frame.GetRealDurationMs());
	}

	// queued dust
	const int rolltime = 3000;
	const int queuesize = 1000;

	m_xQueuedAmount.SetNumber(m_rxGame.GetDustQueuedAmount(), rolltime);
	m_xQueuedAmount.Update(frame);

	m_xDustQueue.SetProgress(m_rxGame.GetDustQueuedAmount() / queuesize, rolltime);
	m_xDustQueue.Update(frame);
	
	// collected dust
	m_xCollectedAmount.SetNumber(m_rxGame.GetDustFillAmount(), rolltime);
	m_xCollectedAmount.Update(frame);

	m_xDustCollector.SetProgress(m_rxGame.GetDustFillPercent(), rolltime);
	m_xDustCollector.Update(frame);
}

void LevelHud::OnRender(Renderer& renderer, const FrameData& frame) {
	IW_CALLSTACK_SELF;

	if (!m_bIsEnabled) {
		return;
	}
	
	m_xButtonBlock.Render(renderer, frame);
	m_xButtonAttack.Render(renderer, frame);
	
	m_xProgressBar.Render(renderer, frame);

	m_xDustQueue.Render(renderer, frame);
	m_xQueuedAmount.Render(renderer, frame);

	m_xDustCollector.Render(renderer, frame);
	m_xCollectedAmount.Render(renderer, frame);

	if (m_pxVial) {
		renderer.SetRederingLayer(Renderer::eRenderingLayerHud2);
		renderer.Draw(m_xVialShape, *m_pxVial);
	}
}

void LevelHud::ButtonPressedEventHandler(const Button& sender, const Button::EventArgs& args) {
	Star* star = m_rxGame.GetStar();
	if (!star) {
		IwAssertMsg(MYAPP, star, ("Cannot initiate action. Star not founnd."));
		return;
	}
	
	switch (args.id) {
		case eButtonCommandIdStarBlock:
			m_xButtonAttack.SetEnabled(false);
			star->BeginBlock();
			break;
		case eButtonCommandIdStarAttack:
			m_xButtonBlock.SetEnabled(false);
			star->BeginAttack();
			break;
		default:
			break;
	}
}

void LevelHud::ButtonReleasedEventHandler(const Button& sender, const Button::EventArgs& args) {
	Star* star = m_rxGame.GetStar();
	if (!star) {
		IwAssertMsg(MYAPP, star, ("Cannot finish action. Star not founnd."));
		return;
	}
	
	switch (args.id) {
		case eButtonCommandIdStarBlock:
			m_xButtonAttack.SetEnabled(true);
			star->EndBlock();
			break;
		case eButtonCommandIdStarAttack:
			m_xButtonBlock.SetEnabled(true);
			star->EndAttack();
			break;
		default:
			break;
	}
}
