#ifndef __SPINEANIMATION_H__
#define __SPINEANIMATION_H__

#include <string>
#include "IwGeom.h"
#include "IwGx.h"
#include "spine/spine.h"

#define SPINEANIMATION_VERTS_PER_SLOT 4

class SpineAnimation {
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
	void SetRotation(float angle);

	void Update(uint32 timestep);
	
	int GetVertexCount();
	CIwTexture* GetStreams(int length, CIwFVec2 xys[], CIwFVec2 uvs[], uint32 cols[]);

private:
	CIwTexture* ExtractStreams(spSlot* slot, spRegionAttachment* att, int length, CIwFVec2 xys[], CIwFVec2 uvs[], uint32 cols[]);
	
	void LoadSkeleton(const std::string& atlasfile, const std::string& jsonfile, float scale);
	void DestroySkeleton();

	void ExtractUVs(CIwFVec2 uvs[4], spAtlasRegion* atlasreg);
	void ExtractColours(uint32 cols[4], spSlot* slot);
};

#endif
