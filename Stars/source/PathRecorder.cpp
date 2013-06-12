#include "PathRecorder.h"
#include "Debug.h"

PathRecorder::PathRecorder() {
	m_ulStartTimeMS = 0;
	m_uiLastSampleNumber = 0;
}

bool PathRecorder::IsRecording() {
	return m_ulStartTimeMS != 0;
}

void PathRecorder::StartRecording() {
	IW_CALLSTACK_SELF;
	IwAssertMsg(MYAPP, !IsRecording(), ("Recorder cannot be started if it has been started already."));

	m_ulStartTimeMS = s3eTimerGetMs();
	m_uiLastSampleNumber = 0;

	m_fLastLength = 0.0f;
	m_fTotalLength = 0.0f;
}

void PathRecorder::Record(const CIwFVec2& pos) {
	IW_CALLSTACK_SELF;
	IwAssertMsg(MYAPP, IsRecording(), ("Cannot record point; the recorder must first be started."));
		
	// keep track of the path length
	float length = 0.0f;
	if (!m_xPoints.empty()) {
		length = (pos - m_xPoints.back()).GetLength();
		m_fTotalLength += length;
	}

	// re-use last sample if possible
	uint16 samplenr = (uint16)((s3eTimerGetMs() - m_ulStartTimeMS) * TOUCH_REC_MAX_RESOLUTION / 1000);
	if (!m_xPoints.empty() && m_uiLastSampleNumber == samplenr) {
		m_xPoints.back() = pos;
		m_fTotalLength -= m_fLastLength;
	} else {
		m_xPoints.push_back(pos);
	}

	// prepare for next
	m_fLastLength = length;
	m_uiLastSampleNumber = samplenr;
}

void PathRecorder::EndRecording() {
	IW_CALLSTACK_SELF;
	IwAssertMsg(MYAPP, IsRecording(), ("Recorder cannot be ended if it has not been started."));
	
	m_ulStartTimeMS = 0;
	m_xPoints.clear();
}

uint16 PathRecorder::GetPointCount() {
	return IsRecording() ? m_xPoints.size() : 0;
}

CIwFVec2* PathRecorder::GetPoints() {
	return &m_xPoints[0];
}

float PathRecorder::GetLength() {
	return m_fTotalLength;
}
