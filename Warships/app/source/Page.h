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

public:
	Page();

	bool IsCompleted();
	CompletionState GetCompletionState();

protected:
	void SetCompletionState(CompletionState state);
};

#endif
