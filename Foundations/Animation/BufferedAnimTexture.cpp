#include "BufferedAnimTexture.h"

#include "TypeConverter.h"

#include "IwDebug.h"
#include "Debug.h"

#include <limits>


BufferedAnimTexture::BufferedAnimTexture() :
m_iBufferLength(0),
m_apxXYConverted(NULL),
m_apxXYBuffer(NULL),
m_apxUVBuffer(NULL),
m_auiColBuffer(NULL),
m_iBufferedVertCount(0),
m_bEnableConversion(false),
m_pxSpriteSheet(NULL),
m_bIsDirty(false) {
	AllocateBuffers(BUFFEREDANIMETEXTURE_DEFAULT_VERT_COUNT);
}

BufferedAnimTexture::~BufferedAnimTexture() {
	DeleteBuffers();
}

void BufferedAnimTexture::EnsureBufferSize(int length) {
	if (length > m_iBufferLength) {
		DeleteBuffers();
		AllocateBuffers(length);
	}
}

void BufferedAnimTexture::AllocateBuffers(int length) {
	IW_CALLSTACK_SELF;
	IwAssert(MYAPP, !m_apxXYBuffer);
	
	m_iBufferLength = length;
	m_apxXYBuffer = new CIwFVec2[length];
	m_apxUVBuffer = new CIwFVec2[length];
	m_auiColBuffer = new uint32[length];
	m_apxShapeConverted = new CIwFVec2[length];
	m_apxXYConverted = new CIwSVec2[length];
	
	m_bIsDirty = true;
}

void BufferedAnimTexture::DeleteBuffers() {
	m_iBufferLength = 0;
	if (m_apxXYBuffer) {
		delete [] m_apxXYBuffer;
		m_apxXYBuffer = NULL;
	}
	if (m_apxUVBuffer) {
		delete [] m_apxUVBuffer;
		m_apxUVBuffer = NULL;
	}
	if (m_auiColBuffer) {
		delete [] m_auiColBuffer;
		m_auiColBuffer = NULL;
	}
	if (m_apxXYConverted) {
		delete [] m_apxXYConverted;
		m_apxXYConverted = NULL;
	}
	if (m_apxShapeConverted) {
		delete [] m_apxShapeConverted;
		m_apxShapeConverted = NULL;
	}
}

void BufferedAnimTexture::UpdateBuffers() {
	IW_CALLSTACK_SELF;

	if (!m_bIsDirty) {
		return;
	}
	
	m_iBufferedVertCount = GetVertexCount();
	EnsureBufferSize(m_iBufferedVertCount);
	m_pxSpriteSheet = GetStreams(m_iBufferedVertCount, m_apxXYBuffer, m_apxUVBuffer, m_auiColBuffer);

	if (m_bEnableConversion) {
		CopyConvertXY();
	}
	
	m_bIsDirty = false;
}

void BufferedAnimTexture::SetAll(CIwFVec2 verts[], int vertcount) {
	IW_CALLSTACK_SELF;
	AnimTexture::SetAll(verts, vertcount);
	m_bIsDirty = true;
}

void BufferedAnimTexture::SetAll(CIwSVec2 verts[], int vertcount) {
	IW_CALLSTACK_SELF;
	SetScreenspaceMode();
	EnsureBufferSize(vertcount);
	CopyConvertShape(verts, vertcount);
	SetAll(m_apxShapeConverted, vertcount);
}

void BufferedAnimTexture::SetShape(CIwFVec2 verts[], int vertcount) {
	IW_CALLSTACK_SELF;
	AnimTexture::SetShape(verts, vertcount);
	m_bIsDirty = true;
}

void BufferedAnimTexture::SetShape(CIwSVec2 verts[], int vertcount) {
	IW_CALLSTACK_SELF;
	SetScreenspaceMode();
	EnsureBufferSize(vertcount);
	CopyConvertShape(verts, vertcount);
	SetShape(m_apxShapeConverted, vertcount);
}

void BufferedAnimTexture::CopyConvertShape(CIwSVec2 verts[], int vertcount) {
	for (int i = 0; i < vertcount; i++) {
		m_apxShapeConverted[i].x = verts[i].x;
		m_apxShapeConverted[i].y = verts[i].y;
	}
}

void BufferedAnimTexture::CopyConvertXY() {
	IW_CALLSTACK_SELF;
	IwAssert(MYAPP, m_iBufferedVertCount <= m_iBufferLength);
	
	for (int i = 0; i < std::min<int>(m_iBufferedVertCount, m_iBufferLength); i++) {
		m_apxXYConverted[i].x = TypeConverter::SafeFloatToInt<float, int16>(m_apxXYBuffer[i].x);
		m_apxXYConverted[i].y = TypeConverter::SafeFloatToInt<float, int16>(m_apxXYBuffer[i].y);
	}
	for (int i = m_iBufferedVertCount; i < m_iBufferLength; i++) {
		// this is simply to indicate an uninitialized vertex
		m_apxXYConverted[i].x = std::numeric_limits<float>::max();
		m_apxXYConverted[i].y = std::numeric_limits<float>::max();
	}
}

void BufferedAnimTexture::Update(uint32 timestep) {
	IW_CALLSTACK_SELF;
	SpineAnimation::Update(timestep);
	m_bIsDirty = true;
}

int BufferedAnimTexture::GetBufferedVertexCount() {
	UpdateBuffers();
	return m_iBufferedVertCount;
}

CIwTexture* BufferedAnimTexture::GetBufferedSpriteSheet() {
	UpdateBuffers();
	return m_pxSpriteSheet;
}

CIwFVec2* BufferedAnimTexture::GetBufferedXYs(const CIwFVec2& type) {
	UpdateBuffers();
	return m_apxXYBuffer;
}

CIwSVec2* BufferedAnimTexture::GetBufferedXYs(const CIwSVec2& type) {
	IW_CALLSTACK_SELF;
	IwAssert(MYAPP, m_bEnableConversion);
	UpdateBuffers();
	return m_apxXYConverted;
}

CIwFVec2* BufferedAnimTexture::GetBufferedUVs() {
	UpdateBuffers();
	return m_apxUVBuffer;
}

uint32* BufferedAnimTexture::GetBufferedCols() {
	UpdateBuffers();
	return m_auiColBuffer;
}

void BufferedAnimTexture::SetScreenspaceMode() {
	// screenspace mode enables conversion of streams to fixed point
	// it also flips the Y axis because screenspace is inverted
	m_bEnableConversion = true;
	SetFlipY(true);
}
