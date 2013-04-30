#ifndef __FACTORYMANAGER_H__
#define __FACTORYMANAGER_H__

#include "EffectFactory.h"
#include "TextureFactory.h"
#include "ShapeFactory.h"
#include "FixtureFactory.h"
#include "BodyFactory.h"
#include "PreambleFactory.h"
#include "LevelFactory.h"

class FactoryManager {
private:
	static ShapeFactory* s_pxShapeFactory;
	static TextureFactory* s_pxTextureFactory;
	static EffectFactory* s_pxEffectFactory;
	static FixtureFactory* s_pxFixtureFactory;
	static BodyFactory* s_pxBodyFactory;
	static PreambleFactory* s_pxPreambleFactory;
	static LevelFactory* s_pxLevelFactory;

public:
	static bool InitializeFromFiles(std::list<std::string> filenames);
	static bool InitializeFromStrings(std::list<std::string> xmldatalist);
	static bool Initialize(std::list<TiXmlDocument> xmldoclist);
	static void Terminate();

	static ShapeFactory& GetShapeFactory();
	static TextureFactory& GetTextureFactory();
	static EffectFactory& GetEffectFactory();
	static FixtureFactory& GetFixtureFactory();
	static BodyFactory& GetBodyFactory();
	static PreambleFactory& GetPreambleFactory();
	static LevelFactory& GetLevelFactory();

private:
	static bool ValidateStructure(TiXmlDocument& xmldoc);
};

#endif
