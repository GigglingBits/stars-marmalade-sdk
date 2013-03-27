#include "InputManager.h"

#define POINTER_STATE(state)	(s3ePointerGetState(S3E_POINTER_BUTTON_SELECT) & state)
#define KEY_STATE(key, state)	(s3eKeyboardGetState((s3eKey)key) & state)

InputManager* InputManager::s_pxInstance = NULL;

InputManager::InputManager() {
	// attach event handlers
    m_bUseMultiTouch = s3ePointerGetInt(S3E_POINTER_MULTI_TOUCH_AVAILABLE) ? true : false;
    if (m_bUseMultiTouch) {
        s3ePointerRegister(S3E_POINTER_TOUCH_EVENT, (s3eCallback)MultiTouchCallback, NULL);
        s3ePointerRegister(S3E_POINTER_TOUCH_MOTION_EVENT, (s3eCallback)MultiTouchMotionCallback, NULL);
    } else {
        s3ePointerRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)SingleTouchCallback, NULL);
        s3ePointerRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)SingleTouchMotionCallback, NULL);
    }
	s3eKeyboardRegister(S3E_KEYBOARD_KEY_EVENT, (s3eCallback)KeyCallback, NULL);
}

InputManager::~InputManager() {
	// detach event handlers
	s3eKeyboardUnRegister(S3E_KEYBOARD_KEY_EVENT, (s3eCallback)KeyCallback);
    if (m_bUseMultiTouch) {
        s3ePointerUnRegister(S3E_POINTER_TOUCH_MOTION_EVENT, (s3eCallback)MultiTouchMotionCallback);
        s3ePointerUnRegister(S3E_POINTER_TOUCH_EVENT, (s3eCallback)MultiTouchCallback);
    } else {
        s3ePointerUnRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)SingleTouchMotionCallback);
        s3ePointerUnRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)SingleTouchCallback);
    }
}

void InputManager::Initialize() {
	if (!s_pxInstance) {
		s_pxInstance = new InputManager();
	}
}

InputManager& InputManager::GetInstance() {
	IwAssertMsg(MYAPP, s_pxInstance, ("The input controller is not initialized."));
	return *s_pxInstance;
}

void InputManager::Terminate() {
	if (s_pxInstance) {
		delete s_pxInstance;
		s_pxInstance = NULL;
	}
}

void InputManager::AddButton(VirtualButton& button) {
	m_xButtonMap[&button];
}

void InputManager::RemoveButton(VirtualButton& button) {
	IwAssertMsg(MYAPP, m_xButtonMap.find(&button) != m_xButtonMap.end(), ("Trying to remove button with id %p. But it does not exist.", &button));
	// delete references to the button registration
	for (int i = 0; i < MAX_TOUCHES; i++) {
		if (m_axTouch[i].button == &button) {
			m_axTouch[i].button = NULL;
		}
	}

	// remove the button registration
	m_xButtonMap.erase(&button);
}

InputManager::VirtualButton* InputManager::HitTestButton(const CIwSVec2& point) {
	// only hits enabled buttons
	std::map<VirtualButton*, VirtualButtonState>::iterator it;
	for (it = m_xButtonMap.begin(); it != m_xButtonMap.end(); ++it) {
		if (it->first->enabled && HitTestButton(it->first, point)) {
			return it->first;
		}
	}
	return NULL;
}

ButtonCommandId InputManager::HitTestButtonCommand(const CIwSVec2& point) {
	VirtualButton* button = HitTestButton(point);
	return button ? button->cmdid : eButtonCommandIdNone;
}

bool InputManager::HitTestButton(VirtualButton* button, const CIwSVec2& point) {
	// only hits enabled buttons
	IwAssertMsg(MYAPP, m_xButtonMap.find(button) != m_xButtonMap.end(), ("Trying to hit-test on a button that is not registered!"));
	if (button && button->enabled) {
		return HitTestRect(button->rect, point);
	}
	return false;
}

ButtonCommandId InputManager::HitTestKeyCommand(s3eKey key) {
	VirtualButton* button = HitTestKey(key);
	return button ? button->cmdid : eButtonCommandIdNone;
}

