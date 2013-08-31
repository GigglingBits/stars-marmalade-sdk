#include "DustCounter.h"

DustCounter::DustCounter(float maxdust)
: m_fMaxDust(maxdust), m_fCollectedDust(0.0f) {
	ClearDustQueue();
}

void DustCounter::EnqueueDust(float dustamount) {
	m_fQueuedAmount += dustamount;
	m_iQueuedCount++;
}

void DustCounter::CommitDustQueue() {
	m_fCollectedDust += m_fQueuedAmount;
	ClearDustQueue();
}

void DustCounter::ClearDustQueue() {
	m_iQueuedCount = 0;
	m_fQueuedAmount = 0.0f;
}

int DustCounter::GetQueuedDustCount() {
	return m_iQueuedCount;
}

float DustCounter::GetQueuedDustAmount() {
	return m_fQueuedAmount;
}

float DustCounter::GetMaxDustAmount() {
	return m_fMaxDust;
}

float DustCounter::GetCollectedDustAmount() {
	return m_fCollectedDust;
}

float DustCounter::GetDustFillPercent() {
	return m_fCollectedDust / m_fMaxDust;
}
