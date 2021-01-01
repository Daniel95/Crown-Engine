#pragma once
#include "Core/ECS/System/ComponentSystem.h"
#include "Core/Audio/AudioManager.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <iostream>
#include <unordered_map>

namespace Crown
{
	//Audio component that wraps around a normal Sound object
	struct AudioSourceComponent
	{
		//Shared pointer to an Sound
		std::shared_ptr<Sound> properties = nullptr;
	};
	
	class AudioSourceComponentSystem : public ComponentSystem
	{
	public:
		AudioSourceComponentSystem();
		~AudioSourceComponentSystem() = default;

		virtual void OnAddEntity(Entity a_Entity) override;
		virtual void OnRemoveEntity(Entity a_Entity) override;

		//Sets up the audio component for 3D sounds
		void SetupAudioComponent(Entity a_Entity, std::string a_SoundName, glm::vec3 a_Position, float a_Volume, bool a_Looping);
		//Sets up the audio component for 2D sounds
		void SetupAudioComponent(Entity a_Entity, std::string a_SoundName, float a_Volume, bool a_Looping);

		//Stops this audio component from playing
		void Stop(Entity a_Entity);
		//Returns if this audio component is currently playing or not
		bool IsPlaying(Entity a_Entity);
		//Plays this audio component
		void Play(Entity a_Entity);

		//Returns the position of this audio component
		glm::vec3 GetPosition(Entity a_Entity);
		//Sets the position fo this audio component
		void SetPosition(Entity a_Entity, glm::vec3 a_Position);

		//Returns the volume of this audio component
		float GetVolume(Entity a_Entity);
		//Sets the volume of this audio component
		void SetVolume(Entity a_Entity, float a_Volume);

		//Returns if this audio component is looping or not
		bool IsLooping(Entity a_Entity);
		//Sets the looping value of this audio component
		void SetLooping(Entity a_Entity, bool a_Loop);

		//Returns if this audio component is pause or not
		bool IsPaused(Entity a_Entity);
		//Pauses this audio component
		void Pause(Entity a_Entity);

	private:
		std::unordered_map<int, AudioSourceComponent> m_Data;
	};
}
