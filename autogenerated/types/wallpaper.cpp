// Generated by APIGenerator 1.0
// DO NOT EDIT!!!

#include "wallpaper.h"


WallPaper::WallPaper(QObject* parent) : TelegramObject(parent)
{
	this->_id = 0;
	this->_color = 0;
	this->_bg_color = 0;
}

void WallPaper::read(MTProtoStream* mtstream) 
{
	this->_constructorid = mtstream->readTLConstructor();
	
	Q_ASSERT((this->_constructorid == WallPaper::CtorWallPaper) ||
		 (this->_constructorid == WallPaper::CtorWallPaperSolid));
	
	if(this->_constructorid == WallPaper::CtorWallPaper)
	{
		this->_id = mtstream->readTLInt();
		this->_title = mtstream->readTLString();
		mtstream->readTLVector<PhotoSize>(this->_sizes, false, this);
		this->_color = mtstream->readTLInt();
	}
	else if(this->_constructorid == WallPaper::CtorWallPaperSolid)
	{
		this->_id = mtstream->readTLInt();
		this->_title = mtstream->readTLString();
		this->_bg_color = mtstream->readTLInt();
		this->_color = mtstream->readTLInt();
	}
}

void WallPaper::write(MTProtoStream* mtstream) 
{
	Q_ASSERT((this->_constructorid == WallPaper::CtorWallPaper) ||
		 (this->_constructorid == WallPaper::CtorWallPaperSolid));
	
	this->compileFlags();
	mtstream->writeTLConstructor(this->_constructorid);
	
	if(this->_constructorid == WallPaper::CtorWallPaper)
	{
		mtstream->writeTLInt(this->_id);
		mtstream->writeTLString(this->_title);
		mtstream->writeTLVector(this->_sizes, false);
		mtstream->writeTLInt(this->_color);
	}
	else if(this->_constructorid == WallPaper::CtorWallPaperSolid)
	{
		mtstream->writeTLInt(this->_id);
		mtstream->writeTLString(this->_title);
		mtstream->writeTLInt(this->_bg_color);
		mtstream->writeTLInt(this->_color);
	}
}

void WallPaper::compileFlags() 
{
	
}

TLInt WallPaper::id() const
{
	return this->_id;
}

void WallPaper::setId(TLInt id) 
{
	if(this->_id == id)
		return;

	this->_id = id;
	emit idChanged();
}

TLString WallPaper::title() const
{
	return this->_title;
}

void WallPaper::setTitle(TLString title) 
{
	if(this->_title == title)
		return;

	this->_title = title;
	emit titleChanged();
}

const TLVector<PhotoSize*>& WallPaper::sizes() const
{
	return this->_sizes;
}

void WallPaper::setSizes(const TLVector<PhotoSize*>& sizes) 
{
	if(this->_sizes == sizes)
		return;

	this->_sizes = sizes;
	emit sizesChanged();
}

TLInt WallPaper::color() const
{
	return this->_color;
}

void WallPaper::setColor(TLInt color) 
{
	if(this->_color == color)
		return;

	this->_color = color;
	emit colorChanged();
}

TLInt WallPaper::bgColor() const
{
	return this->_bg_color;
}

void WallPaper::setBgColor(TLInt bg_color) 
{
	if(this->_bg_color == bg_color)
		return;

	this->_bg_color = bg_color;
	emit bgColorChanged();
}

