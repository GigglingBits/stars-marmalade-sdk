#ifndef __INPUTCONTROLLER_H__
#define __INPUTCONTROLLER_H__

#include <map>
#include <limits>

#include "s3e.h"
#include "IwGxTypes.h"

#include "ButtonCommandId.h"
#include "MulticastEvent.h"

#define MAX_TOUCHES S3E_POINTER_TOUCH_MAX
#define NO_TOUCH_ID (std::numeric_limits<int32>::max())
#define SINGLE_TOUCH_ID 1

typedef int32 TouchId;

/***
 * Virtual control device. Implements the buttons for interacting with the game.
 * The coordinates are display coordinates.
 ***/
class InputManager {
public:
	class VirtualButton {
	public:
		ButtonCommandId cmdid;
		CIwRect rect;	// position for finger touches
		s3eKey key;		// keyboard stroke that triggers the button
		long userdata;	// this data is just dragged along
		bool enabled;	// disable processing of the button, if false

	public:
		VirtualButton();
		~VirtualButton();

	public:
		struct EventArgs {
			mutable bool handled;
		};
		MulticastEvent<VirtualButton, EventArgs> PressedEvent;
		MulticastEvent<VirtualButton, EventArgs> ReleasedEvent;
	};

private:
	// container for keeping track of touch state,
	// that is finger touches on the screen AND touches
	// on the keyboard
	struct TouchState {
		TouchId id;				// touch's unique identifier (positive is finger / negative is keyboard)
		CIwSVec2 startpos;		// position of touch start 
		CIwSVec2 endpos;		// position of touch release
		bool poschanged;		// "pos" has recently changed
		bool active;			// is touch active (currently down)
		bool activechanged;		// "active" has recently changed
		VirtualButton* button;	// which button is concerned

		TouchState() {
			id = NO_TOUCH_ID;
			startpos = CIwSVec2::g_Zero;
			endpos = CIwSVec2::g_Zero;
			poschanged = false;
			active = false;
			activechanged = false;
			button = NULL;
		}
	};

	// encapsulates the state of a virtual button,
	// and associates it to a virtual button
	struct VirtualButtonState {
		bool active;		// is button active (currently down)
		bool activechanged;	// "active" has recently changed

		VirtualButtonState() {
			active = false;
			activechanged = false;
		}
	};

	static InputManager* s_pxInstance;

	bool m_bUseMultiTouch;
	TouchState m_axTouch[MAX_TOUCHES];
	std::map<VirtualButton*, VirtualButtonState> m_xButtonMap;

private:
	InputManager();
	~InputManager();

	void AddButton(VirtualButton& button);
	void RemoveButton(VirtualButton& button);

public:
	static void Initialize();
	static InputManager& GetInstance();
	static void Terminate();

	// infrastructure
	void ResetButtonState();
	void Update();

	VirtualButton* HitTestButton(const CIwSVec2& point);
	ButtonCommandId HitTestButtonCommand(const CIwSVec2& point);
	VirtualButton* HitTestKey(s3eKey key);
	ButtonCommandId HitTestKeyCommand(s3eKey key);

private:
	static void MultiTouchCallback(s3ePointerTouchEvent* event);
	static void MultiTouchMotionCallback(s3ePointerTouchMotionEvent* event);
	static void SingleTouchCallback(s3ePointerEvent* event);
	static void SingleTouchMotionCallback(s3ePointerMotionEvent* event);
	static int32 KeyCallback(s3eKeyboardEvent* sys, void*);

private:
	TouchState* GetTouchState(TouchId id);

	void UpdatePointerState();
	void UpdateButtonState();

	inline bool HitTestButton(VirtualButton* button, const CIwSVec2& point);
	inline bool HitTestRect(const CIwRect& button, const CIwSVec2& point);
	inline bool IsButtonRegistered(VirtualButton* button);

	void OnButtonPressed(VirtualButton& button);
	void OnButtonReleased(VirtualButton& button);

	void OnTouchBegin(TouchId id, const CIwSVec2& pos);
	void OnTouchMove(TouchId id, const CIwSVec2& pos);
	void OnTouchEnd(TouchId id, const CIwSVec2& pos);

public:
	struct ButtonEventArgs {
		ButtonCommandId cmdid;
		long userdata;
	};
	MulticastEvent<InputManager, ButtonEventArgs> ButtonPressedEvent;
	MulticastEvent<InputManager, ButtonEventArgs> ButtonReleasedEvent;

	struct TouchEventArgs {
		TouchId id;
		CIwSVec2 pos;
	};
	MulticastEvent<InputManager, TouchEventArgs> TouchBeginEvent;
	MulticastEvent<InputManager, TouchEventArgs> TouchMoveEvent;
	MulticastEvent<InputManager, TouchEventArgs> TouchEndEvent;
};

#endif
