#ifndef __SPRITE_H__
#define __SPRITE_H__

#include "IwGeom.h"
#include "Texture.h"
#include "TextureTemplate.h"
#include "Renderable.h"
#include "VertexStreamWorld.h"

class GameFoundation;

class Sprite : public Renderable {
public:
	enum Orientation {
		eOrientationUndefined,
		eOrientationLeft,
		eOrientationRight
	};

private:
	std::string m_sId;

	Orientation m_eOrientation;
    Texture* m_pxTexture;
	VertexStreamWorld m_xShape;

	CIwFVec2 m_xPosition;

public:
	Sprite(const std::string& id, const TextureTemplate& texturedef);
	virtual ~Sprite();

	void SetId(const std::string& id);
	const std::string& GetId() const;

	virtual const char* GetTypeName();
	static const char* TypeName();

	virtual bool CanDispose();

	virtual void SetPosition(const CIwFVec2& position);
	virtual CIwFVec2 GetPosition() const;

	void SetShape(CIwFVec2 verts[], int count, bool import);
	void SetShape(const VertexStreamWorld& shape);

	Orientation GetOrientation();
	void SetOrientation(Orientation orientation);

	void SetTextureFrame(const std::string& name);
	
protected:
	virtual Texture* GetTexture();
	virtual VertexStreamWorld& GetShape();

	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);
};

#endif
