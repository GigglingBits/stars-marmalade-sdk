#include "LevelInteractor.h"

LevelInteractor::LevelInteractor(Camera& camera, GameFoundation& game) : 
	m_rxCamera(camera), 
	m_rxGame(game) {

	InputManager& im = InputManager::GetInstance();
	im.TouchBeginEvent.AddListener<LevelInteractor>(this, &LevelInteractor::TouchBeginEventHandler);
	im.TouchMoveEvent.AddListener<LevelInteractor>(this, &LevelInteractor::TouchMoveEventHandler);
	im.TouchEndEvent.AddListener<LevelInteractor>(this, &LevelInteractor::TouchEndEventHandler);
}

LevelInteractor::~LevelInteractor() {
	InputManager& im = InputManager::GetInstance();
	im.TouchBeginEvent.RemoveListener<LevelInteractor>(this, &LevelInteractor::TouchBeginEventHandler);
	im.TouchMoveEvent.RemoveListener<LevelInteractor>(this, &LevelInteractor::TouchMoveEventHandler);
	im.TouchEndEvent.RemoveListener<LevelInteractor>(this, &LevelInteractor::TouchEndEventHandler);
}

void LevelInteractor::EvaluateTouchPurpose(TouchSpec& touch) {
	ClearTouchSpec(touch);

	// assume that we are panning the viewport, unless proven to be wrong
	touch.gesturetype = eGestureTypePanZoomViewport;

	// analyze hit point (was there a relevant object hit?)
	typedef std::map<std::string, Sprite*> SpriteMap;
	SpriteMap sprites = m_rxGame.GetSpriteMap();
	for (SpriteMap::iterator it = sprites.begin(); it != sprites.end(); it++) {
		if (Body* body = dynamic_cast<Body*>(it->second)) {
			if (body->HitTest(touch.worldendpos)) {
				touch.targetsprite = it->first;
				touch.targetspritecenter = body->GetCenter();
				if (body->GetTypeName() == Hero::TypeName()) {
					// the hero is not moved; it is charged instead
					// only one hero can be charged at the time
					if (!IsHeroCharging()) {
						touch.gesturetype = eGestureTypeChargeHero;
					}
				} else if (body->CanDrag()) {
					// all other bodies can be moved (if enabled)
					touch.gesturetype = eGestureTypeDragObject;
				}
				// found a sprite; so no need to look further
				break;
			}
		}
	}
}

void LevelInteractor::ClearTouchSpec(TouchSpec& touch) {
	touch.gesturetype = eGestureTypeNone;
	touch.targetsprite.clear();
	touch.targetspritecenter = CIwFVec2::g_Zero;
}

void LevelInteractor::SetHeroChargeInfo(const TouchSpec& touch) {
	m_xHeroCharge.heroid = touch.targetsprite;
	m_xHeroCharge.heropos = touch.targetspritecenter;

	CIwFVec2 dragextent = (touch.targetspritecenter - touch.worldendpos);
	m_xHeroCharge.impulse = dragextent * 10.0f;
}

void LevelInteractor::ClearHeroChargeInfo() {
	m_xHeroCharge.heroid.clear();
	m_xHeroCharge.heropos = CIwFVec2::g_Zero;
	m_xHeroCharge.impulse = CIwFVec2::g_Zero;
}

void LevelInteractor::TouchBeginEventHandler(const InputManager& sender, const InputManager::TouchEventArgs& args) {
	// record new touch
	TouchSpec touch;
	touch.screenstartpos = args.pos;
	touch.screenendpos = touch.screenstartpos;
	touch.worldstartpos =  m_rxCamera.GetViewport().ViewToWorld(touch.screenstartpos);
	touch.worldendpos = touch.worldstartpos;

	EvaluateTouchPurpose(touch);

	// do the dragging
	if (touch.gesturetype == eGestureTypeDragObject) {
		if (Body* body = m_rxGame.FindSprite<Body>(touch.targetsprite)) {
			body->StartDragging(touch.worldstartpos);
		} else {
			// body does no longer exist, e.g. must have died
			ClearTouchSpec(touch);
		}

	} else if (touch.gesturetype == eGestureTypePanZoomViewport) {
		m_xPinch.AddTouch(args.id, args.pos);
		const PinchGesture::PinchInfo& pinch = m_xPinch.GetPinchInfo();
		m_rxCamera.SetScreenPanning(
			-CIwSVec2((int16) pinch.movement.x, (int16) pinch.movement.y), 
			pinch.scalefactor);

	} else if (touch.gesturetype == eGestureTypeChargeHero) {
		m_sHeroInFocus = touch.targetsprite;
		SetHeroChargeInfo(touch);
	}

	m_xTouchRecorder[args.id] = touch;
}

