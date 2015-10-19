#ifndef _BRICKMETRICS_H
#define _BRICKMETRICS_H

#include "BaseBrick.h"
#include "Stopwatch.h"

class BrickMetrics : public BaseBrick {
private:
	BaseBrick& m_rxBrick;

	bool m_bBusyFlag;
	Stopwatch m_xCycleWatch;
	Stopwatch m_xUpdateWatch;
	Stopwatch m_xRenderWatch;

public:
	BrickMetrics(BaseBrick& brick);
	virtual ~BrickMetrics();

	bool OnUpdate(bool first);
	void OnRender(const CIwSVec2& screensize, const CIwRect& itemregion);

private:
	void ReportStatistics();
};

#endif

