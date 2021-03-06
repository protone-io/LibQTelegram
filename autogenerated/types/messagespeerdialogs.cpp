// Generated by APIGenerator 1.0
// DO NOT EDIT!!!

#include "messagespeerdialogs.h"


MessagesPeerDialogs::MessagesPeerDialogs(QObject* parent) : TelegramObject(parent)
{
	this->_state = NULL;
	this->_constructorid = MessagesPeerDialogs::CtorMessagesPeerDialogs;
}

void MessagesPeerDialogs::read(MTProtoStream* mtstream) 
{
	this->_constructorid = mtstream->readTLConstructor();
	
	Q_ASSERT((this->_constructorid == MessagesPeerDialogs::CtorMessagesPeerDialogs));
	
	if(this->_constructorid == MessagesPeerDialogs::CtorMessagesPeerDialogs)
	{
		mtstream->readTLVector<Dialog>(this->_dialogs, false, this);
		mtstream->readTLVector<Message>(this->_messages, false, this);
		mtstream->readTLVector<Chat>(this->_chats, false, this);
		mtstream->readTLVector<User>(this->_users, false, this);
		TLInt state_ctor = mtstream->peekTLConstructor();
		
		if(state_ctor != TLTypes::Null)
		{
			this->resetTLType<UpdatesState>(&this->_state);
			this->_state->read(mtstream);
		}
		else
		{
			this->nullTLType<UpdatesState>(&this->_state);
			mtstream->readTLConstructor(); // Skip Null
		}
	}
}

void MessagesPeerDialogs::write(MTProtoStream* mtstream) 
{
	Q_ASSERT((this->_constructorid == MessagesPeerDialogs::CtorMessagesPeerDialogs));
	
	this->compileFlags();
	mtstream->writeTLConstructor(this->_constructorid);
	
	if(this->_constructorid == MessagesPeerDialogs::CtorMessagesPeerDialogs)
	{
		mtstream->writeTLVector(this->_dialogs, false);
		mtstream->writeTLVector(this->_messages, false);
		mtstream->writeTLVector(this->_chats, false);
		mtstream->writeTLVector(this->_users, false);
		if(this->_state != NULL)
			this->_state->write(mtstream);
		else
			mtstream->writeTLConstructor(TLTypes::Null);
	}
}

void MessagesPeerDialogs::compileFlags() 
{
	
}

const TLVector<Dialog*>& MessagesPeerDialogs::dialogs() const
{
	return this->_dialogs;
}

void MessagesPeerDialogs::setDialogs(const TLVector<Dialog*>& dialogs) 
{
	if(this->_dialogs == dialogs)
		return;

	this->_dialogs = dialogs;
	emit dialogsChanged();
}

const TLVector<Message*>& MessagesPeerDialogs::messages() const
{
	return this->_messages;
}

void MessagesPeerDialogs::setMessages(const TLVector<Message*>& messages) 
{
	if(this->_messages == messages)
		return;

	this->_messages = messages;
	emit messagesChanged();
}

const TLVector<Chat*>& MessagesPeerDialogs::chats() const
{
	return this->_chats;
}

void MessagesPeerDialogs::setChats(const TLVector<Chat*>& chats) 
{
	if(this->_chats == chats)
		return;

	this->_chats = chats;
	emit chatsChanged();
}

const TLVector<User*>& MessagesPeerDialogs::users() const
{
	return this->_users;
}

void MessagesPeerDialogs::setUsers(const TLVector<User*>& users) 
{
	if(this->_users == users)
		return;

	this->_users = users;
	emit usersChanged();
}

UpdatesState* MessagesPeerDialogs::state() const
{
	return this->_state;
}

void MessagesPeerDialogs::setState(UpdatesState* state) 
{
	if(this->_state == state)
		return;

	this->deleteChild(this->_state);
	this->_state = state;

	if(this->_state)
		this->_state->setParent(this);

	emit stateChanged();
}

