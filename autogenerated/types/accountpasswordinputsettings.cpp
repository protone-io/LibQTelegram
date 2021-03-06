// Generated by APIGenerator 1.0
// DO NOT EDIT!!!

#include "accountpasswordinputsettings.h"


AccountPasswordInputSettings::AccountPasswordInputSettings(QObject* parent) : TelegramObject(parent)
{
	this->_flags = 0;
	this->_constructorid = AccountPasswordInputSettings::CtorAccountPasswordInputSettings;
}

void AccountPasswordInputSettings::read(MTProtoStream* mtstream) 
{
	this->_constructorid = mtstream->readTLConstructor();
	
	Q_ASSERT((this->_constructorid == AccountPasswordInputSettings::CtorAccountPasswordInputSettings));
	
	if(this->_constructorid == AccountPasswordInputSettings::CtorAccountPasswordInputSettings)
	{
		this->_flags = mtstream->readTLInt();
		if(IS_FLAG_SET(this->_flags, 0))
			this->_new_salt = mtstream->readTLBytes();
		
		if(IS_FLAG_SET(this->_flags, 0))
			this->_new_password_hash = mtstream->readTLBytes();
		
		if(IS_FLAG_SET(this->_flags, 0))
			this->_hint = mtstream->readTLString();
		
		if(IS_FLAG_SET(this->_flags, 1))
			this->_email = mtstream->readTLString();
	}
}

void AccountPasswordInputSettings::write(MTProtoStream* mtstream) 
{
	Q_ASSERT((this->_constructorid == AccountPasswordInputSettings::CtorAccountPasswordInputSettings));
	
	this->compileFlags();
	mtstream->writeTLConstructor(this->_constructorid);
	
	if(this->_constructorid == AccountPasswordInputSettings::CtorAccountPasswordInputSettings)
	{
		mtstream->writeTLInt(this->_flags);
		if(IS_FLAG_SET(this->_flags, 0))
			mtstream->writeTLBytes(this->_new_salt);
		
		if(IS_FLAG_SET(this->_flags, 0))
			mtstream->writeTLBytes(this->_new_password_hash);
		
		if(IS_FLAG_SET(this->_flags, 0))
			mtstream->writeTLString(this->_hint);
		
		if(IS_FLAG_SET(this->_flags, 1))
			mtstream->writeTLString(this->_email);
	}
}

void AccountPasswordInputSettings::compileFlags() 
{
	this->_flags = 0;
	
	if(this->_constructorid == AccountPasswordInputSettings::CtorAccountPasswordInputSettings)
	{
		if(!this->_new_salt.isEmpty())
			SET_FLAG_BIT(this->_flags, 0);
		if(!this->_new_password_hash.isEmpty())
			SET_FLAG_BIT(this->_flags, 0);
		if(!this->_hint.isEmpty())
			SET_FLAG_BIT(this->_flags, 0);
		if(!this->_email.isEmpty())
			SET_FLAG_BIT(this->_flags, 1);
	}
}

TLInt AccountPasswordInputSettings::flags() const
{
	return this->_flags;
}

void AccountPasswordInputSettings::setFlags(TLInt flags) 
{
	if(this->_flags == flags)
		return;

	this->_flags = flags;
	emit flagsChanged();
}

TLBytes AccountPasswordInputSettings::newSalt() const
{
	return this->_new_salt;
}

void AccountPasswordInputSettings::setNewSalt(TLBytes new_salt) 
{
	if(this->_new_salt == new_salt)
		return;

	this->_new_salt = new_salt;
	emit newSaltChanged();
}

TLBytes AccountPasswordInputSettings::newPasswordHash() const
{
	return this->_new_password_hash;
}

void AccountPasswordInputSettings::setNewPasswordHash(TLBytes new_password_hash) 
{
	if(this->_new_password_hash == new_password_hash)
		return;

	this->_new_password_hash = new_password_hash;
	emit newPasswordHashChanged();
}

TLString AccountPasswordInputSettings::hint() const
{
	return this->_hint;
}

void AccountPasswordInputSettings::setHint(TLString hint) 
{
	if(this->_hint == hint)
		return;

	this->_hint = hint;
	emit hintChanged();
}

TLString AccountPasswordInputSettings::email() const
{
	return this->_email;
}

void AccountPasswordInputSettings::setEmail(TLString email) 
{
	if(this->_email == email)
		return;

	this->_email = email;
	emit emailChanged();
}

