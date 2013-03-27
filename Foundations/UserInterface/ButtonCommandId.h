#ifndef __BUTTONCOMMANDID_H__
#define __BUTTONCOMMANDID_H__

enum ButtonCommandId {
	eButtonCommandIdNone			= 0,

	//// game ineraction
	//eButtonCommandIdUp			= 1,
	//eButtonCommandIdDown			= 2,
	//eButtonCommandIdLeft			= 3,
	//eButtonCommandIdRight			= 4,

	// game actions
	eButtonCommandIdSetHeroFocus,

	// application navigation
	eButtonCommandIdOpenLevelMenu,
	eButtonCommandIdOpenLevel,
	eButtonCommandIdRestartLevel,
	eButtonCommandIdNextLevel,
	eButtonCommandIdNextWorld,
	eButtonCommandIdPreviousWorld,

	// application interaction
	eButtonCommandIdSocial,
	eButtonCommandIdAchievements,
	eButtonCommandIdLeaderboards,
	eButtonCommandIdOptions,
	eButtonCommandIdSoundFx,
	eButtonCommandIdMusic,
	eButtonCommandIdHelp,
	eButtonCommandIdCredits,
	eButtonCommandIdToggleHud,
	eButtonCommandIdReload,
};

#endif
