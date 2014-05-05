#ifndef __ANIMTEXTURE_H__
#define __ANIMTEXTURE_H__

#include "SpineAnimation.h"

class AnimTexture : public SpineAnimation {
private:
	CIwFMat2D m_xConfineTransform;	// transformation for confining the skeleton within a given shape
	CIwFMat2D m_xWorldTransform;	// transformation for placing the confined animation within a world

public:
	AnimTexture();
	
	/**
	 Derives the rotation, position and size all at once.
	 The rotation and the position are derived from the first 2 vertices.
	 The first vertex will be used as origin of the shape.
	 This method is useful for absolute positioning of the animation within the world,
	 i.e. if the rotation and positioning is handled outside such as in a physics package.
	 In that situation, the vertices would be supplied in world space before rendering.
	 */
	void SetAll(CIwFVec2 verts[], int vertcount);


	/**
	 Derives the size and origin.
	 The origin of the vertex is used as is.
	 This method is useful when the positioning and rotation is explicitly set by the game.
	 In that situation the vertices would be supplied in model space when initializing the sprite.
	 And only rotation and position would be updated using SetRotation and SetPosition before rendering.
	 */
	void SetShape(CIwFVec2 verts[], int vertcount);
	void SetPosition(const CIwFVec2& pos);
	void SetRotation(float angle);

	int GetVertexCount();
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
