#ifndef __PAGE_H__
#define __PAGE_H__

#include "Main.h"
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
	std::string m_sMusicFileName;
	
public:
	Page(const std::string& resgrpname, const std::string& musicfilename);

	bool IsCompleted();
	CompletionState GetCompletionState();

	const std::string& GetResourceGroupName();
	const std::string& GetMusicFileName();
	
protected:
	void SetCompletionState(CompletionState state);
};

#endif
