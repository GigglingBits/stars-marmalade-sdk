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
	
	m_xProgressBar.Initialize();
	m_xVial.Initialize();
}

void LevelHud::SetEnabled(bool enabled) {
	if (m_bIsEnabled != enabled) {
		m_xButtonAttack.SetEnabled(enabled);
		m_xButtonBlock.SetEnabled(enabled);
	}
	
	m_bIsEnabled = enabled;
}

void LevelHud::SetLevelSectionIcon(uint32 milliseconds, const std::string& texturename) {
	m_xProgressBar.SetIcon(milliseconds, texturename);
}

void LevelHud::SetLevelDuration(uint32 milliseconds) {
	m_xProgressBar.SetMax(milliseconds);
}

void LevelHud::SetLevelProgress(uint32 milliseconds) {
	m_xProgressBar.SetProgress(milliseconds);
}

void LevelHud::OnDoLayout(const CIwSVec2& screensize) {
	int extent = GetScreenExtents();
	
	int spacing = extent / 40;
	
	int buttonwidth = extent / 6;
	int buttonheight = extent / 9;

	int progressbarheight = extent / 20;

	int dustvialheight = extent / 3;
	int dustvialwidth = extent / 7;
	
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
	rect.Make(spacing, screensize.y - dustvialheight, dustvialwidth, dustvialheight);
	m_xVial.SetPosition(rect);
}

void LevelHud::OnUpdate(const FrameData& frame) {
	IW_CALLSTACK_SELF;

	m_xButtonBlock.Update(frame);
	m_xButtonAttack.Update(frame);
	
	m_xProgressBar.Update(frame);

	m_xVial.SetDustAmount(m_rxGame.GetDustFillPercent(), m_rxGame.GetDustQueuedPercent());
	m_xVial.Update(frame);
}

void LevelHud::OnRender(Renderer& renderer, const FrameData& frame) {
	IW_CALLSTACK_SELF;

	if (!m_bIsEnabled) {
		return;
	}
	
	m_xButtonBlock.Render(renderer, frame);
	m_xButtonAttack.Render(renderer, frame);
	
	m_xProgressBar.Render(renderer, frame);
	m_xVial.Render(renderer, frame);
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
