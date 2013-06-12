#ifndef __PathRecorder_H__
#define __PathRecorder_H__

#include <vector>
#include "IwGeomFVec2.h"
#include "s3e.h"

#define TOUCH_REC_MAX_RESOLUTION	40		// samples / second

class PathRecorder {
private:
	uint64 m_ulStartTimeMS;

	uint16 m_uiLastSampleNumber;
	std::vector<CIwFVec2> m_xPoints;

	float m_fLastLength;
	float m_fTotalLength;
	
public:
	PathRecorder();
	
public: 
	bool IsRecording();
	
	void StartRecording();
	void Record(const CIwFVec2& pos);
	void EndRecording();
	
	uint16 GetPointCount();
	CIwFVec2* GetPoints();
	
	float GetLength();
};

#endif
