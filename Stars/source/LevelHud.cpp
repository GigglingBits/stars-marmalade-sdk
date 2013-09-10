#include "LevelHud.h"
#include "Debug.h"
#include "FactoryManager.h"

LevelHud::LevelHud(GameFoundation& game) :
	m_rxGame(game),
	m_xButtonBlock(eButtonCommandIdStarBlock, s3eKey1),
	m_xButtonAttack(eButtonCommandIdStarAttack, s3eKey2),
	m_pxBackdrop(NULL),
	m_bIsEnabled(true) {
	m_xButtonBlock.PressedEvent.AddListener(this, &LevelHud::ButtonPressedEventHandler);
	m_xButtonBlock.ReleasedEvent.AddListener(this, &LevelHud::ButtonReleasedEventHandler);
	m_xButtonAttack.PressedEvent.AddListener(this, &LevelHud::ButtonPressedEventHandler);
	m_xButtonAttack.ReleasedEvent.AddListener(this, &LevelHud::ButtonReleasedEventHandler);
}

LevelHud::~LevelHud() {
	if (m_pxBackdrop) {
		delete m_pxBackdrop;
	}

	m_xButtonBlock.PressedEvent.RemoveListener(this, &LevelHud::ButtonPressedEventHandler);
	m_xButtonBlock.ReleasedEvent.RemoveListener(this, &LevelHud::ButtonReleasedEventHandler);
	m_xButtonAttack.PressedEvent.RemoveListener(this, &LevelHud::ButtonPressedEventHandler);
	m_xButtonAttack.ReleasedEvent.RemoveListener(this, &LevelHud::ButtonReleasedEventHandler);
}

void LevelHud::Initialize() {
	m_pxBackdrop = FactoryManager::GetTextureFactory().Create("backdrop");

	m_xButtonBlock.SetTexture(FactoryManager::GetTextureFactory().Create("button_action_block"));
	m_xButtonAttack.SetTexture(FactoryManager::GetTextureFactory().Create("button_action_attack"));
	
	m_xDustQueue.Initialize();
	m_xDustCollector.Initialize();
	m_xProgressBar.Initialize();
}

void LevelHud::SetEnabled(bool enabled) {
	if (m_bIsEnabled != enabled) {
		m_xButtonAttack.SetEnabled(enabled);
		m_xButtonBlock.SetEnabled(enabled);
	}
	
	m_bIsEnabled = enabled;
}


void LevelHud::OnDoLayout(const CIwSVec2& screensize) {
	int extent = GetScreenExtents();
	
	int spacing = extent / 40;
	
	int backdropwidth = extent / 5;
	int widgetwidth = backdropwidth - (3 * spacing);

	int buttonwidth = extent / 6;
	int buttonheight = extent / 9;

	int textheight = extent / 15;
	int progressbarheight = extent / 30;
	int dustqueueheight = (screensize.y - (4 * spacing) - progressbarheight - (2 * textheight)) / 3;
	int dustcollectorheight = dustqueueheight * 2;
	
	// action buttons (right)
	CIwRect rect;
	rect.w = buttonwidth;
	rect.h = buttonheight;
	rect.x = screensize.x - buttonwidth;
	rect.y = screensize.y - (2 * buttonheight) - (2 * spacing);
	m_xButtonBlock.SetPosition(rect);
	rect.y += rect.h + spacing;
	m_xButtonAttack.SetPosition(rect);
	
	// backdrop (left)
	m_xBackdropShape.SetRect(0, 0, backdropwidth, screensize.y);
	
	// progress bar (left)
	int x = spacing, y = spacing;
	int w = widgetwidth, h = progressbarheight;
	rect.Make(x, y, w, h);
	m_xProgressBar.SetPosition(rect);

	// dust queue (left)
	y += h + spacing;
	h = dustqueueheight;
	rect.Make(x, y, w, h);
	m_xDustQueue.SetPosition(rect);
	
	// queued dust amount (left)
	m_xQueuedAmount.SetBackground("number_back");
	y += h;
	h = textheight;
	m_xQueuedAmount.SetPosition(CIwSVec2(x, y));
	m_xQueuedAmount.SetSize(CIwSVec2(w, h));
	
	// dust collector (left)
	y += h + spacing;
	h = dustcollectorheight;
	rect.Make(x, y, w, h);
	m_xDustCollector.SetPosition(rect);

	// collected dust amount (left)
	m_xCollectedAmount.SetBackground("number_back");
	y += h;
	h = textheight;
	m_xCollectedAmount.SetPosition(CIwSVec2(x, y));
	m_xCollectedAmount.SetSize(CIwSVec2(w, h));
}

void LevelHud::OnUpdate(const FrameData& frame) {
	IW_CALLSTACK_SELF;

	if (m_pxBackdrop) {
		m_pxBackdrop->Update(frame.GetRealDurationMs());
	}

	// buttons
	m_xButtonBlock.Update(frame);
	m_xButtonAttack.Update(frame);
	
	// progress indicator
	float progress = m_rxGame.GetCompletionDegree();
	m_xProgressBar.SetProgress(progress);
	m_xProgressBar.Update(frame);

	// queued dust
	const int rolltime = 1500;
	const int queuesize = 1000;

	m_xQueuedAmount.SetRollingNumber(m_rxGame.GetDustQueuedAmount(), rolltime);
	m_xQueuedAmount.Update(frame);

	m_xDustQueue.SetProgress(m_rxGame.GetDustQueuedAmount() / queuesize);
	m_xDustQueue.Update(frame);
	
	// collected dust
	m_xCollectedAmount.SetRollingNumber(m_rxGame.GetDustFillAmount(), rolltime);
	m_xCollectedAmount.Update(frame);

	m_xDustCollector.SetProgress(m_rxGame.GetDustFillPercent());
	m_xDustCollector.Update(frame);
}

void LevelHud::OnRender(Renderer& renderer, const FrameData& frame) {
	IW_CALLSTACK_SELF;

	if (!m_bIsEnabled) {
		return;
	}
	
	if (m_pxBackdrop) {
		renderer.SetRederingLayer(Renderer::eRenderingLayerHud3);
		renderer.Draw(m_xBackdropShape, *m_pxBackdrop);
	}

	m_xButtonBlock.Render(renderer, frame);
	m_xButtonAttack.Render(renderer, frame);
	
	m_xProgressBar.Render(renderer, frame);

	m_xDustQueue.Render(renderer, frame);
	m_xQueuedAmount.Render(renderer, frame);

	m_xDustCollector.Render(renderer, frame);
	m_xCollectedAmount.Render(renderer, frame);
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
