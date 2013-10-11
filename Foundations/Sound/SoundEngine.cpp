#include "SoundEngine.h"

#include "s3e.h"

#include "IwResManager.h"
#include "IwSound.h"
#include "IwDebug.h"

void SoundEngine::Initialize() {
#ifdef IW_BUILD_RESOURCES
	IwGetResManager()->AddHandler(new CIwResHandlerWAV);
#endif
	IwSoundInit();
}

void SoundEngine::Terminate() {
	IwSoundTerminate();
}

void SoundEngine::PlayMusicFileLoop(const std::string& filename) {
	s3eAudioPlay(filename.c_str(), 0);
}

void SoundEngine::PlayMusicFile(const std::string& filename) {
	s3eAudioPlay(filename.c_str(), 1);
}

void SoundEngine::StopMusicFile() {
	s3eAudioStop();
}

void SoundEngine::PlaySoundEffect(const std::string& soundname) {
	CIwSoundSpec* sound = (CIwSoundSpec*)IwGetResManager()->GetResNamed(soundname.c_str(), IW_SOUND_RESTYPE_SPEC);
	IwAssertMsg(MYAPP, sound, ("The sound %s cannot be played. It was not found in the resource group.", soundname.c_str()));
	sound->Play();
}

void SoundEngine::Update() {
	IwGetSoundManager()->Update();
}
