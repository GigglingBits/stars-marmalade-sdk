#include <string>

#include "s3e.h"

#include "Configuration.h"
#include "Application.h"
#include "Controller.h"
#include "Directions.h"
#include "IwUI.h"

Application::Application(bool showmetrics, bool autorotate) {
	m_pxController = new Controller();
	m_pxController->Initialize();

	m_pxMetrics = showmetrics ? new BrickMetrics(*m_pxController) : NULL;
	m_pxAutoRotate = autorotate ? new AutoRotate() : NULL;
}

Application::~Application() {
	delete m_pxController;
	if (m_pxMetrics) {
		delete m_pxMetrics;
	}
	if (m_pxAutoRotate) {
		delete m_pxAutoRotate;
	}
}

void Application::Update() {
	// read from other input devices
	s3eKeyboardUpdate();
	s3ePointerUpdate();

	// timing since last update (including rendering)
	uint32 frametime = m_xWatch.GetElapsed();

	// calculate the timing
	m_xWatch.Reset();
	m_xWatch.Start();

	// update application state (do as much work as possible 
	// before the next frame is to be rendered)
	const uint32 maxtimeavailable = 15; // milliseconds
	bool first = true;
	bool morework = true;
	do {
		morework = Update(first);
		first = false;
	} while (morework && maxtimeavailable > m_xWatch.GetElapsed());

	// process/handle UI related input	
	IwGetUIController()->Update(); 

	// update UI and animations
	IwGetUIView()->Update(frametime);
}

bool Application::Update(bool first) {
	bool bIsInput = false;

	bIsInput |= ParseKeyboard();
	bIsInput |= ParseMousewheel();
	bIsInput |= ParseAccelerometer();
	bIsInput |= ParsePointer();

	// reset the back-light timer
	if (bIsInput) {
		s3eDeviceBacklightOn();
	}

	if (m_pxMetrics) {
		return m_pxMetrics->Update(first);
	} else {
		return m_pxController->Update(first);
	}
}

void Application::Render() {
	CIwSVec2 size(
		(int16)Iw2DGetSurfaceWidth(),
		(int16)Iw2DGetSurfaceHeight());

	if (m_pxMetrics) {
		m_pxMetrics->Render(size, CIwRect(0, 0, size.x, size.y));
	} else {
		m_pxController->Render(size, CIwRect(0, 0, size.x, size.y));
	}

	// render the UI
	Iw2DFinishDrawing();
	IwGetUIView()->Render();
}

bool Application::ParseKeyboard() {
	bool bIsInput = false;
	if ((s3eKeyboardGetState(s3eKeyEsc) & S3E_KEY_STATE_PRESSED)) {
		s3eDeviceRequestQuit();
		bIsInput = true;
	}
	if ((s3eKeyboardGetState(s3eKeyLeft) & S3E_KEY_STATE_PRESSED)) {
		m_pxController->Move(eRight);
		bIsInput = true;
	}
	if ((s3eKeyboardGetState(s3eKeyRight) & S3E_KEY_STATE_PRESSED)) {
		m_pxController->Move(eLeft);
		bIsInput = true;
	}
	if ((s3eKeyboardGetState(s3eKeyUp) & S3E_KEY_STATE_PRESSED) || 
		(s3eKeyboardGetState(s3eKeyPageUp) & S3E_KEY_STATE_PRESSED)) {
		m_pxController->Move(eDown);
		bIsInput = true;
	}
	if ((s3eKeyboardGetState(s3eKeyDown) & S3E_KEY_STATE_PRESSED) || 
		(s3eKeyboardGetState(s3eKeyPageDown) & S3E_KEY_STATE_PRESSED)) {
		m_pxController->Move(eUp);
		bIsInput = true;
	}
	if ((s3eKeyboardGetState(s3eKeyV) & S3E_KEY_STATE_PRESSED)) {
		m_pxController->BtnVoicePressed();
		bIsInput = true;
	}
	if ((s3eKeyboardGetState(s3eKeyH) & S3E_KEY_STATE_PRESSED) || 
		(s3eKeyboardGetState(s3eKeyF1) & S3E_KEY_STATE_PRESSED)) {
			if (Configuration::GetInstance().HelpButton) {
				m_pxController->BtnHelpPressed();
				bIsInput = true;
			}
	}
	if ((s3eKeyboardGetState(s3eKeyS) & S3E_KEY_STATE_PRESSED) ||
	    (s3eKeyboardGetState(s3eKeySpace) & S3E_KEY_STATE_PRESSED) ||
	    (s3eKeyboardGetState(s3eKeyEnter) & S3E_KEY_STATE_PRESSED) ||
	    (s3eKeyboardGetState(s3eKeyOk) & S3E_KEY_STATE_PRESSED) ||
	    (s3eKeyboardGetState(s3eKeyAccept) & S3E_KEY_STATE_PRESSED)) {
		m_pxController->BtnSoundPressed();
		bIsInput = true;
	}
	if ((s3eKeyboardGetState(s3eKeyT) & S3E_KEY_STATE_PRESSED) || 
		(s3eKeyboardGetState(s3eKeyA) & S3E_KEY_STATE_PRESSED)) {
		m_pxController->BtnTextPressed();
		bIsInput = true;
	}
	return bIsInput;
}

