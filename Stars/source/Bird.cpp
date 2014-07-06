#include "Bird.h"
#include "Debug.h"

#include "SoundEngine.h"
#include "Main.h"

Bird::Bird(const std::string& id, const b2BodyDef& bodydef, const b2FixtureDef& fixturedef, const TextureTemplate& texturedef)
: Enemy(id, bodydef, fixturedef, texturedef) {
	SetTextureFrame("normal");

	RegisterSoftsopt("softspot_a");
	RegisterSoftsopt("softspot_b");
	RegisterSoftsopt("softspot_c");
}

const char* Bird::GetTypeName() {
	return Bird::TypeName();
}

const char* Bird::TypeName() {
	static const char* type = "bird";
	return type;
}

void Bird::KnockOut() {
	Enemy::KnockOut();

	SetTextureFrame("hurt");
		
	SoundEngine::GetInstance().PlaySoundEffect("BirdCollision");
	
	DustEventArgs args;
	args.amount = GAME_POINTS_BIRD;
	args.eventtype = eDustEventTypeAdd;
	args.position = GetPosition();
	DustEvent.Invoke(*this, args);
}
