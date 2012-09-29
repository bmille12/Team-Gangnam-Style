#pragma once

#include "IComponent.h"

#include <fmod.hpp>

#include <string>

using namespace FMOD;
using namespace std;

class CAudioSourceComponent:public CBaseComponent
{
public:
	CAudioSourceComponent();
	~CAudioSourceComponent();

	void init();

	void setFilename(const string& name)
	{
		m_strFilename=name;
	};

	void play();

	void stop();
private:
	string m_strFilename;
	System* m_pAudioSystem;
	Sound *m_pSound;
	Channel *m_pChannel;
	bool m_bStream;

};