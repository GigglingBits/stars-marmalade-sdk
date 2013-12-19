#ifndef __LEVELPROGRESSBAR_H__
#define __LEVELPROGRESSBAR_H__

#include "Window.h"
#include "Texture.h"

class LevelProgressBar : public Window {
private:
	struct Icon {
		float progress;
		std::string textureid;
		Texture* texture;
		VertexStreamScreen* shape;
	};
	
private:
	float m_fMaxValue;
	float m_fProgress;

	VertexStreamScreen m_xBackgroundShape;
	Texture* m_pxBackground;

	VertexStreamScreen m_xStarShape;
	Texture* m_pxStar;

	typedef std::vector<Icon> Icons;
	Icons m_xIcons;
	
public:
	LevelProgressBar();
	virtual ~LevelProgressBar();

	void Initialize();
	
	void SetIcon(float progress, const std::string& textureid);
	void SetMax(float value);
	void SetProgress(float progress);

protected:
	void UpdateStarShapes();
	void UpdateStarShape(float progress, VertexStreamScreen& shape);

	float CalcNormalProgress(float value);
	
	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);
	virtual void OnDoLayout(const CIwSVec2& screensize);
};

#endif
