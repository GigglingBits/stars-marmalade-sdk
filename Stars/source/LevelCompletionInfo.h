#ifndef __LEVELCOMPLETIONINFO_H__
#define __LEVELCOMPLETIONINFO_H__

#include <string>

struct LevelCompletionInfo {
	bool IsCleared;

	float DustFillPercent;
	float DustFillAmount;
	float DustFillMax;
	
	int PathDrawnCount;
	
	LevelCompletionInfo() {
		IsCleared = false;
	
		DustFillPercent = 0.0f;
		DustFillAmount = 0.0f;
		DustFillMax = 0.0f;
		
		PathDrawnCount = 0;
	}
};

#endif
