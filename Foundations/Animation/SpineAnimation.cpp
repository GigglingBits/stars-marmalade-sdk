#include "SpineAnimation.h"
#include "IwDebug.h"
#include "Debug.h"

#include <spine/extension.h>

SpineAnimation::SpineAnimation() :
m_pxAtlas(NULL),
m_pxSkeletonJson(NULL),
m_pxSkeletonData(NULL),
m_pxSkeleton(NULL),
m_pxAnimation(NULL),
m_fAnimationTime(0.0f) {
	m_xOffset = CIwFVec2::g_Zero;
	m_xScale.SetIdentity();
	m_xRotation.SetIdentity();
	m_xTranslation.SetIdentity();
	m_xTransformation.SetIdentity();
}

SpineAnimation::~SpineAnimation() {
	DestroySkeleton();
}

void SpineAnimation::Load(const std::string& atlasfile, const std::string& jsonfile) {
	LoadSkeleton(atlasfile, jsonfile);
}

void SpineAnimation::ConfineShape(CIwFVec2 verts[], int vertcount) {
	if (vertcount < 1) {
		// scale = 0.01f;
		// center offset = 0,0f;
		return;
	}
		
	// find shape's AABB
	CIwFVec2 ll(verts[0]);
	CIwFVec2 ur(ll);
	for (int i = 1; i < vertcount; i++) {
		GrowAABB(ll, ur, verts[i]);
	}
	
	// find scale
	float scale = ContainAABB(m_xAABBLL, m_xAABBUR, ll, ur);
	m_xScale.SetIdentity();
	m_xScale.Scale(scale);
	
	// center offset
	m_xOffset = OffsetAABB(ll, ur, m_xAABBLL * scale, m_xAABBUR * scale);
	m_xRotation.SetIdentity();
	m_xRotation.SetRot(0.0f, m_xOffset);
	
	// combine all matrices
	UpdateTransformationMatrix();
}

float SpineAnimation::ContainAABB(const CIwFVec2& ll1, const CIwFVec2& ur1, const CIwFVec2& ll2, const CIwFVec2& ur2) {
	// aabb 1 is the content
	// aabb 2 is the container
	// return value is the scaleing factor for the content so that it fits into the container
	return std::min<float>((ur2.x - ll2.x)/(ur1.x - ll1.x), (ur2.x - ll2.x)/(ur1.x - ll1.x));
}

CIwFVec2 SpineAnimation::OffsetAABB(const CIwFVec2& ll1, const CIwFVec2& ur1, const CIwFVec2& ll2, const CIwFVec2& ur2) {
	// aabb 1 is the off aabb
	// aabb 2 is the reference aabb
	// return value is the distance from the origin of the off aabb to the origin of the reference aabb, when their centers are aligned
	CIwFVec2 center1(((ur1 - ll1)/2.0f) + ur1);
	CIwFVec2 center2(((ur2 - ll2)/2.0f) + ur2);
	return center2 - center1;
}

void SpineAnimation::SetAnimation(const std::string& name) {
	IW_CALLSTACK_SELF;
	IwAssertMsg(MYAPP, m_pxSkeletonData, ("Skeleton must be loaded before setting the animation"));
	if (!m_pxSkeletonData) {
		return;
	}
	
	if (m_pxAnimation && name.compare(m_pxAnimation->name) != 0) {
		spAnimation_dispose(m_pxAnimation);
		m_pxAnimation = NULL;
	}
	if (!m_pxAnimation) {
		m_pxAnimation = spSkeletonData_findAnimation(m_pxSkeletonData, name.c_str());
		IwAssertMsg(MYAPP, m_pxAnimation, ("Unable to find '%s' animation", name.c_str()));
	}
}

