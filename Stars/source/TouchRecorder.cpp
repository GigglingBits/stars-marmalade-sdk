#include "TouchRecorder.h"
#include "Debug.h"

TouchRecorder::TouchRecorder() {
	m_ulStartTimeMS = 0;
	m_uiLastSample = 0;
}

bool TouchRecorder::IsRecording() {
	return m_ulStartTimeMS != 0;
}

void TouchRecorder::StartRecording() {
	IW_CALLSTACK_SELF;
	IwAssertMsg(MYAPP, !IsRecording(), ("Recorded cannot be started if it has been started already."));

	m_ulStartTimeMS = s3eTimerGetMs();
	m_uiLastSample = 0;
}

void TouchRecorder::Record(const CIwFVec2& pos) {
	IW_CALLSTACK_SELF;
	IwAssertMsg(MYAPP, IsRecording(), ("Cannot record point; the recorder must first be started."));
	
	uint16 sample= (uint16)((s3eTimerGetMs() - m_ulStartTimeMS) * TOUCH_REC_MAX_RESOLUTION / 1000);
	
	if (!m_xPoints.empty() && m_uiLastSample == sample) {
		// re-use last sample if possible
		m_xPoints.back() = pos;
	} else {
		m_xPoints.push_back(pos);
	}

	m_uiLastSample = sample;
}

void TouchRecorder::EndRecording() {
	IW_CALLSTACK_SELF;
	IwAssertMsg(MYAPP, IsRecording(), ("Recorded cannot be ended if it has not been started."));
	
	m_ulStartTimeMS = 0;
	m_xPoints.clear();
}

uint16 TouchRecorder::GetSampleCount() {
	return IsRecording() ? m_xPoints.size() : 0;
}

CIwFVec2* TouchRecorder::GetSamples() {
	return &m_xPoints[0];
}
