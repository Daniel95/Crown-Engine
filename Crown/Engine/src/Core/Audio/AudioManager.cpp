#include "CrownPCH.h"
#include "AudioManager.h"
#include "Core/ECS/Entity/Entity.h"
#include "Core/Application.h"

Crown::Sound::Sound(std::string a_SoundName, glm::vec3 a_Position, float a_Volume, bool a_Looping) : m_SoundName(a_SoundName), m_Position(a_Position), m_Volume(a_Volume), m_Looping(a_Looping)
{
	m_Is3d = true;
}

Crown::Sound::Sound(std::string a_SoundName, float a_Volume, bool a_Looping) : m_SoundName(a_SoundName), m_Volume(a_Volume), m_Looping(a_Looping)
{
	m_Is3d = false;
}

Crown::Sound::~Sound()
{
	m_Sound->release();
	delete m_Sound;
	delete m_Channel;
}

bool Crown::Sound::IsPlaying()
{
	return m_IsPlaying;
}

glm::vec3 Crown::Sound::GetPosition()
{
	return m_Position;
}

float Crown::Sound::GetVolume()
{
	return m_Volume;
}

bool Crown::Sound::IsLooping()
{
	return m_Looping;
}

Crown::AudioManager::AudioManager()
{
	Init();
}

Crown::AudioManager::~AudioManager()
{
	Shutdown();
}

void Crown::AudioManager::Update() {
	ErrorCheck(m_StudioSystem->update());
}

void Crown::AudioManager::LoadSound(std::shared_ptr<Sound> a_AudioSound)
{
	FMOD_MODE eMode = FMOD_DEFAULT;
	eMode |= a_AudioSound->m_Is3d ? FMOD_3D : FMOD_2D;
	eMode |= a_AudioSound->m_Looping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
	eMode |= false ? FMOD_CREATESTREAM : FMOD_CREATECOMPRESSEDSAMPLE;
	ErrorCheck(m_System->createSound(a_AudioSound->m_SoundName.c_str(), eMode, nullptr, &a_AudioSound->m_Sound));
}

void Crown::AudioManager::PlaySound(std::shared_ptr<Sound> a_AudioSound)
{
	ErrorCheck(m_System->playSound(a_AudioSound->m_Sound, nullptr, true, &a_AudioSound->m_Channel));
	if (a_AudioSound->m_Channel)
	{
		std::cout << a_AudioSound->m_Position.x << std::endl;
		FMOD_MODE currMode;
		a_AudioSound->m_Sound->getMode(&currMode);
		if (currMode & FMOD_3D) {
			FMOD_VECTOR position = VectorToFmod(a_AudioSound->m_Position);
			ErrorCheck(a_AudioSound->m_Channel->set3DAttributes(&position, nullptr));
		}
		ErrorCheck(a_AudioSound->m_Channel->setVolume(DbToVolume(a_AudioSound->m_Volume)));
		ErrorCheck(a_AudioSound->m_Channel->setPaused(false));
	}
	a_AudioSound->m_IsPlaying = true;
}

void Crown::AudioManager::PauseSound(std::shared_ptr<Sound> a_AudioSound)
{
	a_AudioSound->m_IsPlaying = false;
	a_AudioSound->m_Channel->setPaused(true);
}

void Crown::AudioManager::StopSound(std::shared_ptr<Sound> a_AudioSound)
{
	ErrorCheck(a_AudioSound->m_Sound->release());
	a_AudioSound->m_IsPlaying = false;
}

void Crown::AudioManager::SetSoundVolume(std::shared_ptr<Sound> a_AudioSound, float a_Volume)
{
	a_AudioSound->m_Volume = a_Volume;
	a_AudioSound->m_Channel->setVolume(a_Volume);
}

void Crown::AudioManager::SetSoundLoop(std::shared_ptr<Sound> a_AudioSound, bool a_Loop)
{
	a_AudioSound->m_Looping = a_Loop;
	FMOD_MODE mode;
	if (a_Loop) mode = FMOD_LOOP_NORMAL;
	else mode = FMOD_LOOP_OFF;
	a_AudioSound->m_Channel->setMode(mode);
}

void Crown::AudioManager::SetSoundPosition(std::shared_ptr<Sound> a_AudioSound, glm::vec3 a_Position)
{
	a_AudioSound->m_Position = a_Position;
	FMOD_VECTOR position = VectorToFmod(a_Position);
	ErrorCheck(a_AudioSound->m_Channel->set3DAttributes(&position, nullptr));
}

bool Crown::AudioManager::IsPlaying(std::shared_ptr<Sound> a_AudioSound)
{
	return a_AudioSound->m_IsPlaying;
}

void Crown::AudioManager::Init() {
	m_StudioSystem = NULL;
	ErrorCheck(FMOD::Studio::System::create(&m_StudioSystem));
	ErrorCheck(m_StudioSystem->initialize(32, FMOD_STUDIO_INIT_LIVEUPDATE, FMOD_INIT_PROFILE_ENABLE, NULL));

	m_System = NULL;
	ErrorCheck(m_StudioSystem->getCoreSystem(&m_System));
}

void Crown::AudioManager::Shutdown() {
	ErrorCheck(m_StudioSystem->unloadAll());
	ErrorCheck(m_StudioSystem->release());
}

float Crown::AudioManager::DbToVolume(float dB)
{
	return powf(10.0f, 0.05f * dB);
}

float Crown::AudioManager::VolumeToDb(float a_Volume)
{
	return 20.0f * log10f(a_Volume);
}

FMOD_VECTOR Crown::AudioManager::VectorToFmod(const glm::vec3& vPosition) {
	FMOD_VECTOR fVec;
	fVec.x = vPosition.x;
	fVec.y = vPosition.y;
	fVec.z = vPosition.z;
	return fVec;
}

int Crown::AudioManager::ErrorCheck(FMOD_RESULT result) {
	if (result != FMOD_OK) {
		std::cout << "FMOD ERROR " << result << std::endl;
		return 1;
	}

	return 0;
}
