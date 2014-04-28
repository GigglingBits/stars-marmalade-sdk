#include "AnimSprite.h"
#include "IwDebug.h"
#include "Debug.h"

AnimSprite::AnimSprite() {
	m_xConfineTransform.SetIdentity();
	m_xWorldTransform.SetIdentity();
}

void AnimSprite::SetPosition(const CIwFVec2& pos) {
	m_xWorldTransform.SetTrans(pos);
}

void AnimSprite::SetRotation(float angle) {
	CIwFVec2 t = m_xWorldTransform.GetTrans(); // preserve center
	m_xWorldTransform.SetRot(angle);
	m_xWorldTransform.SetTrans(t);
}

void AnimSprite::TransformToWorld(CIwFVec2 v[], int c) {
	for (int i = 0; i < c; i++) {
		v[i] = m_xConfineTransform.TransformVec(v[i]);
		v[i] = m_xWorldTransform.TransformVec(v[i]);
	}
}

void AnimSprite::ConfineAnimation(CIwFVec2 verts[], int vertcount) {
	if (vertcount < 1) {
		// scale = 0.01f;
		// center offset = 0.0f;
		return;
	}
	
	// find shape's AABB
	CIwFVec2 ll(verts[0]);
	CIwFVec2 ur(ll);
	for (int i = 1; i < vertcount; i++) {
		GrowAABB(ll, ur, verts[i]);
	}
	
	// configure transformation matrix for confining the skeleton
	// in the shape's AABB
	float scale = ContainAABB(GetAABBLL(), GetAABBUR(), ll, ur);
	CIwFVec2 offset = OffsetAABB(GetAABBLL() * scale, GetAABBUR() * scale, ll, ur);
	m_xConfineTransform.Scale(scale);
	m_xConfineTransform.SetTrans(offset);
}

float AnimSprite::ContainAABB(const CIwFVec2& ll1, const CIwFVec2& ur1, const CIwFVec2& ll2, const CIwFVec2& ur2) {
	// aabb 1 is the content
	// aabb 2 is the container
	// return value is the scaleing factor for the content so that it fits into the container
	return std::min<float>((ur2.x - ll2.x)/(ur1.x - ll1.x), (ur2.x - ll2.x)/(ur1.x - ll1.x));
}

CIwFVec2 AnimSprite::OffsetAABB(const CIwFVec2& ll1, const CIwFVec2& ur1, const CIwFVec2& ll2, const CIwFVec2& ur2) {
	// aabb 1 is the off aabb
	// aabb 2 is the reference aabb
	// return value is the distance from the origin of the off aabb to the origin of the reference aabb, when their centers are aligned
	CIwFVec2 center1(((ur1 - ll1)/2.0f) + ll1);
	CIwFVec2 center2(((ur2 - ll2)/2.0f) + ll2);
	return center2 - center1;
}

CIwTexture* AnimSprite::GetStreams(int length, CIwFVec2 xys[], CIwFVec2 uvs[], uint32 cols[]) {
	CIwTexture* texture = SpineAnimation::GetStreams(length, xys, uvs, cols);
	TransformToWorld(xys, length);
	return texture;
}

#ifdef IW_DEBUG
void AnimSprite::GetDebugSkeletonBoundigBox(CIwFVec2 bb[4]) {
	SpineAnimation::GetDebugSkeletonBoundigBox(bb);
	TransformToWorld(bb, 4);
}

void AnimSprite::GetDebugAnimationOrigin(CIwFVec2 area[4]) {
	float extent = (GetAABBUR() - GetAABBLL()).GetLength() / 50.0f;
	CIwFVec2 t = m_xConfineTransform.GetInverse().GetTrans();
	area[0] = CIwFVec2(-extent, 0.0f) + t;
	area[1] = CIwFVec2(0.0f, -extent) + t;a
	area[2] = CIwFVec2(extent, 0.0f) + t;
	area[3] = CIwFVec2(0.0f, extent) + t;
	TransformToWorld(area, 4);
}

void AnimSprite::GetDebugSkeletonOrigin(CIwFVec2 area[4]) {
	SpineAnimation::GetDebugSkeletonOrigin(area);
	TransformToWorld(area, 4);
}
#endif

