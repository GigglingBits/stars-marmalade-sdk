#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "s3e.h"
#include "InputManager.h"
#include "Texture.h"
#include "Renderer.h"
#include "Renderable.h"

class Button : public Renderable {
private:
	InputManager::VirtualButton m_xButton;
	VertexStreamScreen m_xButtonVerts;

	Texture* m_pxTexture;

	std::string m_sText;
	uint32 m_uiTextCol;
	Renderer::FontType m_eFontType;

	bool m_bHideWhenDisabled;
	bool m_bShadedWhenPressed;
	bool m_bDown;

public:
	Button(ButtonCommandId cmdid, s3eKey key, long userdata = 0);
	virtual ~Button();

	void SetEnabled(bool enabled);
	bool IsEnabled();

	void SetUserData(long userdata);
	long GetUserData() const;

	void SetText(const std::string& text, uint32 colour = 0xff222222, Renderer::FontType font = Renderer::eFontTypeNormal);

	void SetTexture(Texture* texture);
	void SetTextureFrame(const std::string& frame);

	const CIwRect& GetPosition();
	void SetPosition(const CIwRect& rect);

	void SetHideWhenDisabled(bool hide);
	void SetShadedWhenPressed(bool shaded);

protected:
	bool IsPressed();
	bool IsTextureLoaded();
	
	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);

	virtual void OnTextureLoaded(Texture& texture);
	virtual void OnStateChanged(bool enabled, bool pressed);

private:
	void PressedEventHandler(const InputManager::VirtualButton& sender, const InputManager::VirtualButton::EventArgs& args);
	void ReleasedEventHandler(const InputManager::VirtualButton& sender, const InputManager::VirtualButton::EventArgs& args);

public:
	struct EventArgs {
		ButtonCommandId id;
		long userdata;
		bool handled;
	};
	MulticastEvent<Button, EventArgs> PressedEvent;
	MulticastEvent<Button, EventArgs> ReleasedEvent;
};

#endif
