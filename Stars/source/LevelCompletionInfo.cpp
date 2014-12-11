#include "LevelCompletionInfo.h"
#include "IwDebug.h"

LevelCompletionInfo::LevelCompletionInfo() :
m_fDustFillMax(1.0f),
m_fDustFillAmount(0.0f),
m_iPathsStarted(0),
m_iPathsAborted(0),
m_iNuggetsDeployed(0),
m_iNuggetsColleted(0),
m_iBirdsDeployed(0),
m_iBirdsKilled(0),
m_iBirdsCollided(0),
m_bEvaluated(false),
m_fTotalPoints(0.0f),
m_iLifesGranted(0) {
}

void LevelCompletionInfo::SetDustFillMax(float f) {
	IwAssert(MYAPP, !m_bEvaluated);
	m_fDustFillMax = f;
}

void LevelCompletionInfo::SetDustFillAmount(float f) {
	IwAssert(MYAPP, !m_bEvaluated);
	m_fDustFillAmount = f;
}

void LevelCompletionInfo::SetNumberOfLifesLeft(int lives) {
	m_iLifesLeft = lives;
}

void LevelCompletionInfo::SetNumberOfLifesGranted(int lives) {
	m_iLifesGranted = lives;
}

void LevelCompletionInfo::IncrementBuffsDeployed() {
	IwAssert(MYAPP, !m_bEvaluated);
	m_iBuffsUsed++;
}

void LevelCompletionInfo::IncrementBuffsUsed() {
	IwAssert(MYAPP, !m_bEvaluated);
	m_iBuffsDeployed++;
}

void LevelCompletionInfo::IncrementPathsStarted() {
	IwAssert(MYAPP, !m_bEvaluated);
	m_iPathsStarted++;
}

void LevelCompletionInfo::IncrementPathsAborted() {
	IwAssert(MYAPP, !m_bEvaluated);
	m_iPathsAborted++;
}

void LevelCompletionInfo::IncrementNuggetsDeployed() {
	IwAssert(MYAPP, !m_bEvaluated);
	m_iNuggetsDeployed++;
}

void LevelCompletionInfo::IncrementNuggetsCollected() {
	IwAssert(MYAPP, !m_bEvaluated);
	m_iNuggetsColleted++;
}

void LevelCompletionInfo::IncrementBirdsKilled() {
	IwAssert(MYAPP, !m_bEvaluated);
	m_iBirdsKilled++;
}

void LevelCompletionInfo::IncrementBirdsDeployed() {
	IwAssert(MYAPP, !m_bEvaluated);
	m_iBirdsDeployed++;
}

void LevelCompletionInfo::IncrementBirdsCollided() {
	IwAssert(MYAPP, !m_bEvaluated);
	m_iBirdsCollided++;
}

void LevelCompletionInfo::AddPoints(const std::string& text, float amount) {
	m_fTotalPoints += amount;

	Points points;
	points.Text = text;
	points.Amount = amount;
	m_xIndividualPoints.push_back(points);
}

void LevelCompletionInfo::Evaluate() {
	IwAssert(MYAPP, !m_bEvaluated);
	if (m_fDustFillAmount > 0.0001f) {
		AddPoints("Dust collected", m_fDustFillAmount);
		if (m_iNuggetsDeployed > 0 && m_iNuggetsDeployed == m_iNuggetsColleted) {
			AddPoints("All nuggets collected", m_fDustFillAmount / 5.0f);
		}
		if (m_iBirdsDeployed > 0 && m_iBirdsCollided <= 0) {
			AddPoints("All enemies dodged", m_fDustFillAmount / 5.0f);
		}
		if (m_iBirdsDeployed > 0 && m_iBirdsDeployed == m_iBirdsKilled) {
			AddPoints("All enemies killed", m_fDustFillAmount / 2.0f);
		}
		if (m_iBuffsDeployed > 0 && m_iBuffsUsed == 0) {
			AddPoints("No buffs used", m_fDustFillAmount / 10.0f);
		}
	}
	
	m_bEvaluated = true;
}

void LevelCompletionInfo::GetIndividualPoints(std::vector<Points>& points) const {
	IwAssert(MYAPP, m_bEvaluated);
	points = m_xIndividualPoints;
}

float LevelCompletionInfo::GetTotalPoints() const {
	IwAssert(MYAPP, m_bEvaluated);
	return m_fTotalPoints;
}

int LevelCompletionInfo::GetLivesLeft() const {
	IwAssert(MYAPP, m_bEvaluated);
	return m_iLifesLeft;
}

int LevelCompletionInfo::GetBirdsKilled() const {
	IwAssert(MYAPP, m_bEvaluated);
	return m_iBirdsKilled;
}

int LevelCompletionInfo::GetLivesUsed() const {
	IwAssert(MYAPP, m_bEvaluated);
	return m_iLifesGranted - m_iLifesLeft;
}

int LevelCompletionInfo::GetAchievedStars() const {
	IwAssert(MYAPP, m_bEvaluated);
	
	float points = m_fTotalPoints;
	float maxpoints = m_fDustFillMax;

	if (points >= maxpoints) return 3;
	if (points >= maxpoints * 0.8f) return 2;
	if (points >= maxpoints * 0.5f) return 1;
	
	return 0;
}

bool LevelCompletionInfo::IsLevelAchieved() const {
	return m_bEvaluated && GetAchievedStars() > 0;
}
