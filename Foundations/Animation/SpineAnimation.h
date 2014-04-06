#ifndef __SPINEANIMATION_H__
#define __SPINEANIMATION_H__

#include <string>
#include "Renderable.h"

#include "spine/spine.h"

class SpineAnimation : public Renderable {
private:
	spAtlas* m_pxAtlas;
	spSkeletonJson* m_pxSkeletonJson;
	spSkeletonData* m_pxSkeletonData;
	spSkeleton* m_pxSkeleton;
	spAnimation* m_pxAnimation;
	float m_fAnimationTime;

public:
	SpineAnimation();
	~SpineAnimation();
	
	void Load(const std::string& atlasfile, const std::string& jsonfile, float scale = 1.0f);
	void SetAnimation(const std::string& name);

	void SetPosition(const CIwFVec2& pos);
	void SetScale(float scale);
	
protected:
	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);
	
private:
	void LoadSkeleton(const std::string& atlasfile, const std::string& jsonfile, float scale);
	void DestroySkeleton();
	void UpdateSkeleton(const FrameData& frame);
	void RenderSkeleton(Renderer& renderer);
	void RenderSkeletonSlot(Renderer& renderer, spSlot* slot, spRegionAttachment* att);
	void ExtractUVs(CIwFVec2 uvs[4], spAtlasRegion* atlasreg);
	void ExtractColours(uint32 cols[4], spSlot* slot);
};

#endif
