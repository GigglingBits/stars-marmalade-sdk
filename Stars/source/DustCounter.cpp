#include <string>

#include "DustCounter.h"
#include "Debug.h"
#include "IwDebug.h"

DustCounter::DustCounter(float maxdust)
: m_fMaxDust(maxdust), m_fCollectedDust(0.0f), m_bIsQueueing(true) {
	RollbackDustQueue();
}

void DustCounter::AddDust(float dustamount) {
	if (m_bIsQueueing) {
		m_fQueuedAmount += dustamount;
		m_iQueuedCount++;
	} else {
		m_fCollectedDust += dustamount;
	}
}

bool DustCounter::IsQueueing() {
	return m_bIsQueueing;
}

void DustCounter::BeginDustQueue() {
	IW_CALLSTACK_SELF;
	IwAssert(MYAPP, !m_bIsQueueing);
	m_bIsQueueing = true;
}

void DustCounter::CommitDustQueue() {
	IW_CALLSTACK_SELF;
	IwAssert(MYAPP, m_bIsQueueing);
	m_fCollectedDust += m_fQueuedAmount;
	RollbackDustQueue();
}

void DustCounter::RollbackDustQueue() {
	IW_CALLSTACK_SELF;
	IwAssert(MYAPP, m_bIsQueueing);
	m_iQueuedCount = 0;
	m_fQueuedAmount = 0.0f;
	m_bIsQueueing = false;
}

int DustCounter::GetQueuedDustCount() {
	return m_iQueuedCount;
}

float DustCounter::GetQueuedDustAmount() {
	return m_fQueuedAmount;
}

float DustCounter::GetQueuedDustPercent() {
	return std::min<float>(1.0f, m_fQueuedAmount / m_fMaxDust);
}

float DustCounter::GetMaxDustAmount() {
	return m_fMaxDust;
}

float DustCounter::GetCollectedDustAmount() {
	return m_fCollectedDust;
}

float DustCounter::GetDustFillPercent() {
	return std::min<float>(1.0f, m_fCollectedDust / m_fMaxDust);
}
