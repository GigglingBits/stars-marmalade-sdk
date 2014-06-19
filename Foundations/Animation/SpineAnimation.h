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
	
	CIwFVec2 m_xSkeletonAABBLL;		// AABB of the original skeleton in starting pose (lower left corner)
	CIwFVec2 m_xSkeletonAABBUR;		// AABB of the original skeleton in starting pose (upper right corner)
	
public:
	SpineAnimation();
	virtual ~SpineAnimation();
	
	bool Load(const std::string& resourceid);

	bool SetSkin(const std::string& name);
	bool ConstainsSkin(const std::string& name);
	
	bool SetAnimation(const std::string& name, float position = 0.0f);
	bool ConstainsAnimation(const std::string& name);
	
	bool GetFlipX();
	void SetFlipX(bool flip);

	bool GetFlipY();
	void SetFlipY(bool flip);
	
	virtual void Update(uint32 timestep);
	
#ifdef IW_DEBUG
	void GetDebugSkeletonOrigin(CIwFVec2 area[4]);
	void GetDebugSkeletonBoundigBox(CIwFVec2 bb[4]);
#endif
	
private:
	bool LoadSkeleton(const std::string& atlasdata, const std::string& jsondata);
	void DestroySkeleton();

	CIwTexture* ExtractStreams(spSlot* slot, spRegionAttachment* att, int length, CIwFVec2 xys[], CIwFVec2 uvs[], uint32 cols[]);
	void ExtractUVs(CIwFVec2 uvs[4], spAtlasRegion* atlasreg);
	void ExtractColours(uint32 cols[4], spSlot* slot);
	bool ExtractAABB(CIwFVec2& ll, CIwFVec2& ur);
	
protected:
	int GetVertexCount();
	CIwTexture* GetStreams(int length, CIwFVec2 xys[], CIwFVec2 uvs[], uint32 cols[]);

	void RebuildGeometry();
	
	const CIwFVec2& GetAABBLL() const;
	const CIwFVec2& GetAABBUR() const;
	void GrowAABB(CIwFVec2& ll, CIwFVec2& ur, const CIwFVec2& point) const;
};

#endif
