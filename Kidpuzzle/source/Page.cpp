#include "Page.h"
#include "Debug.h"
#include "DeviceInfo.h"

Page::Page() 
	: Window(), m_eCompletionState(eInProgress) {
}

bool Page::IsCompleted() {
	return m_eCompletionState != eInProgress;
}

Page::CompletionState Page::GetCompletionState() {
	return m_eCompletionState;
}

void Page::SetCompletionState(CompletionState state) {
	m_eCompletionState = state;
}
