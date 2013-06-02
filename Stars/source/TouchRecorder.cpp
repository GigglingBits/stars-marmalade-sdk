#include "TouchRecorder.h"
#include "Debug.h"

TouchRecorder::TouchRecorder() {
	m_ulStartTimeMS = 0;
}

bool TouchRecorder::IsRecording() {
	return m_ulStartTimeMS != 0;
}

void TouchRecorder::StartRecording() {
	IW_CALLSTACK_SELF;
	IwAssertMsg(MYAPP, !IsRecording(), ("Recorded cannot be started if it has been started already."));

	m_ulStartTimeMS = s3eTimerGetMs();
	m_uiCursor = 0;
}

void TouchRecorder::Record(const CIwFVec2& pos) {
	IW_CALLSTACK_SELF;
	IwAssertMsg(MYAPP, IsRecording(), ("Cannot record point; the rcorder must first be started."));
	
	uint16 sample= (uint16)((s3eTimerGetMs() - m_ulStartTimeMS) * TOUCH_REC_MAX_RESOLUTION / 1000);
	
	// re-use last sample if possible
	if (m_uiCursor > 0 && m_auiSample[m_uiCursor - 1] == sample) {
		// IwTrace(MYAPP, ("Re-using sample %i at %i.", m_uiCursor, sample));
		m_uiCursor--;
	}

	// record
	m_auiSample[m_uiCursor] = sample;
	m_axPoints[m_uiCursor] = pos;
	
	// advance cursor, if possible
	if (m_uiCursor != TOUCH_REC_MAX_SAMPLE_COUNT - 1) {
		m_uiCursor++;
	} else {
		IwAssertMsg(MYAPP, false, ("Recorder overflow. Sample %i at %i will be overwritten by the next sample.", m_uiCursor, sample));
	}
}

void TouchRecorder::EndRecording() {
	IW_CALLSTACK_SELF;
	IwAssertMsg(MYAPP, IsRecording(), ("Recorded cannot be ended if it has not been started."));
	
	for (int i = 0; i < m_uiCursor; i++)  {
		IwTrace(MYAPP, ("Touch point %i: %i - %.2f/%.2f", i, m_auiSample[i], m_axPoints[i].x, m_axPoints[i].y));
	}
	
	m_ulStartTimeMS = 0;
}

uint16 TouchRecorder::GetSampleCount() {
	return IsRecording() ? m_uiCursor : 0;
}

CIwFVec2* TouchRecorder::GetSamples() {
	return m_axPoints;
}
