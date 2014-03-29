#ifndef __LEVELCOMPLETIONINFO_H__
#define __LEVELCOMPLETIONINFO_H__

#include <string>

class LevelCompletionInfo {
private:
	float m_fDustFillMax;
	float m_fDustFillAmount;
	
	int m_iPathsStarted;
	int m_iPathsAborted;
	
	int m_iNuggetsColleted;
	int m_iNuggetsMissed;

public:
	LevelCompletionInfo();
	
	bool IsCleared() const;
	
	void SetDustFillMax(float f);
	void SetDustFillAmount(float f);

	void IncrementPathsStarted();
	void IncrementPathsAborted();
	
	void IncrementNuggetsCollected();
	void IncrementNuggetsMissed();
	
public:
	float GetDustAmount() const;
	int GetPathsStarted() const;
	int GetNuggetsCollected() const;

	bool GetDustAchievement() const;
};

#endif
