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
	
	CIwFMat2D m_xConfineTransform;	// transformation for confining the skeleton within a given shape
	CIwFMat2D m_xWorldTransform;	// transformation for placing the confined animation within a world
	
public:
	SpineAnimation();
	~SpineAnimation();
	
	void Load(const std::string& atlasfile, const std::string& jsonfile);
	void SetAnimation(const std::string& name);

	void ConfineAnimation(CIwFVec2 verts[], int vertcount);

	void SetPosition(const CIwFVec2& pos);
	void SetRotation(float angle);

	void Update(uint32 timestep);
	
	int GetVertexCount();
	CIwTexture* GetStreams(int length, CIwFVec2 xys[], CIwFVec2 uvs[], uint32 cols[]);

#ifdef IW_DEBUG
	void GetDebugAnimationOrigin(CIwFVec2 area[4]);
	void GetDebugSkeletonOrigin(CIwFVec2 area[4]);
	void GetDebugSkeletonBoundigBox(CIwFVec2 bb[4]);
#endif
	
private:
	void LoadSkeleton(const std::string& atlasfile, const std::string& jsonfile);
	void DestroySkeleton();

	CIwTexture* ExtractStreams(spSlot* slot, spRegionAttachment* att, int length, CIwFVec2 xys[], CIwFVec2 uvs[], uint32 cols[]);
	void ExtractUVs(CIwFVec2 uvs[4], spAtlasRegion* atlasreg);
	void ExtractColours(uint32 cols[4], spSlot* slot);
	
	bool ExtractLocalAABB(CIwFVec2& ll, CIwFVec2& ur);
	void GrowAABB(CIwFVec2& ll, CIwFVec2& ur, const CIwFVec2& point);
	float ContainAABB(const CIwFVec2& ll1, const CIwFVec2& ur1, const CIwFVec2& ll2, const CIwFVec2& ur2);
	CIwFVec2 OffsetAABB(const CIwFVec2& ll1, const CIwFVec2& ur1, const CIwFVec2& ll2, const CIwFVec2& ur2);
	
	void TransformToWorld(CIwFVec2 v[], int c);
};

#endif
