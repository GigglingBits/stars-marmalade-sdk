#include "Leaderboards.h"

#include "LeaderboardsNull.h"
#include "LeaderboardsApple.h"
#include "LeaderboardsGoogle.h"

#include "IwDebug.h"
#include "Debug.h"

Leaderboards* Leaderboards::s_pxInstance = NULL;

void Leaderboards::Initialize() {
	IW_CALLSTACK_SELF;
	
	if (!s_pxInstance) {
		s_pxInstance = new LeaderboardsApple();
		if (s_pxInstance->IsAvailable()) {
			return;
		}
		delete s_pxInstance;
		
		s_pxInstance = new LeaderboardsGoogle();
		if (s_pxInstance->IsAvailable()) {
			return;
		}
		delete s_pxInstance;
		
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
