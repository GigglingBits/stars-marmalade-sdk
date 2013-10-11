#ifndef __PAGE_H__
#define __PAGE_H__

#include "Window.h"

class Page : public Window {
public: 
	enum CompletionState {
		eInProgress,
		eCompleted,
		eFailed
	};

private:
    CompletionState m_eCompletionState;

	std::string m_sResGrpName;
	
public:
	Page(const std::string& resgrpname);

	bool IsCompleted();
	CompletionState GetCompletionState();

	const std::string& GetResourceGroupName();
	
protected:
	void SetCompletionState(CompletionState state);
};

#endif
