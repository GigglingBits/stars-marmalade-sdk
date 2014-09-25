#include "AnimTexture.h"
#include "IwDebug.h"
#include "Debug.h"

#include <cmath>

AnimTexture::AnimTexture() {
	m_xConfineTransform.SetIdentity();
	m_xWorldTransform.SetIdentity();
}

void AnimTexture::SetAll(CIwFVec2 verts[], int vertcount) {
	IW_CALLSTACK_SELF;

	IwAssertMsg(MYAPP, vertcount = 4, ("Failed to set transformations. This function only supports bounding boxes (4 vertices). Identity transformation will be set."));
	if (vertcount <= 0) {
		return;
	}

	// determine rotation and position
	CIwFVec2 v(verts[1] - verts[0]);
	float rotation = atan2f(v.y, v.x);
	CIwFVec2& position = verts[0];
	m_xWorldTransform.SetIdentity();
	m_xWorldTransform.SetRot(rotation);
	m_xWorldTransform.SetTrans(position);
	
	// normalize vertices
	CIwFMat2D normt = m_xWorldTransform.GetInverse();
	CIwFVec2* normv = new CIwFVec2[vertcount];
	for (int i = 0; i < vertcount; i++) {
		normv[i] = normt.TransformVec(verts[i]);
	}
	
	// confine to normalized
	SetShape(normv, vertcount);
	
	// cleanup
	delete [] normv;
}

void AnimTexture::SetPosition(const CIwFVec2& pos) {
	m_xWorldTransform.SetTrans(pos);
}

void AnimTexture::SetRotation(float angle) {
	CIwFVec2 t = m_xWorldTransform.GetTrans(); // preserve center
	m_xWorldTransform.SetRot(angle);
	m_xWorldTransform.SetTrans(t);
}

void AnimTexture::TransformToWorld(CIwFVec2 v[], int c) {
	IW_CALLSTACK_SELF;
	
	for (int i = 0; i < c; i++) {
		IwAssertMsg(MYAPP, !isnanf(v[i].x) && !isnanf(v[i].y), ("Undefined value in coordinate (%f / %f)", v[i].x, v[i].y));
		v[i] = m_xConfineTransform.TransformVec(v[i]);
		v[i] = m_xWorldTransform.TransformVec(v[i]);
		IwAssertMsg(MYAPP, !isnanf(v[i].x) && !isnanf(v[i].y), ("Undefined value in coordinate (%f / %f)", v[i].x, v[i].y));
		IwAssertMsg(MYAPP, v[i].x < 10000.0f || v[i].y < 10000.0f || v[i].GetLength() < 10000.0f, ("Odd looking coordinates at %i (%f / %f). This may indicate an error.", i, v[i].x, v[i].y));
	}
}

void AnimTexture::SetShape(CIwFVec2 verts[], int vertcount) {
	IW_CALLSTACK_SELF;
	
	IwAssertMsg(MYAPP, vertcount = 4, ("Failed to set transformations. For determining the shape, at least 1 vertex is needed."));
	if (vertcount < 1) {
		m_xConfineTransform.Scale(0.01f);
		m_xConfineTransform.SetTrans(CIwFVec2::g_Zero);
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
	m_xConfineTransform.SetIdentity();
	m_xConfineTransform.Scale(scale);
	m_xConfineTransform.SetTrans(offset);
}

float AnimTexture::ContainAABB(const CIwFVec2& ll1, const CIwFVec2& ur1, const CIwFVec2& ll2, const CIwFVec2& ur2) {
	// aabb 1 is the content
	// aabb 2 is the container
	// return value is the scaleing factor for the content so that it fits into the container
	return std::min<float>((ur2.x - ll2.x)/(ur1.x - ll1.x), (ur2.x - ll2.x)/(ur1.x - ll1.x));
}

CIwFVec2 AnimTexture::OffsetAABB(const CIwFVec2& ll1, const CIwFVec2& ur1, const CIwFVec2& ll2, const CIwFVec2& ur2) {
	// aabb 1 is the off aabb
	// aabb 2 is the reference aabb
	// return value is the distance from the origin of the off aabb to the origin of the reference aabb, when their centers are aligned
	CIwFVec2 center1(((ur1 - ll1)/2.0f) + ll1);
	CIwFVec2 center2(((ur2 - ll2)/2.0f) + ll2);
	return center2 - center1;
}

int AnimTexture::GetVertexCount() {
	return SpineAnimation::GetVertexCount();
}

CIwTexture* AnimTexture::GetStreams(int length, CIwFVec2 xys[], CIwFVec2 uvs[], uint32 cols[]) {
	CIwTexture* texture = SpineAnimation::GetStreams(length, xys, uvs, cols);
	TransformToWorld(xys, length);
	return texture;
}

#ifdef IW_DEBUG
void AnimTexture::GetDebugSkeletonBoundigBox(CIwFVec2 bb[4]) {
	SpineAnimation::GetDebugSkeletonBoundigBox(bb);
	TransformToWorld(bb, 4);
}

void AnimTexture::GetDebugAnimationOrigin(CIwFVec2 area[4]) {
	float extent = (GetAABBUR() - GetAABBLL()).GetLength() / 50.0f;
	CIwFVec2 t = m_xConfineTransform.GetInverse().GetTrans();
	area[0] = CIwFVec2(-extent, 0.0f) + t;
	area[1] = CIwFVec2(0.0f, -extent) + t;
	area[2] = CIwFVec2(extent, 0.0f) + t;
	area[3] = CIwFVec2(0.0f, extent) + t;
	TransformToWorld(area, 4);
}

void AnimTexture::GetDebugSkeletonOrigin(CIwFVec2 area[4]) {
	SpineAnimation::GetDebugSkeletonOrigin(area);
	TransformToWorld(area, 4);
}
#endif

