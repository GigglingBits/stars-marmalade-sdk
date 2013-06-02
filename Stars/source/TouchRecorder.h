#ifndef __TOUCHRECORDER_H__
#define __TOUCHRECORDER_H__

#include <vector>
#include "IwGeomFVec2.h"
#include "s3e.h"

#define TOUCH_REC_MAX_SAMPLE_COUNT	200		// seconds
#define TOUCH_REC_MAX_RESOLUTION	40		// n / seconds

class TouchRecorder {
private:
	uint64 m_ulStartTimeMS;

	uint16 m_uiCursor;
	uint16 m_auiSample[TOUCH_REC_MAX_SAMPLE_COUNT];
	CIwFVec2 m_axPoints[TOUCH_REC_MAX_SAMPLE_COUNT];

public:
	TouchRecorder();
	
public: 
	bool IsRecording();
	
	void StartRecording();
	void Record(const CIwFVec2& pos);
	void EndRecording();
	
	uint16 GetSampleCount();
	CIwFVec2* GetSamples();
};

#endif
