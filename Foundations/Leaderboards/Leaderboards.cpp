#include "Leaderboards.h"

#include "LeaderboardsNull.h"
#include "LeaderboardsApple.h"
#include "LeaderboardsGoogle.h"

#include "IwDebug.h"
#include "Debug.h"

Leaderboards* Leaderboards::s_pxInstance = NULL;

void Leaderboards::Initialize(bool suppressgamecenter, bool suppressgoogleplay) {
	IW_CALLSTACK_SELF;

	// try Apple first
	if (!s_pxInstance && !suppressgamecenter) {
		s_pxInstance = new LeaderboardsApple();
		if (!s_pxInstance->IsAvailable()) {
			delete s_pxInstance;
			s_pxInstance = NULL;
		}
	}
	
	// Google next
	if (!s_pxInstance && !suppressgoogleplay) {
		s_pxInstance = new LeaderboardsGoogle();
		if (!s_pxInstance->IsAvailable()) {
			delete s_pxInstance;
			s_pxInstance = NULL;
		}
	}
	
	// NULL implementation, if nothing else works
	if (!s_pxInstance) {
		s_pxInstance = new LeaderboardsNull();
	}
}

void Leaderboards::Terminate() {
	if (s_pxInstance) {
		delete s_pxInstance;
		s_pxInstance = NULL;
	}
}

Leaderboards::Leaderboards() {
}

Leaderboards::~Leaderboards() {
}

Leaderboards& Leaderboards::GetInstance() {
	return *s_pxInstance;
}
