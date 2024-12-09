#include "../../SDK.h"
#include "AudioSource.h"

using namespace Engine::EngineObjects;

#pragma managed(push, off)
void onUnloaded(AudioData sound)
{
	// do nothing lol, no need to unload resources (unmanaged btw, we dont want user to see this method)

	return;
}
#pragma managed(pop)

bool triggerPlay = true;

AudioSource::AudioSource(String^ name, Engine::Internal::Components::Transform^ transform)
	: Script(name, transform)
{
}

void AudioSource::onSoundChanged(unsigned int newId, unsigned int oldId)
{
	AudioData data = AudioData(
		DataPacks::singleton().GetSound(soundId),
		DataPacks::singleton().GetMusic(soundId)
	);

	soundPtr->setInstance(
		data
	);
}

void AudioSource::Start()
{
	AudioData data = AudioData(
		DataPacks::singleton().GetSound(soundId),
		DataPacks::singleton().GetMusic(soundId)
	);

	soundPtr = new Engine::Native::EnginePtr<AudioData>(data, &onUnloaded);

	if (attributes->hasAttribute("soundId"))
		attributes->getAttribute("soundId")->onPropertyChanged->connect(gcnew Action<unsigned int, unsigned int>(this, &AudioSource::onSoundChanged));

	isPlaying = false;
	wasPlaying = false;
}

void AudioSource::Update()
{
	if (&soundPtr->getInstance() == NULL)
		return;

	if (triggerPlay)  // SET DEBOUNCER, JUST IN CASE ATTRIBUTE FLAGS TRUE ON ISPLAYING
	{
		isPlaying = false;
		triggerPlay = false;
	}

	if (isPlaying)
	{
		if (resourceType == ResourceType::Music)
		{
			RAYLIB::SetMusicVolume(soundPtr->getInstance().music, volume);
			RAYLIB::SetMusicPan(soundPtr->getInstance().music, panning);
			RAYLIB::SetMusicPitch(soundPtr->getInstance().music, pitch);
			UpdateMusicStream(soundPtr->getInstance().music);
		}
		else if (resourceType == ResourceType::Sound)
		{
			RAYLIB::SetSoundVolume(soundPtr->getInstance().sound, volume);
			RAYLIB::SetSoundPan(soundPtr->getInstance().sound, panning);
			RAYLIB::SetSoundPitch(soundPtr->getInstance().sound, pitch);
		}

		if (!wasPlaying)
		{
			if (resourceType == ResourceType::Sound)
			{
				RAYLIB::PlaySound(soundPtr->getInstance().sound);
			}
			else if (resourceType == ResourceType::Music)
			{
				RAYLIB::PlayMusicStream(soundPtr->getInstance().music);
			}

			wasPlaying = true;
		}
	}
	else
	{
		if (wasPlaying)
		{
			if (resourceType == ResourceType::Sound)
				RAYLIB::StopSound(soundPtr->getInstance().sound);
			else if (resourceType == ResourceType::Music)
				RAYLIB::StopMusicStream(soundPtr->getInstance().music);

			wasPlaying = false;
		}
	}
}


void AudioSource::Play()
{
	wasPlaying = false;
	isPlaying = true;
}

void AudioSource::Stop()
{
	wasPlaying = true;
	isPlaying = false;
}