InputManager::VirtualButton* InputManager::HitTestKey(s3eKey key) {
	std::map<VirtualButton*, VirtualButtonState>::iterator it;
	for (it = m_xButtonMap.begin(); it != m_xButtonMap.end(); ++it) {
		VirtualButton* button = it->first;
		if (button && button->key == key && button->enabled) {
			return it->first;
		}
	}
	return NULL;
}

bool InputManager::HitTestRect(const CIwRect& button, const CIwSVec2& point) {
	return 
    button.x <= point.x && 
    button.y <= point.y &&
    button.x + button.w >= point.x && 
    button.y + button.h >= point.y;
}

void InputManager::ResetButtonState() {
	// clear the changed flags
	for (int i = 0; i < MAX_TOUCHES; i++) {
		if (m_axTouch[i].activechanged) {
			if (!m_axTouch[i].active) {
				m_axTouch[i].button = NULL;
				m_axTouch[i].id = NO_TOUCH_ID;
			}
		}
		m_axTouch[i].activechanged = false;
		m_axTouch[i].poschanged = false;
	}
    
	std::map<VirtualButton*, VirtualButtonState>::iterator it;
	for (it = m_xButtonMap.begin(); it != m_xButtonMap.end(); ++it) {
		it->second.activechanged = false;
	}
}

void InputManager::Update() {
	UpdateButtonState();
}

void InputManager::UpdateButtonState() {
	// consolidating the touches and defining the button state
	// (eg. if there are multiple touches on the same button)
	for (int i = 0; i < MAX_TOUCHES; i++) {
		TouchState& tstate = m_axTouch[i];
		if (tstate.id == NO_TOUCH_ID) {
			// invalid touch
			continue;
		}
        
		if (tstate.button == NULL) {
			// pure touch
			if (tstate.activechanged && tstate.active) {
				OnTouchBegin(tstate.id, tstate.startpos);
			} else if (tstate.activechanged && !tstate.active) {
				OnTouchEnd(tstate.id, tstate.endpos);
			} else if (tstate.poschanged) {
				OnTouchMove(tstate.id, tstate.endpos);
			}
			continue;
		}
        
		IwAssertMsg(MYAPP, IsButtonRegistered(tstate.button), ("Button registration is corrupt. Found touch state %i with reference to button %p, but the button does not exist.", i, tstate.button));

		if (!tstate.activechanged) {
			// must be a button, but nothing changed
			continue;
		}

		VirtualButtonState& bstate = m_xButtonMap[tstate.button]; 
		if (tstate.active) {
			// activate, if this is the first touch on the button
			if (!bstate.active) {
				bstate.active = true;
				bstate.activechanged = true;
				OnButtonPressed(*tstate.button);
			}
		} else {
			// de-activate if there are no other active touches on the button
			bool foundanother = false;
			for (int j = 0; j < MAX_TOUCHES; j++) {
				TouchState& tstate2 = m_axTouch[j];
				// see if another touch is still active on the same button
				if (i != j && tstate2.button == tstate.button && tstate2.active) {
					foundanother = true;
				}
			}
			if (!foundanother) {
				bstate.active = false;
				bstate.activechanged = true;
                
				OnButtonReleased(*tstate.button);
			}
		}
	}
}

InputManager::TouchState* InputManager::GetTouchState(TouchId id) {
	TouchState* pInActive = NULL;
    for (uint32 i = 0; i < MAX_TOUCHES; i++) {
        if (id == m_axTouch[i].id) {
            return &m_axTouch[i];
		}
        if (!pInActive && !m_axTouch[i].active) {
            pInActive = &m_axTouch[i];
		}
    }
    
    // Return first inactive touch
    if (pInActive) {
        pInActive->id = id;
        return pInActive;
    }
    
    // No more touches, give up.
    return NULL;
}

bool InputManager::IsButtonRegistered(VirtualButton* button) {
	return m_xButtonMap.find(button) != m_xButtonMap.end();
}

void InputManager::MultiTouchCallback(s3ePointerTouchEvent* event) {
	TouchState* touch = GetInstance().GetTouchState(event->m_TouchID);
    if (touch) {
        touch->active = event->m_Pressed != 0;
        touch->activechanged = true;
		
		if (touch->active) {
			touch->startpos.x = event->m_x;
			touch->startpos.y = event->m_y;
			touch->button = GetInstance().HitTestButton(touch->startpos);
			touch->endpos = touch->startpos;
		} else {
			touch->endpos.x = event->m_x;
			touch->endpos.y = event->m_y;
		}
		touch->poschanged = true;
    }
}

