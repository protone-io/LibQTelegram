// Generated by APIGenerator 1.0
// DO NOT EDIT!!!

#include "helpappupdate.h"


HelpAppUpdate::HelpAppUpdate(QObject* parent) : TelegramObject(parent)
{
	this->_id = 0;
	this->_is_critical = false;
}

void HelpAppUpdate::read(MTProtoStream* mtstream) 
{
	this->_constructorid = mtstream->readTLConstructor();
	
	Q_ASSERT((this->_constructorid == HelpAppUpdate::CtorHelpAppUpdate) ||
		 (this->_constructorid == HelpAppUpdate::CtorHelpNoAppUpdate));
	
	if(this->_constructorid == HelpAppUpdate::CtorHelpAppUpdate)
	{
		this->_id = mtstream->readTLInt();
		this->_is_critical = mtstream->readTLBool();
		this->_url = mtstream->readTLString();
		this->_text = mtstream->readTLString();
	}
}

void HelpAppUpdate::write(MTProtoStream* mtstream) 
{
	Q_ASSERT((this->_constructorid == HelpAppUpdate::CtorHelpAppUpdate) ||
		 (this->_constructorid == HelpAppUpdate::CtorHelpNoAppUpdate));
	
	this->compileFlags();
	mtstream->writeTLConstructor(this->_constructorid);
	
	if(this->_constructorid == HelpAppUpdate::CtorHelpAppUpdate)
	{
		mtstream->writeTLInt(this->_id);
		mtstream->writeTLBool(this->_is_critical);
		mtstream->writeTLString(this->_url);
		mtstream->writeTLString(this->_text);
	}
}

void HelpAppUpdate::compileFlags() 
{
	
}

TLInt HelpAppUpdate::id() const
{
	return this->_id;
}

void HelpAppUpdate::setId(TLInt id) 
{
	if(this->_id == id)
		return;

	this->_id = id;
	emit idChanged();
}

TLBool HelpAppUpdate::isCritical() const
{
	return this->_is_critical;
}

void HelpAppUpdate::setIsCritical(TLBool is_critical) 
{
	if(this->_is_critical == is_critical)
		return;

	this->_is_critical = is_critical;
	emit isCriticalChanged();
}

TLString HelpAppUpdate::url() const
{
	return this->_url;
}

void HelpAppUpdate::setUrl(TLString url) 
{
	if(this->_url == url)
		return;

	this->_url = url;
	emit urlChanged();
}

TLString HelpAppUpdate::text() const
{
	return this->_text;
}

void HelpAppUpdate::setText(TLString text) 
{
	if(this->_text == text)
		return;

	this->_text = text;
	emit textChanged();
}

