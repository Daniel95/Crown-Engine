#pragma once
#include <fmod_studio.hpp>
#include <fmod.hpp>

namespace Crown
{
	class Entity;

	class Sound
	{
	public:
		//Constructor for 3D sounds
		Sound(std::string a_SoundName, glm::vec3 a_Position, float a_Volume, bool a_Looping);
		//Constructor for 2D sounds
		Sound(std::string a_SoundName, float a_Volume, bool a_Looping);
		~Sound();

		///Getter functions
		bool IsPlaying();
		glm::vec3 GetPosition();
		float GetVolume();
		bool IsLooping();

	private:
		//The fmod m_Sound
		FMOD::Sound* m_Sound = nullptr;
		//The fmod m_Channel
		FMOD::Channel* m_Channel = nullptr;
		bool m_IsPlaying;

		///Generic needed values for an Sound object
		std::string m_SoundName;
		bool m_Is3d;
		glm::vec3 m_Position;
		float m_Volume;
		bool m_Looping;

		//Friend class of the Audiomanager
		friend class AudioManager;
	};

	class AudioManager
	{
	public:
		AudioManager();
		~AudioManager();

		//Updates all current active channels used in this Audio system
		void Update();

		//Loads 3D and 2D sounds into memory
		void LoadSound(std::shared_ptr<Sound> a_AudioSound);
		//Makes sure a m_Sound is played
		void PlaySound(std::shared_ptr<Sound> a_AudioSound);
		//Pauses a m_Sound with the asked name
		void PauseSound(std::shared_ptr<Sound> a_AudioSound);
		//Stops a m_Sound with the asked name
		void StopSound(std::shared_ptr<Sound> a_AudioSound);

		//Sets a m_Sound m_Volume
		void SetSoundVolume(std::shared_ptr<Sound> a_AudioSound, float a_Volume);
		//Sets the active loop state of a m_Sound
		void SetSoundLoop(std::shared_ptr<Sound> a_AudioSound, bool a_Loop);
		//Sets the m_Position of a m_Sound
		void SetSoundPosition(std::shared_ptr<Sound> a_AudioSound, glm::vec3 a_Position);

		//Returns if a m_Sound is playing or not
		bool IsPlaying(std::shared_ptr<Sound> a_AudioSound);

	private:
		//Initializes FMOD
		void Init();
		//Shuts down FMOD
		void Shutdown();

		//Decibels to m_Volume
		float DbToVolume(float a_Decibel);
		//Volume to decibels
		float VolumeToDb(float a_Volume);
		//Transforms in this case a glm vector to the vector type FMOD is mostly using
		FMOD_VECTOR VectorToFmod(const glm::vec3& vPosition);

		//Used for error checking
		int ErrorCheck(FMOD_RESULT result);

	private:
		//FMOD studio System
		FMOD::Studio::System* m_StudioSystem = nullptr;
		//FMOD System
		FMOD::System* m_System;
	};
}