bool Application::ParseMousewheel() {
	bool bIsInput = false;
	if ((s3ePointerGetState(S3E_POINTER_BUTTON_MOUSEWHEELDOWN) & S3E_POINTER_STATE_PRESSED)) {
		m_pxController->Move(eUp);
		bIsInput = true;
	}
	if ((s3ePointerGetState(S3E_POINTER_BUTTON_MOUSEWHEELUP) & S3E_POINTER_STATE_PRESSED)) {
		m_pxController->Move(eDown);
		bIsInput = true;
	}
	return bIsInput;
}

bool Application::ParseAccelerometer() {
	bool bIsInput = false;
	if (Configuration::GetInstance().AccelerometerStarted) {
		const float forcethreshold = S3E_ACCELEROMETER_EARTH_GRAVITY * 1.8;
		const int32 countthreshold = 10;
		static uint8 shakecounter = 0;
		CIwVec3 forcevector(
			s3eAccelerometerGetX(),
			s3eAccelerometerGetY(),
			s3eAccelerometerGetZ());
		if (forcevector.GetLength() > forcethreshold) {
			if (shakecounter > countthreshold) {
				m_pxController->Shake();
				bIsInput = true;
			} else {
				shakecounter++;
			}
		} else {
			if (shakecounter > 0) {
				shakecounter--;
			}
		}
	}
	return bIsInput;
}

bool Application::ParsePointer() {
	// a 40th fraction of the screen width will trigger the move instead of a touch
	int touchmovethreshold = (int)std::min<uint32>(Iw2DGetSurfaceWidth(), Iw2DGetSurfaceHeight()) / 40;

	bool bIsInput = false;

	static CIwSVec2 xTouchPos;
	static bool bMoveStarted = false;
	if (s3ePointerGetState(S3E_POINTER_BUTTON_SELECT) & S3E_POINTER_STATE_PRESSED) {
		xTouchPos = CIwSVec2(s3ePointerGetX(), s3ePointerGetY());

	} else if (s3ePointerGetState(S3E_POINTER_BUTTON_SELECT) & S3E_POINTER_STATE_RELEASED) {
		if (!bMoveStarted) {
			m_pxController->Touch(RotateCoords(xTouchPos));
			bIsInput = true;
		}
		bMoveStarted = false;

	} else if (s3ePointerGetState(S3E_POINTER_BUTTON_SELECT) & S3E_POINTER_STATE_UP) {
		bMoveStarted = false; // S3E_POINTER_STATE_RELEASED

	} else if (s3ePointerGetState(S3E_POINTER_BUTTON_SELECT) & S3E_POINTER_STATE_DOWN) {
		CIwSVec2 xMovePos = CIwSVec2(s3ePointerGetX(), s3ePointerGetY());
		if (!bMoveStarted 
			&& ((abs(xTouchPos.x - xMovePos.x) > touchmovethreshold) 
			|| (abs(xTouchPos.y - xMovePos.y) > touchmovethreshold))) {
			bMoveStarted = true;
		}
		if (bMoveStarted) {
			m_pxController->TouchMove(
				RotateCoords(xTouchPos), 
				RotateCoords(xMovePos));
		}
		bIsInput = true;
	}

	return bIsInput;
}

CIwSVec2 Application::RotateCoords(const CIwSVec2& pos) {
	if (m_pxAutoRotate) {
		return m_pxAutoRotate->RotateCoords(pos);
	} else {
		return pos;
	}
}
