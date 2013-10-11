#include "Page.h"
#include "Debug.h"
#include "DeviceInfo.h"

Page::Page(const std::string& resgrpname) : Window(), m_eCompletionState(eInProgress), m_sResGrpName(resgrpname) {
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

const std::string& Page::GetResourceGroupName() {
	return m_sResGrpName;
}
