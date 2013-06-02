#include "LevelInteractor.h"

LevelInteractor::LevelInteractor(Camera& camera, GameFoundation& game) : 
	m_rxCamera(camera), 
	m_rxGame(game),
    m_xTouchpad() {

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

	// analyze hit point (was a non-star, draggable object hit?)
	typedef std::map<std::string, Sprite*> SpriteMap;
	SpriteMap sprites = m_rxGame.GetSpriteMap();
	for (SpriteMap::iterator it = sprites.begin(); it != sprites.end(); it++) {
		if (Body* body = dynamic_cast<Body*>(it->second)) {
			if (body->HitTest(touch.worldendpos) && body->CanDrag() && !body->IsDragging()) {
				touch.targetsprite = it->first;
				touch.gesturetype = eGestureTypeDragObject;
				break;
			}
		}
	}

	// nothing draggable hit? try the touchpad
	if (touch.gesturetype == eGestureTypeNone && m_xTouchpad.HitTest(touch.screenstartpos)) {
		Star* star = m_rxGame.GetStar();
		if (star && !m_xTouchpad.IsTouching()) {
			touch.gesturetype = eGestureTypeMoveStar;
			touch.targetsprite = star->GetId();
		}
	}
	
	// nothing draggable hit? start recording the path of the touch 
	if (touch.gesturetype == eGestureTypeNone && !m_xRecorder.IsRecording()) {
		touch.gesturetype = eGestureTypeDrawStarPath;
	}
}

void LevelInteractor::ClearTouchSpec(TouchSpec& touch) {
	touch.gesturetype = eGestureTypeNone;
	touch.targetsprite.clear();
}

void LevelInteractor::OnUpdate(const FrameData& frame) {
	const CIwSVec2& framesize = frame.GetScreensize();
	m_xTouchpad.SetSize(CIwSVec2(framesize.x / 4, framesize.y / 4));
	m_xTouchpad.SetPosition(CIwSVec2(framesize.x / 4 * 3, framesize.y / 4 * 3));
    m_xTouchpad.Update(frame);
}

void LevelInteractor::OnRender(Renderer& renderer, const FrameData& frame) {
    m_xTouchpad.Render(renderer, frame);
	
	uint16 count = m_xRecorder.GetSampleCount();
	if (count > 0) {
		renderer.DrawPolygon(
			m_xRecorder.GetSamples(),
			count,
			0xffffffff,
			0x00000000);
	}
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
			body->BeginDragging(body->GetCenter());
			body->MoveDragging(touch.worldendpos);			
		} else {
			// body does no longer exist, e.g. must have died
			ClearTouchSpec(touch);
		}

		// do navigation
	} else if (touch.gesturetype == eGestureTypeMoveStar) {
		IwAssertMsg(MYAPP, !m_xTouchpad.IsTouching(), ("Already touching; this call is unintentional."));
		m_xTouchpad.SetTouch(touch.screenendpos);
		BeginMoveStar.Invoke(*this, m_xTouchpad.GetTouchVectorNormalized());
	
	// do path recording
	} else if (touch.gesturetype == eGestureTypeDrawStarPath) {
		IwAssertMsg(MYAPP, !m_xRecorder.IsRecording(), ("Already recording; this call is unintentional."));
		m_xRecorder.StartRecording();
		m_xRecorder.Record(touch.worldstartpos);
	}

	m_xTouchMap[args.id] = touch;
}

void LevelInteractor::TouchMoveEventHandler(const InputManager& sender, const InputManager::TouchEventArgs& args) {
	// record the motion
	TouchSpec& touch = m_xTouchMap[args.id];
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

	// do navigation
	} else if (touch.gesturetype == eGestureTypeMoveStar) {
		IwAssertMsg(MYAPP, m_xTouchpad.IsTouching(), ("Not dragging; this call is unintentional."));
		m_xTouchpad.SetTouch(touch.screenendpos);
		MoveStar.Invoke(*this, m_xTouchpad.GetTouchVectorNormalized());
		
	// do path recording
	} else if (touch.gesturetype == eGestureTypeDrawStarPath) {
		IwAssertMsg(MYAPP, m_xRecorder.IsRecording(), ("Not recording; this call is unintentional."));
		m_xRecorder.Record(touch.worldendpos);
	}
}

void LevelInteractor::TouchEndEventHandler(const InputManager& sender, const InputManager::TouchEventArgs& args) {
	// record the end
	TouchSpec& touch = m_xTouchMap[args.id];
	touch.screenendpos = args.pos;
	touch.worldendpos =  m_rxCamera.GetViewport().ViewToWorld(touch.screenendpos);

	if (touch.gesturetype == eGestureTypeDragObject) {
		// end the dragging
		if (Body* body = m_rxGame.FindSprite<Body>(touch.targetsprite)) {
			body->MoveDragging(touch.worldendpos);
			body->EndDragging();
		}

	// do navigation
	} else if (touch.gesturetype == eGestureTypeMoveStar) {
		IwAssertMsg(MYAPP, m_xTouchpad.IsTouching(), ("Not dragging; this call is unintentional."));
		m_xTouchpad.SetTouch(touch.screenendpos);
		EndMoveStar.Invoke(*this, m_xTouchpad.GetTouchVectorNormalized());
		m_xTouchpad.UnsetTouch();
		
	// do path recording
	} else if (touch.gesturetype == eGestureTypeDrawStarPath) {
		IwAssertMsg(MYAPP, m_xRecorder.IsRecording(), ("Not recording; this call is unintentional."));
		m_xRecorder.Record(touch.worldendpos);
		m_xRecorder.EndRecording();
	}

	// delete
	m_xTouchMap.erase(args.id);
}
