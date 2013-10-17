#ifndef __SOUNDENGINE_H__
#define __SOUNDENGINE_H__

#include <map>
#include <string>

#include "IwSound.h"
#include "IwSoundInst.h"

class SoundEngine {
private:
	static SoundEngine* s_pxInstance;
	SoundEngine();
	~SoundEngine();
	
private:
	std::string m_sMusicFile;
	
public:
	static void Initialize();
	static void Terminate();
	static SoundEngine& GetInstance();

public:
	const std::string& GetPlayingMusicFile();
	void PlayMusicFileLoop(const std::string& filename);
	void PlayMusicFile(const std::string& filename);
	void StopMusicFile();

	void PlaySoundEffect(const std::string& soundname);

	void Update();
};

#endif
