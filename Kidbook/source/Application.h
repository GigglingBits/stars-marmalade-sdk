//--------------------------------------------------------------------------
// Top-level repesentation of the application
//--------------------------------------------------------------------------

#ifndef _APPLICATION_H_ 
#define _APPLICATION_H_ 

#include <string>
#include "AutoRotate.h"
#include "Controller.h"
#include "BrickMetrics.h"
#include "Stopwatch.h"

class Application {
private:
	Controller* m_pxController;
	BrickMetrics* m_pxMetrics;
	AutoRotate* m_pxAutoRotate;

	Stopwatch m_xWatch;

public:
	Application(bool showmetrics, bool autorotate);
	~Application();

	void Update();
	void Render();

private:
	bool Update(bool first);

	bool ParseKeyboard();
	bool ParseMousewheel();
	bool ParseAccelerometer();
	bool ParsePointer();

	CIwSVec2 RotateCoords(const CIwSVec2& pos);
};

#endif