void InputManager::MultiTouchMotionCallback(s3ePointerTouchMotionEvent* event) {
    TouchState* touch = GetInstance().GetTouchState(event->m_TouchID);
	if (touch) {
        touch->endpos.x = event->m_x;
        touch->endpos.y = event->m_y;
		touch->poschanged = true;
    }
}

void InputManager::SingleTouchCallback(s3ePointerEvent* event) {
	TouchState* touch = GetInstance().GetTouchState(SINGLE_TOUCH_ID);
    if (touch) {
        touch->active = event->m_Pressed != 0;
        touch->activechanged = true;
        
        if (touch->active) {
            touch->startpos.x = event->m_x;
            touch->startpos.y = event->m_y;
            touch->button = GetInstance().HitTestButton(touch->startpos);
            touch->endpos = touch->startpos;
        } else {
            touch->endpos.x = event->m_x;
            touch->endpos.y = event->m_y;
        }
        touch->poschanged = true;
    }
}

void InputManager::SingleTouchMotionCallback(s3ePointerMotionEvent* event) {
	TouchState* touch = GetInstance().GetTouchState(SINGLE_TOUCH_ID);
    
    if (touch) {
        touch->endpos.x = event->m_x;
        touch->endpos.y = event->m_y;
        touch->poschanged = true;
    }
}

int32 InputManager::KeyCallback(s3eKeyboardEvent* sys, void*) {
	s3eKeyboardEvent* event = (s3eKeyboardEvent*)sys;
	InputManager& im = GetInstance();
    
	// is a button associated to that key?
	VirtualButton* button = im.HitTestKey(event->m_Key);
	if (button == NULL) {
		return 0;
	}
    
	// set the button
	TouchState* touch = im.GetTouchState(-event->m_Key); // negative values for keyboard events, so that there's no conflict with finger touches
    if (touch) { 
		touch->active = event->m_Pressed != 0;
		touch->activechanged = true;
        
		CIwRect rect = button->rect;
		touch->startpos.x = rect.x + (rect.w / 2);
		touch->startpos.y = rect.y + (rect.h / 2);
		touch->endpos = touch->startpos;
		touch->poschanged = true;
        
		touch->button = button;
	}
	return 0;
}

void InputManager::OnButtonPressed(VirtualButton& button) {
	// let the button handle the event first
	VirtualButton::EventArgs btnargs;
	btnargs.handled = false;
	button.PressedEvent.Invoke(button, btnargs);

	// if not handled, publish the event globally
	if (!btnargs.handled) {
		ButtonEventArgs args;
		args.cmdid = button.cmdid;
		args.userdata = button.userdata;
		ButtonPressedEvent(*this, args);
	}
}

void InputManager::OnButtonReleased(VirtualButton& button) {
	// let the button handle the event first
	VirtualButton::EventArgs btnargs;
	btnargs.handled = false;
	button.ReleasedEvent.Invoke(button, btnargs);

	// if not handled, publish the event globally
	if (!btnargs.handled) {
		ButtonEventArgs args;
		args.cmdid = button.cmdid;
		args.userdata = button.userdata;
		ButtonReleasedEvent(*this, args);
	}
}

void InputManager::OnTouchBegin(TouchId id, const CIwSVec2& pos) {
	TouchEventArgs args;
	args.id = id;
	args.pos = pos;
	TouchBeginEvent(*this, args);
}

void InputManager::OnTouchMove(TouchId id, const CIwSVec2& pos) {
	TouchEventArgs args;
	args.id = id;
	args.pos = pos;
	TouchMoveEvent(*this, args);
}

void InputManager::OnTouchEnd(TouchId id, const CIwSVec2& pos) {
	TouchEventArgs args;
	args.id = id;
	args.pos = pos;
	TouchEndEvent(*this, args);
}

InputManager::VirtualButton::VirtualButton() {
	cmdid = eButtonCommandIdNone;
	key = s3eKeyFirst;
	rect.Make(0, 0, 0, 0);
	userdata = 0;
	enabled = true;

	InputManager::GetInstance().AddButton(*this);
}

InputManager::VirtualButton::~VirtualButton() {
	InputManager::GetInstance().RemoveButton(*this);
}
