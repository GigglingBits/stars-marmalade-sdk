#ifndef __LEVELCOMPLETIONINFO_H__
#define __LEVELCOMPLETIONINFO_H__

#include <string>

struct LevelCompletionInfo {
	bool IsCleared;

	float DustFillPercent;
	float DustFillAmount;
	float DustFillMax;
	
	int PathCount;
	
	int NuggetsColleted;
	int NuggetsMissed;
	
	LevelCompletionInfo() {
		IsCleared = false;
	
		DustFillPercent = 0.0f;
		DustFillAmount = 0.0f;
		DustFillMax = 0.0f;
		
		PathCount = 0;
		
		NuggetsColleted = 0;
		NuggetsMissed = 0;
	}
};

#endif
