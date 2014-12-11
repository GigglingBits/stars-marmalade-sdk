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

	int m_iBirdsDeployed;
	int m_iBirdsCollided;
	int m_iBirdsKilled;
	
	int m_iBuffsDeployed;
	int m_iBuffMagnetsUsed;
	int m_iBuffShieldsUsed;
	int m_iBuffShotsUsed;

	int m_iLifesGranted;
	int m_iLifesLeft;
	
	// scores
	bool m_bEvaluated;
	std::vector<Points> m_xIndividualPoints;
	float m_fTotalPoints;

public:
	LevelCompletionInfo();
	
	void SetDustFillMax(float f);
	void SetDustFillAmount(float f);

	void SetNumberOfLifesGranted(int lives);
	void SetNumberOfLifesLeft(int lives);
	
	void IncrementPathsStarted();
	void IncrementPathsAborted();
	
	void IncrementNuggetsDeployed();
	void IncrementNuggetsCollected();
	
	void IncrementBirdsDeployed();
	void IncrementBirdsKilled();
	void IncrementBirdsCollided();

	void IncrementBuffsDeployed();
	void IncrementBuffMagnetsUsed();
	void IncrementBuffShieldsUsed();
	void IncrementBuffShotsUsed();

	void Evaluate();

public:
	void GetIndividualPoints(std::vector<Points>& points) const;
	float GetTotalPoints() const;

	int GetLivesLeft() const;
	int GetLivesUsed() const;
	
	int GetBirdsKilled() const;

	int GetBuffMagnetsUsed() const;
	int GetBuffShieldsUsed() const;
	int GetBuffShotsUsed() const;
	
	int GetAchievedStars() const;
	bool IsLevelAchieved() const;
	
private:
	void AddPoints(const std::string& text, float amount);
};

#endif
