#pragma once

#include "AudioManager.h"

#include <unordered_map>
#include <vector>

#include "string"

template <typename Event, typename Group>
class AudioMap
{

private:
	std::unordered_map<Event, std::vector<SoundHandle>> m_sounds;
	std::unordered_map<Group, SoundGroupHandle> m_soundsGroups;
	AudioManager& m_audioManager = AudioManager::getInstance();

public:
	bool addSound(Event _trigger, std::vector<const char*> _paths)
	{
		for (const char* path : _paths)
		{
			std::string fullPath = "../../../";
			fullPath += path;
			int handle = m_audioManager.load(fullPath.c_str());
			if (handle == -1)
				return false;
			m_sounds[_trigger].push_back(handle);
		}
		return true;
	}

	bool addSound(Event _trigger, std::vector<const char*> _paths, Group _group)
	{

		if (m_soundsGroups.contains(_group) == false)
			m_soundsGroups[_group] = m_soundsGroups.size() - 1;

		for (const char* path : _paths)
		{
			int handle = m_audioManager.load(path, m_soundsGroups[_group]);
			if (handle == -1)
				return false;
			m_sounds[_trigger].push_back(handle);
		}
		return true;
	}

	bool playSound(Event _trigger, uint64_t _framedelay = 0)
	{
		if (m_sounds.contains(_trigger) == false)
			return false;

		SoundHandle handle = rand() % (m_sounds[_trigger].size());
		m_audioManager.play(m_sounds[_trigger][handle], _framedelay);
		return true;
	}

	bool stopSound(Event _trigger, uint64_t _framedelay = 0)
	{
		if (m_sounds.contains(_trigger) == false)
			return false;
		for (SoundHandle handle : m_sounds[_trigger]) {
			m_audioManager.stop(handle, _framedelay);
		}
		return true;
	}

	void setGeneraleVolume(float _volume)
	{
		m_audioManager.setGeneralVolume(_volume);
	}

	void setVolume(Event _trigger, float _volume)
	{
		if (m_sounds.contains(_trigger) == false)
			return;
		for (SoundHandle handle : m_sounds[_trigger]) {
			m_audioManager.setVolume(handle, _volume);
		}
	}

	void setGroupVolume(Group _group, float _volume)
	{
		if (m_soundsGroups.contains(_group) == false)
			return;
		m_audioManager.setGroupVolume(m_soundsGroups[_group], _volume);
	}
};
