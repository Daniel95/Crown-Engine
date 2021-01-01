#include "CrownPCH.h"
#include "AudioSourceComponentSystem.h"
#include "Core/ECS/Entity/Entity.h"

Crown::AudioSourceComponentSystem::AudioSourceComponentSystem()
{
}

void Crown::AudioSourceComponentSystem::OnAddEntity(Entity a_Entity)
{
	ASSERT_ENGINE(m_Data.count(a_Entity) == 0, "Entity already exists!");

	std::pair<int, AudioSourceComponent> pair{ a_Entity, {} };
	m_Data.insert(pair);
}

void Crown::AudioSourceComponentSystem::OnRemoveEntity(Entity a_Entity)
{
	ASSERT_ENGINE(m_Data.count(a_Entity) != 0, "Entity does not exist!");

	m_Data.erase(a_Entity);
}

void Crown::AudioSourceComponentSystem::SetupAudioComponent(Entity a_Entity, std::string a_SoundName,
	glm::vec3 a_Position, float a_Volume, bool a_Looping)
{
	m_Data.at(a_Entity).properties = std::make_shared<Sound>(a_SoundName, a_Position, a_Volume, a_Looping);
	Application::Get().GetAudioManager()->LoadSound(m_Data.at(a_Entity).properties);
}

void Crown::AudioSourceComponentSystem::SetupAudioComponent(Entity a_Entity, std::string a_SoundName, float a_Volume,
	bool a_Looping)
{
	m_Data.at(a_Entity).properties = std::make_shared<Sound>(a_SoundName, a_Volume, a_Looping);
	Application::Get().GetAudioManager()->LoadSound(m_Data.at(a_Entity).properties);
}

void Crown::AudioSourceComponentSystem::Stop(Entity a_Entity)
{
	Application::Get().GetAudioManager()->StopSound(m_Data.at(a_Entity).properties);
}

bool Crown::AudioSourceComponentSystem::IsPlaying(Entity a_Entity)
{
	return Application::Get().GetAudioManager()->IsPlaying(m_Data.at(a_Entity).properties);
}

void Crown::AudioSourceComponentSystem::Play(Entity a_Entity)
{
	Application::Get().GetAudioManager()->PlaySound(m_Data.at(a_Entity).properties);
}

glm::vec3 Crown::AudioSourceComponentSystem::GetPosition(Entity a_Entity)
{
	return m_Data.at(a_Entity).properties->GetPosition();
}

void Crown::AudioSourceComponentSystem::SetPosition(Entity a_Entity, glm::vec3 a_Position)
{
	Application::Get().GetAudioManager()->SetSoundPosition(m_Data.at(a_Entity).properties, a_Position);
}

float Crown::AudioSourceComponentSystem::GetVolume(Entity a_Entity)
{
	return m_Data.at(a_Entity).properties->GetVolume();
}

void Crown::AudioSourceComponentSystem::SetVolume(Entity a_Entity, float a_Volume)
{
	Application::Get().GetAudioManager()->SetSoundVolume(m_Data.at(a_Entity).properties, a_Volume);
}

bool Crown::AudioSourceComponentSystem::IsLooping(Entity a_Entity)
{
	return m_Data.at(a_Entity).properties->IsLooping();
}

void Crown::AudioSourceComponentSystem::SetLooping(Entity a_Entity, bool a_Loop)
{
	Application::Get().GetAudioManager()->SetSoundLoop(m_Data.at(a_Entity).properties, a_Loop);
}

bool Crown::AudioSourceComponentSystem::IsPaused(Entity a_Entity)
{
	return m_Data.at(a_Entity).properties->IsPlaying();
}

void Crown::AudioSourceComponentSystem::Pause(Entity a_Entity)
{
	Application::Get().GetAudioManager()->PauseSound(m_Data.at(a_Entity).properties);
}
