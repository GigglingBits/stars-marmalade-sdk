#ifndef __BUTTONCOMMANDID_H__
#define __BUTTONCOMMANDID_H__

enum ButtonCommandId {
	eButtonCommandIdNone			= 0,

	//// game ineraction
	//eButtonCommandIdUp			= 1,
	//eButtonCommandIdDown			= 2,
	//eButtonCommandIdLeft			= 3,
	//eButtonCommandIdRight			= 4,

	// application navigation
	eButtonCommandIdOpenTitleMenu,
	eButtonCommandIdOpenLevelMenu,
	
    eButtonCommandIdOpenWorld,
    eButtonCommandIdOpenLevel,
	eButtonCommandIdOpenNextLevel,
	eButtonCommandIdRestartLevel,
	eButtonCommandIdQuitLevel,

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
	
	// star game actions
	eButtonCommandIdStarBlock,
	eButtonCommandIdStarAttack,
	
	// gnome game actions
	eButtonCommandIdSetHeroFocus,
};

#endif
