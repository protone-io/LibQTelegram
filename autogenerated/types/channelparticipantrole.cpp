// Generated by APIGenerator 1.0
// DO NOT EDIT!!!

#include "channelparticipantrole.h"

ChannelParticipantRole::ChannelParticipantRole(QObject* parent) : TelegramObject(parent)
{
	
}

void ChannelParticipantRole::read(MTProtoStream* mtstream) 
{
	this->_constructorid = mtstream->readTLConstructor();
	
	Q_ASSERT((this->_constructorid == ChannelParticipantRole::CtorChannelRoleEmpty) ||
		 (this->_constructorid == ChannelParticipantRole::CtorChannelRoleModerator) ||
		 (this->_constructorid == ChannelParticipantRole::CtorChannelRoleEditor));
}

void ChannelParticipantRole::write(MTProtoStream* mtstream) 
{
	Q_ASSERT((this->_constructorid == ChannelParticipantRole::CtorChannelRoleEmpty) ||
		 (this->_constructorid == ChannelParticipantRole::CtorChannelRoleModerator) ||
		 (this->_constructorid == ChannelParticipantRole::CtorChannelRoleEditor));
	
	this->compileFlags();
	mtstream->writeTLConstructor(this->_constructorid);
}

void ChannelParticipantRole::compileFlags() 
{
	
}

