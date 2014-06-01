#include "LevelCompletionInfo.h"

LevelCompletionInfo::LevelCompletionInfo() :
m_fDustFillMax(1.0f),
m_fDustFillAmount(0.0f),
m_iPathsStarted(0),
m_iPathsAborted(0),
m_iNuggetsColleted(0),
m_iNuggetsMissed(0) {
}

bool LevelCompletionInfo::IsCleared() const {
	return GetDustAchievement();
}

void LevelCompletionInfo::SetDustFillMax(float f) {
	m_fDustFillMax = f;
}

void LevelCompletionInfo::SetDustFillAmount(float f) {
	m_fDustFillAmount = f;
}

void LevelCompletionInfo::IncrementPathsStarted() {
	m_iPathsStarted++;
}

void LevelCompletionInfo::IncrementPathsAborted() {
	m_iPathsAborted++;
}

void LevelCompletionInfo::IncrementNuggetsCollected() {
	m_iNuggetsColleted++;
}

void LevelCompletionInfo::IncrementNuggetsMissed() {
	m_iNuggetsMissed++;
}

float LevelCompletionInfo::GetDustAmount() const {
	return m_fDustFillAmount;
}

bool LevelCompletionInfo::GetDustAchievement() const {
	return m_fDustFillAmount >= m_fDustFillMax;
}

int LevelCompletionInfo::GetPathsStarted() const {
	return m_iPathsStarted;
}

int LevelCompletionInfo::GetNuggetsCollected() const {
	return m_iNuggetsColleted;
}
