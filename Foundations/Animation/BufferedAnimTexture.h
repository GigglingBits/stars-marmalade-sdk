#ifndef __BUFFEREDANIMTEXTURE_H__
#define __BUFFEREDANIMTEXTURE_H__

#include "AnimTexture.h"

// Start with 10 slots to start with. Most animations can probably fit in there.
#define BUFFEREDANIMETEXTURE_DEFAULT_VERT_COUNT (SPINEANIMATION_VERTS_PER_SLOT * 10)

/**
 * Extends the AnimTexture class to with ability to work with fixed point coordinates
 */
class BufferedAnimTexture : public AnimTexture {
private:
	int m_iBufferLength;
	CIwFVec2* m_apxXYBuffer;
	CIwFVec2* m_apxUVBuffer;
	uint32* m_auiColBuffer;

	int m_iBufferedVertCount;
	
	CIwTexture* m_pxSpriteSheet;
	
	bool m_bEnableConversion;		// conversion is needed when input verts were fixed point
	CIwFVec2* m_apxShapeConverted;
	CIwSVec2* m_apxXYConverted;
	
	bool m_bIsDirty;				// dirty means that the buffered vertices need to be updated
	
public:
	BufferedAnimTexture();
	~BufferedAnimTexture();
	
	void SetAll(CIwFVec2 verts[], int vertcount);
	void SetAll(CIwSVec2 verts[], int vertcount);

	void SetShape(CIwFVec2 verts[], int vertcount);
	void SetShape(CIwSVec2 verts[], int vertcount);

	virtual void Update(uint32 timestep);

	int GetBufferedVertexCount();
	CIwTexture* GetBufferedSpriteSheet();
	CIwFVec2* GetBufferedXYs(const CIwFVec2& type);
	CIwSVec2* GetBufferedXYs(const CIwSVec2& type);
	CIwFVec2* GetBufferedUVs();
	uint32* GetBufferedCols();
	
private:
	void EnsureBufferSize(int length);
	void AllocateBuffers(int length);
	void DeleteBuffers();
	void UpdateBuffers();
	
	void CopyConvertShape(CIwSVec2 verts[], int vertcount);
	void CopyConvertXY();
};

#endif
