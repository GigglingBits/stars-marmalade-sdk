#include "Page.h"
#include "FactoryManager.h"
#include "Debug.h"

Page::Page(const std::string& resgrpname, const std::string& musicfilename) : Window(), m_eCompletionState(eInProgress), m_sResGrpName(resgrpname), m_sMusicFileName(musicfilename) {
	SetRenderingLayer(Renderer::eRenderingLayerBackground);
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

const std::string& Page::GetMusicFileName() {
	return m_sMusicFileName;
}
