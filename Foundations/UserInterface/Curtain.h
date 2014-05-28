#ifndef __CURTAIN_H__
#define __CURTAIN_H__

#include "Renderable.h"

class Curtain : public Renderable {
private:
	enum State {
		eStateOpening,
		eStateOpen,
		eStateClosing,
		eStateClosed,
	};
	State m_eState;

	int m_uiFadeTime;
	int m_iCountdown;
	uint32 m_uiAlpha;

public:
	Curtain(uint32 alpha = 0xff, uint32 fadetime = 250);

	bool IsClosed();
	bool IsOpen();

	void Close();
	void Open();
	
	void SetAlpha(uint32 alpha);
	void SetFadeTime(uint32 fadetime);

protected:
	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);
};

#endif