void SpineAnimation::LoadSkeleton(const std::string& atlasfile, const std::string& jsonfile) {
	IW_CALLSTACK_SELF;
	if ((m_pxAtlas = spAtlas_readAtlasFile(atlasfile.c_str()))) {
		if ((m_pxSkeletonJson = spSkeletonJson_create(m_pxAtlas))) {
			if ((m_pxSkeletonData = spSkeletonJson_readSkeletonDataFile(m_pxSkeletonJson, jsonfile.c_str()))) {
				if ((m_pxSkeleton = spSkeleton_create(m_pxSkeletonData))) {
					m_pxSkeleton->flipX = false;
					m_pxSkeleton->flipY = false;
					spSkeleton_setToSetupPose(m_pxSkeleton);
					spSkeleton_updateWorldTransform(m_pxSkeleton);
					ExtractLocalAABB(m_xAABBLL, m_xAABBUR);
				} else {
					IwAssertMsg(MYAPP, false, ("Unable to create generic skeleton"));
				}
			} else {
				IwAssertMsg(MYAPP, false, ("Unable to load skeleton data from: %s", jsonfile.c_str()));
			}
		} else {
			IwAssertMsg(MYAPP, false, ("Unable to create JSON skeleton"));
		}
	} else {
		IwAssertMsg(MYAPP, false, ("Unable to load atlas file: %s", atlasfile.c_str()));
	}
}

void SpineAnimation::DestroySkeleton() {
	if (m_pxSkeleton) {
		spSkeleton_dispose(m_pxSkeleton);
		m_pxSkeleton = NULL;
	}
	if (m_pxSkeletonData) {
		spSkeletonData_dispose(m_pxSkeletonData);
		m_pxSkeletonData = NULL;
	}
	if (m_pxSkeletonJson) {
		spSkeletonJson_dispose(m_pxSkeletonJson);
		m_pxSkeletonJson = NULL;
	}
	if (m_pxAtlas) {
		spAtlas_dispose(m_pxAtlas);
		m_pxAtlas = NULL;
	}
}

void SpineAnimation::Update(uint32 timestep) {
	float seconds = timestep / 1000.0f;
	
	// let skeleton know about time (why is this needed?)
	if (m_pxSkeleton) {
		spSkeleton_update(m_pxSkeleton, seconds);

		// perform actual animation
		if (m_pxAnimation) {
			spAnimation_apply(m_pxAnimation, m_pxSkeleton, m_fAnimationTime, m_fAnimationTime + seconds, 1, NULL, 0);
			spSkeleton_updateWorldTransform(m_pxSkeleton);
		}
	}
	
	// accumulate
	m_fAnimationTime += seconds;
}

int SpineAnimation::GetVertexCount() {
	return (m_pxSkeleton ? m_pxSkeleton->slotCount : 0) * SPINEANIMATION_VERTS_PER_SLOT;
}

CIwTexture* SpineAnimation::GetStreams(int length, CIwFVec2 xys[], CIwFVec2 uvs[], uint32 cols[]) {
	IW_CALLSTACK_SELF;
	if (!m_pxSkeleton) {
		return NULL;
	}

	CIwTexture* texture = NULL;
	for (int slotid = 0; slotid < m_pxSkeleton->slotCount; slotid++) {
		if (spSlot* slot = m_pxSkeleton->drawOrder[slotid]) {
			IwAssertMsg(MYAPP, 0 == slot->data->additiveBlending, ("Slot %i uses additive blending. Additive blending is not supported. Drawing errors may occur.", slotid));
			if (spAttachment* attachment = slot->attachment) {
				if (attachment->type == ATTACHMENT_REGION) {
					if (spRegionAttachment* regionAttachment = (spRegionAttachment*)attachment) {
						int cursor = SPINEANIMATION_VERTS_PER_SLOT * slotid;
						CIwTexture* tmp = ExtractStreams(slot, regionAttachment, SPINEANIMATION_VERTS_PER_SLOT, &xys[cursor], &uvs[cursor], &cols[cursor]);
						IwAssertMsg(MYAPP, tmp, ("Slot %i seems to have no texture. Drawing errors will occur.", slotid));
						IwAssertMsg(MYAPP, !texture || tmp == texture, ("Slot Not all slots seem to use the same texture. Multitexture animations are not supported. Drawing errors will occur."));
						texture = tmp;
					}
				}
			}
		}
	}
	return texture;
}

