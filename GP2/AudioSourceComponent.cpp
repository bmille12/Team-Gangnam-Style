#include "AudioSourceComponent.h"

CAudioSourceComponent::CAudioSourceComponent()
{
	m_pAudioSystem=NULL;
	m_pSound=NULL;
	m_bStream=false;
	m_pChannel=NULL;
	m_strName="AudioSourceComponent";
}

CAudioSourceComponent::~CAudioSourceComponent()
{
	if (m_pSound)
	{
		m_pSound->release();
		m_pSound=NULL;
	}
}

void CAudioSourceComponent::init()
{
	FMOD_RESULT result;
	if (m_bStream)
		result = m_pAudioSystem->createSound(m_strFilename.c_str(), FMOD_DEFAULT, 0, &m_pSound);
	else
		result = m_pAudioSystem->createStream(m_strFilename.c_str(), FMOD_DEFAULT, 0, &m_pSound);
	
}

void CAudioSourceComponent::play()
{
	m_pAudioSystem->playSound(FMOD_CHANNEL_FREE,m_pSound,false,&m_pChannel);
}

void CAudioSourceComponent::stop()
{
	if (m_pChannel)
	{
		m_pChannel->stop();
	}
}