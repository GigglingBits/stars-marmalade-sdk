#include "BrickMetrics.h"
#include "IwGx.h"

BrickMetrics::BrickMetrics(BaseBrick& brick) : m_rxBrick(brick) {
	m_bBusyFlag = false;
}

BrickMetrics::~BrickMetrics() {
}

bool BrickMetrics::OnUpdate(bool first) {
	m_xUpdateWatch.Start();
	m_bBusyFlag = m_rxBrick.Update(first);
	m_xUpdateWatch.Stop();
	return m_bBusyFlag;
}

void BrickMetrics::OnRender(const CIwSVec2& screensize, const CIwRect& itemregion) {
	m_xRenderWatch.Start();
	m_rxBrick.Render(screensize, itemregion);
	m_xRenderWatch.Stop();
	ReportStatistics();
}

void BrickMetrics::ReportStatistics() {
	static int cyclems = 0;
	static int renderms = 0;
	static int updatems = 0;
	static uint8 counter = 0;
	counter++;
	if (counter >= 5 ) {
		m_xCycleWatch.Stop();
		cyclems = (int) m_xCycleWatch.GetElapsed() / counter;
		updatems = (int) m_xUpdateWatch.GetElapsed();
		renderms = (int) m_xRenderWatch.GetElapsed();

		counter = 0;
		m_xUpdateWatch.Reset();
		m_xRenderWatch.Reset();
		m_xCycleWatch.Reset();
		m_xCycleWatch.Start();
	}

	const int len = 80;
	char buf[len];
	snprintf(
		buf, len, "%ifps / cycle %ims / update %ims / render %ims %s", 
		cyclems > 0 ? 1000/cyclems : 1000, 
		cyclems, 
		updatems, 
		renderms,
		m_bBusyFlag ? "(busy)\0" : "\0");
	IwGxPrintString(20, 20, buf, false); 
}
