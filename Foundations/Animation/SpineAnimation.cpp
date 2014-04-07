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
}

SpineAnimation::~SpineAnimation() {
	DestroySkeleton();
}

void SpineAnimation::Load(const std::string& atlasfile, const std::string& jsonfile, float scale) {
	LoadSkeleton(atlasfile, jsonfile, scale);
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

void SpineAnimation::SetPosition(const CIwFVec2& pos) {
	IW_CALLSTACK_SELF;
	IwAssertMsg(MYAPP, m_pxSkeleton, ("No skeleton loaded. Cannot set position."));
	if (m_pxSkeleton) {
		m_pxSkeleton->x = pos.x;
		m_pxSkeleton->y = pos.y;
		spSkeleton_updateWorldTransform(m_pxSkeleton);
	}
}

void SpineAnimation::SetScale(float scale) {
	IwAssertMsg(MYAPP, m_pxSkeleton, ("No skeleton loaded. Cannot set scale."));
	IwAssertMsg(MYAPP, m_pxSkeleton->root, ("Skeleton has no root bone. Cannot set scale."));
	if (m_pxSkeleton) {
		if (m_pxSkeleton->root) {
			m_pxSkeleton->root->scaleX = scale;
			m_pxSkeleton->root->scaleY = scale;
			spSkeleton_updateWorldTransform(m_pxSkeleton);
		}
	}
}

void SpineAnimation::SetRotation(float angle) {
	IwAssertMsg(MYAPP, m_pxSkeleton, ("No skeleton loaded. Cannot set rotation."));
	IwAssertMsg(MYAPP, m_pxSkeleton->root, ("Skeleton has no root bone. Cannot set rotation."));
	if (m_pxSkeleton) {
		if (m_pxSkeleton->root) {
			m_pxSkeleton->root->rotation = angle;
			spSkeleton_updateWorldTransform(m_pxSkeleton);
		}
	}	
}

void SpineAnimation::LoadSkeleton(const std::string& atlasfile, const std::string& jsonfile, float scale) {
	IW_CALLSTACK_SELF;

	if ((m_pxAtlas = spAtlas_readAtlasFile(atlasfile.c_str()))) {
		if ((m_pxSkeletonJson = spSkeletonJson_create(m_pxAtlas))) {
			m_pxSkeletonJson->scale = scale;
			if ((m_pxSkeletonData = spSkeletonJson_readSkeletonDataFile(m_pxSkeletonJson, jsonfile.c_str()))) {
				if ((m_pxSkeleton = spSkeleton_create(m_pxSkeletonData))) {
					m_pxSkeleton->flipX = false;
					m_pxSkeleton->flipY = false;
					spSkeleton_setToSetupPose(m_pxSkeleton);
					spSkeleton_updateWorldTransform(m_pxSkeleton);
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
	}
	
	// perform actual animation
	if (m_pxAnimation) {
		spAnimation_apply(m_pxAnimation, m_pxSkeleton, m_fAnimationTime, m_fAnimationTime + seconds, 1, NULL, 0);
		spSkeleton_updateWorldTransform(m_pxSkeleton);
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