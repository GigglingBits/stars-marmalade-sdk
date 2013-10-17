#include "SoundEngine.h"

#include "s3e.h"

#include "IwResManager.h"
#include "IwSound.h"
#include "IwDebug.h"

SoundEngine* SoundEngine::s_pxInstance = NULL;

void SoundEngine::Initialize() {
#ifdef IW_BUILD_RESOURCES
	IwGetResManager()->AddHandler(new CIwResHandlerWAV);
#endif
	IwSoundInit();

	GetInstance();
}

void SoundEngine::Terminate() {
	if (s_pxInstance) {
		delete s_pxInstance;
		s_pxInstance = NULL;
	}

	IwSoundTerminate();
}

SoundEngine::SoundEngine() {
}

SoundEngine::~SoundEngine() {
	StopMusicFile();
}

SoundEngine& SoundEngine::GetInstance() {
	if (!s_pxInstance) {
		s_pxInstance = new SoundEngine();
	}
	return *s_pxInstance;
}

const std::string& SoundEngine::GetPlayingMusicFile() {
	static const std::string s;
	return s3eAudioIsPlaying() ? m_sMusicFile : s;
}

void SoundEngine::PlayMusicFileLoop(const std::string& filename) {
	s3eAudioPlay(filename.c_str(), 0);
	m_sMusicFile = filename;
}

void SoundEngine::PlayMusicFile(const std::string& filename) {
	s3eAudioPlay(filename.c_str(), 1);
	m_sMusicFile = filename;
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
