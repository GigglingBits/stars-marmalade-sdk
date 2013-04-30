#ifndef _CONTROLLER_H_ 
#define _CONTROLLER_H_ 

//--------------------------------------------------------------------------
// Controller for moving the pages on the screen
//--------------------------------------------------------------------------
#include "IwGeomVec2.h"
#include "Spectator.h"
#include "Toolbar.h"
#include "Directions.h"
#include "BaseBrick.h"
#include "Welcome.h"

class Controller : public BaseBrick {
private:
	PageMap m_xPageMap;
	Toolbar m_xAppToolbar;
	Toolbar m_xPageToolbar;

	Welcome* m_pxWelcome;
	Spectator* m_pxSpectator;

	bool m_bIsTouchMoving;
	bool m_bIsAnimating;

	CIwSVec2 m_xOffset;
	
public:
	Controller();
	~Controller();

	void Initialize();

	// user interactions
	void Touch(const CIwSVec2& touch);
	void TouchMove(const CIwSVec2& touch, const CIwSVec2& move);
	void Shake();

	// application operations
	bool OnUpdate(bool first);
	void OnRender(const CIwSVec2& screensize, const CIwRect& itemregion);
	
	void Move(Direction dir);

	void BtnSoundPressed();
	void BtnVoicePressed();
	void BtnTextPressed();
	void BtnHelpPressed();

private:
	void StartAnimation();
	void StopAnimation();
	bool IsHelpVisible();
	bool IsAnimating();
	void AnimateOffset(CIwSVec2& offset);
	
	void PlaySound(std::string soundfile, bool interrupt);

	bool HasOffset();
	void DoPageChange();
	void UpdateToolbar();

	void ShowText(bool show);
};

#endif