void SpineAnimation::SetPosition(const CIwFVec2& pos) {
	m_xTranslation.SetIdentity();
	m_xTranslation.SetTrans(pos + m_xOffset);
	UpdateTransformationMatrix();
}

void SpineAnimation::SetRotation(float angle) {
	m_xRotation.SetIdentity();
	m_xRotation.SetRot(angle, m_xOffset);
	UpdateTransformationMatrix();
}

void SpineAnimation::UpdateTransformationMatrix() {
	m_xTransformation = m_xRotation * m_xScale * m_xTranslation;
}

void SpineAnimation::TransformToWorld(CIwFVec2 v[], int c) {
	for (int i = 0; i < c; i++) {
		//v[i] = m_xRotation.TransformVec(v[i]);
		//v[i] = m_xScale.TransformVec(v[i]);
		//v[i] = m_xTranslation.TransformVec(v[i]);
		v[i] = m_xTransformation.TransformVec(v[i]);
	}
}

void SpineAnimation::GetBoundigBox(CIwFVec2 bb[4]) {
	// rescale the bounding box to match the skeleton
	bb[0].x = m_xAABBLL.x;
	bb[0].y = m_xAABBLL.y;
	bb[1].x = m_xAABBUR.x;
	bb[1].y = m_xAABBLL.y;
	bb[2].x = m_xAABBUR.x;
	bb[2].y = m_xAABBUR.y;
	bb[3].x = m_xAABBLL.x;
	bb[3].y = m_xAABBUR.y;
	
	TransformToWorld(bb, 4);
}

void SpineAnimation::GetDebugAnimationOrigin(CIwFVec2 area[4]) {
	float extent = (m_xAABBUR - m_xAABBLL).GetLength() / 10.0f;
	area[0] = CIwFVec2(-extent, 0.0f);
	area[1] = CIwFVec2(0.0f, -extent);
	area[2] = CIwFVec2(extent, 0.0f);
	area[3] = CIwFVec2(0.0f, extent);
	TransformToWorld(area, 4);
}

void SpineAnimation::GetDebugAnimationOffset(CIwFVec2 area[4]) {
	float extent = (m_xAABBUR - m_xAABBLL).GetLength() / 10.0f;
	area[0] = m_xOffset + CIwFVec2(-extent, 0.0f);
	area[1] = m_xOffset + CIwFVec2(0.0f, -extent);
	area[2] = m_xOffset + CIwFVec2(extent, 0.0f);
	area[3] = m_xOffset + CIwFVec2(0.0f, extent);
	TransformToWorld(area, 4);
}

bool SpineAnimation::ExtractLocalAABB(CIwFVec2& ll, CIwFVec2& ur) {
	IW_CALLSTACK_SELF;
	
	bool hasbounds = false;
	for (int slotid = 0; slotid < m_pxSkeleton->slotCount; slotid++) {
		if (spSlot* slot = m_pxSkeleton->drawOrder[slotid]) {
			if (spAttachment* attachment = slot->attachment) {
				if (attachment->type == ATTACHMENT_BOUNDING_BOX) {
					if (spBoundingBoxAttachment* boundingboxAttachment = (spBoundingBoxAttachment*)attachment) {
						IwAssertMsg(MYAPP, boundingboxAttachment->verticesCount%2 == 0, ("Invalid number of vertices in bounding box."));
						for (int i = 0; i < boundingboxAttachment->verticesCount - 1; i += 2) {
							if (!hasbounds) {
								// get starting vertice
								hasbounds = true;
								ll.x = boundingboxAttachment->vertices[i];
								ll.y = boundingboxAttachment->vertices[i + 1];
								ur = ll;
							} else {
								GrowAABB(ll, ur, CIwFVec2(boundingboxAttachment->vertices[i], boundingboxAttachment->vertices[i + 1]));
							}
						}
					}
				}
			}
		}
	}
	IwAssertMsg(MYAPP, hasbounds, ("The skeleton does not seem to include any bounding box attachments. AABB cannot be calculated."));
	return hasbounds;
}

