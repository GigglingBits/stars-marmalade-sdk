#include "LevelHud.h"
#include "Debug.h"
#include "FactoryManager.h"

LevelHud::LevelHud(GameFoundation& game) :
	m_rxGame(game),
	m_xButtonBlock(eButtonCommandIdStarBlock, s3eKey1),
	m_xButtonAttack(eButtonCommandIdStarAttack, s3eKey2),
	m_bIsEnabled(true) {
	m_xButtonBlock.PressedEvent.AddListener(this, &LevelHud::ButtonPressedEventHandler);
	m_xButtonBlock.ReleasedEvent.AddListener(this, &LevelHud::ButtonReleasedEventHandler);
	m_xButtonAttack.PressedEvent.AddListener(this, &LevelHud::ButtonPressedEventHandler);
	m_xButtonAttack.ReleasedEvent.AddListener(this, &LevelHud::ButtonReleasedEventHandler);
}

LevelHud::~LevelHud() {
	m_xButtonBlock.PressedEvent.RemoveListener(this, &LevelHud::ButtonPressedEventHandler);
	m_xButtonBlock.ReleasedEvent.RemoveListener(this, &LevelHud::ButtonReleasedEventHandler);
	m_xButtonAttack.PressedEvent.RemoveListener(this, &LevelHud::ButtonPressedEventHandler);
	m_xButtonAttack.ReleasedEvent.RemoveListener(this, &LevelHud::ButtonReleasedEventHandler);
}

void LevelHud::Initialize() {
	m_xButtonBlock.SetTexture(FactoryManager::GetTextureFactory().Create("button_action_block"));
	m_xButtonAttack.SetTexture(FactoryManager::GetTextureFactory().Create("button_action_attack"));
	
	m_xStatsPanel.Initialize();
	m_xDustCollector.Initialize();
}

void LevelHud::SetEnabled(bool enabled) {
	if (m_bIsEnabled != enabled) {
		m_xButtonAttack.SetEnabled(enabled);
		m_xButtonBlock.SetEnabled(enabled);
	}
	
	m_bIsEnabled = enabled;
}


void LevelHud::OnDoLayout(const CIwSVec2& screensize) {	
	// action buttons
	const int spacing = 20;
	CIwRect rect;
	rect.w = 120;
	rect.h = 80;
	rect.x = 0;
	rect.y = screensize.y - (2 * rect.h) - (2 * spacing);
	m_xButtonBlock.SetPosition(rect);
	rect.y += rect.h + spacing;
	m_xButtonAttack.SetPosition(rect);
	
	// dust collector
	const int w = 80, h = screensize.y - w - w - 200;
	int x = w / 2, y = w;
	rect.Make(x, y, w, h);
	m_xDustCollector.SetPosition(rect);
	
	// scores
	m_xCollectedAmount.SetBackground("number_back");
	m_xCollectedAmount.SetPosition(CIwSVec2(250, 200));
	m_xCollectedAmount.SetSize(CIwSVec2(100, 50));
	
	m_xQueuedAmount.SetBackground("number_back");
	m_xQueuedAmount.SetPosition(CIwSVec2(350, 200));
	m_xQueuedAmount.SetSize(CIwSVec2(100, 50));
}

void LevelHud::OnUpdate(const FrameData& frame) {
	IW_CALLSTACK_SELF;

	// buttons
	m_xButtonBlock.Update(frame);
	m_xButtonAttack.Update(frame);
	
	// progress indicator
	float progress = m_rxGame.GetCompletionDegree();
	m_xStatsPanel.SetProgress(progress);
	m_xStatsPanel.Update(frame);

	// dust collector
	m_xDustCollector.SetProgress(m_rxGame.GetDustFillPercent());
	m_xDustCollector.Update(frame);
	
	// collected nuggets
	const int rolltime = 1500;
	
	m_xCollectedAmount.SetRollingNumber(m_rxGame.GetDustFillAmount(), rolltime);
	m_xCollectedAmount.Update(frame);
	
	m_xQueuedAmount.SetRollingNumber(m_rxGame.GetDustQueuedAmount(), rolltime);
	m_xQueuedAmount.Update(frame);
}

void LevelHud::OnRender(Renderer& renderer, const FrameData& frame) {
	IW_CALLSTACK_SELF;

	if (!m_bIsEnabled) {
		return;
	}
	
	m_xButtonBlock.Render(renderer, frame);
	m_xButtonAttack.Render(renderer, frame);
	
	m_xStatsPanel.Render(renderer, frame);
	m_xDustCollector.Render(renderer, frame);

	m_xCollectedAmount.Render(renderer, frame);
	m_xQueuedAmount.Render(renderer, frame);
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