void LevelInteractor::TouchMoveEventHandler(const InputManager& sender, const InputManager::TouchEventArgs& args) {
	// record the motion
	TouchSpec& touch = m_xTouchRecorder[args.id];
	touch.screenendpos = args.pos;
	touch.worldendpos =  m_rxCamera.GetViewport().ViewToWorld(touch.screenendpos);

	// do the dragging
	if (touch.gesturetype == eGestureTypeDragObject) {
		if (Body* body = m_rxGame.FindSprite<Body>(touch.targetsprite)) {
			body->MoveDragging(touch.worldendpos);			
		} else {
			// body does no longer exist, e.g. it must have died
			ClearTouchSpec(touch);
		}

	} else if (touch.gesturetype == eGestureTypePanZoomViewport) {
		m_xPinch.MoveTouch(args.id, args.pos);
		const PinchGesture::PinchInfo& pinch = m_xPinch.GetPinchInfo();
		m_rxCamera.SetScreenPanning(
			-CIwSVec2((int16) pinch.movement.x, (int16) pinch.movement.y), 
			pinch.scalefactor);

	} else if (touch.gesturetype == eGestureTypeChargeHero) {
		SetHeroChargeInfo(touch);
	}
}

void LevelInteractor::TouchEndEventHandler(const InputManager& sender, const InputManager::TouchEventArgs& args) {
	// record the end
	TouchSpec& touch = m_xTouchRecorder[args.id];
	touch.screenendpos = args.pos;
	touch.worldendpos =  m_rxCamera.GetViewport().ViewToWorld(touch.screenendpos);

	if (touch.gesturetype == eGestureTypeDragObject) {
		// end the dragging
		if (Body* body = m_rxGame.FindSprite<Body>(touch.targetsprite)) {
			body->MoveDragging(touch.worldendpos);
			body->EndDragging();
		}

	} else if (touch.gesturetype == eGestureTypePanZoomViewport) {
		m_xPinch.MoveTouch(args.id, args.pos);
		const PinchGesture::PinchInfo& pinch = m_xPinch.GetPinchInfo();
		m_rxCamera.SetScreenPanning(
			-CIwSVec2((int16) pinch.movement.x, 
			(int16) pinch.movement.y), pinch.scalefactor);

		m_xPinch.RemoveTouch(args.id);
		if (!m_xPinch.IsPinching()) {
			m_rxCamera.FinishScreenPanning();
		}

	} else if (touch.gesturetype == eGestureTypeChargeHero) {
		// move hero
		if (Hero* hero = m_rxGame.FindSprite<Hero>(touch.targetsprite)) {
			SetHeroChargeInfo(touch);
			hero->Jump(GetHeroChargeInfo().impulse);
		}
		if (!m_sHeroInFocus.compare(touch.targetsprite)) {
			m_sHeroInFocus.clear();
		}

		// reset charge vector
		ClearHeroChargeInfo();
	}

	// delete
	m_xTouchRecorder.erase(args.id);
}

bool LevelInteractor::IsHeroInFocus() {
	return !m_sHeroInFocus.empty();
}

const std::string& LevelInteractor::GetHeroInFocus() {
	return m_sHeroInFocus;
}

bool LevelInteractor::IsHeroCharging() {
	return !m_xHeroCharge.impulse.IsZero();
}

const LevelInteractor::HeroChargeInfo& LevelInteractor::GetHeroChargeInfo() {
	return m_xHeroCharge;
}

bool LevelInteractor::IsPinching() {
	return m_xPinch.IsPinching();
}

const PinchGesture::PinchInfo& LevelInteractor::GetPinchInfo() {
	return m_xPinch.GetPinchInfo();
}
