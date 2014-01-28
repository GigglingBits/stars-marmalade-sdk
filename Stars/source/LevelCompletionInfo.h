#ifndef __LEVELCOMPLETIONINFO_H__
#define __LEVELCOMPLETIONINFO_H__

#include <string>

struct LevelCompletionInfo {
	bool IsCleared;
	float DustFillPercent;
	
	LevelCompletionInfo() {
		IsCleared = false;
		DustFillPercent = 0.0f;
	}
};

#endif