void SpineAnimation::GrowAABB(CIwFVec2& ll, CIwFVec2& ur, const CIwFVec2& point) {
	ll.x = std::min<float>(ll.x, point.x);
	ll.y = std::min<float>(ll.y, point.y);
	ur.x = std::max<float>(ur.x, point.x);
	ur.y = std::max<float>(ur.y, point.y);
}

CIwTexture* SpineAnimation::ExtractStreams(spSlot* slot, spRegionAttachment* att, int length, CIwFVec2 xys[], CIwFVec2 uvs[], uint32 cols[]) {
	if (!slot || !slot->data || !att || length != SPINEANIMATION_VERTS_PER_SLOT) {
		return NULL;
	}
	
	float spineverts[8];
	spRegionAttachment_computeWorldVertices(att, slot->skeleton->x, slot->skeleton->y, slot->bone, spineverts);
	
	xys[0].x = spineverts[VERTEX_X1];
	xys[0].y = spineverts[VERTEX_Y1];
	xys[1].x = spineverts[VERTEX_X2];
	xys[1].y = spineverts[VERTEX_Y2];
	xys[2].x = spineverts[VERTEX_X3];
	xys[2].y = spineverts[VERTEX_Y3];
	xys[3].x = spineverts[VERTEX_X4];
	xys[3].y = spineverts[VERTEX_Y4];
	TransformToWorld(xys, length);

	CIwTexture* texture = NULL;
	if (att->rendererObject) {
		if (spAtlasRegion* atlasreg = (spAtlasRegion*)att->rendererObject) {
			if (spAtlasPage* atlaspage = (spAtlasPage*)atlasreg->page) {
				if (atlaspage->rendererObject) {
					ExtractUVs(uvs, atlasreg);
					texture = (CIwTexture*)atlaspage->rendererObject;
				}
			}
		}
	}
			
	ExtractColours(cols, slot);
		
	return texture;
}

void SpineAnimation::ExtractColours(uint32 cols[4], spSlot* slot) {
	CIwColour col;
	col.Set((slot->r * 0xff), (slot->g * 0xff), (slot->b * 0xff), (slot->a * 0xff));
	cols[0] = col.Get();
	cols[1] = cols[0];
	cols[2] = cols[0];
	cols[3] = cols[0];
}

void SpineAnimation::ExtractUVs(CIwFVec2 uvs[4], spAtlasRegion* atlasreg) {
	float u = atlasreg->u;
	float v = atlasreg->v;
	float u2 = atlasreg->u2;
	float v2 = atlasreg->v2;
	
	if (atlasreg->rotate) {
		uvs[1].x = u;
		uvs[1].y = v2;
		uvs[2].x = u;
		uvs[2].y = v;
		uvs[3].x = u2;
		uvs[3].y = v;
		uvs[0].x = u2;
		uvs[0].y = v2;
	} else {
		uvs[0].x = u;
		uvs[0].y = v2;
		uvs[1].x = u;
		uvs[1].y = v;
		uvs[2].x = u2;
		uvs[2].y = v;
		uvs[3].x = u2;
		uvs[3].y = v2;
	}
}

extern "C" {
	void _spAtlasPage_createTexture (spAtlasPage* self, const char* path) {
		IW_CALLSTACK_SELF;

		// copy the path for manipulation
		int len = strlen(path);
		char* buf = new char[len + 1];
		buf[len] = '\0';
		strncpy(buf, path, len);
		
		// finding basename
		char* basename = buf;
		if (char* p = strrchr(basename, '/')) basename = &p[1];
		if (char* p = strrchr(basename, '.')) (*p)='\0';
		
		CIwTexture* texture = (CIwTexture*)IwGetResManager()->GetResNamed(basename, "CIwTexture");
		self->rendererObject = texture;
		self->width = texture->GetWidth();
		self->height = texture->GetHeight();
		
		delete [] buf;
	}
	
	void _spAtlasPage_disposeTexture (spAtlasPage* self) {
		if (self->rendererObject) {
			// delete (CIwTexture*)self->rendererObject;
			// memory is managed by resource manager
			self->rendererObject = NULL;
		}
	}
	
	char* _spUtil_readFile (const char* path, int* length) {
		IW_CALLSTACK_SELF;

		return _readFile(path, length);
	}
}