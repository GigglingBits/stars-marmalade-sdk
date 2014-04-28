#ifndef __ANIMSPRITE_H__
#define __ANIMSPRITE_H__

#include "SpineAnimation.h"

class AnimSprite : public SpineAnimation {
private:
	CIwFMat2D m_xConfineTransform;	// transformation for confining the skeleton within a given shape
	CIwFMat2D m_xWorldTransform;	// transformation for placing the confined animation within a world

public:
	AnimSprite();
	
	void ConfineAnimation(CIwFVec2 verts[], int vertcount);
	
	void SetPosition(const CIwFVec2& pos);
	void SetRotation(float angle);

	CIwTexture* GetStreams(int length, CIwFVec2 xys[], CIwFVec2 uvs[], uint32 cols[]);

#ifdef IW_DEBUG
	void GetDebugAnimationOrigin(CIwFVec2 area[4]);
	void GetDebugSkeletonOrigin(CIwFVec2 area[4]);
	void GetDebugSkeletonBoundigBox(CIwFVec2 bb[4]);
#endif

private:
	void TransformToWorld(CIwFVec2 v[], int c);

	float ContainAABB(const CIwFVec2& ll1, const CIwFVec2& ur1, const CIwFVec2& ll2, const CIwFVec2& ur2);
	CIwFVec2 OffsetAABB(const CIwFVec2& ll1, const CIwFVec2& ur1, const CIwFVec2& ll2, const CIwFVec2& ur2);
};

#endif
