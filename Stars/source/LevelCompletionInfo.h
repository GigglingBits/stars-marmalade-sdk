#ifndef __LEVELCOMPLETIONINFO_H__
#define __LEVELCOMPLETIONINFO_H__

#include <string>
#include <vector>

class LevelCompletionInfo {
public:
	struct Points {
		std::string Text;
		float Amount;
	};
		
private:
	// game stats
	float m_fDustFillMax;
	float m_fDustFillAmount;
	
	int m_iPathsStarted;
	int m_iPathsAborted;
	
	int m_iNuggetsDeployed;
	int m_iNuggetsColleted;

	int m_iEnemiesDeployed;
	int m_iEnemiesCollided;
	int m_iEnemiesKilled;

	int m_iBuffsDeployed;
	int m_iBuffsUsed;

	// scores
	bool m_bEvaluated;
	std::vector<Points> m_xPoints;
	float m_fTotalPoints;

public:
	LevelCompletionInfo();
	
	void SetDustFillMax(float f);
	void SetDustFillAmount(float f);

	void IncrementPathsStarted();
	void IncrementPathsAborted();
	
	void IncrementNuggetsDeployed();
	void IncrementNuggetsCollected();
	
	void IncrementEnemiesDeployed();
	void IncrementEnemiesKilled();
	void IncrementEnemiesCollided();

	void IncrementBuffsDeployed();
	void IncrementBuffsUsed();

	void Evaluate();

public:
	void GetPoints(std::vector<Points>& points) const;
	float GetTotalPoints() const;
	int GetAchievedStars() const;
	bool IsAchieved() const;
	
private:
	void AddPoints(const std::string& text, float amount);
};

#endif
