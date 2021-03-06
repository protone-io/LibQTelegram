#ifndef INPUTBOTINLINEMESSAGEID_H
#define INPUTBOTINLINEMESSAGEID_H

// Generated by APIGenerator 1.0
// DO NOT EDIT!!!

#include "../../types/basic.h"
#include "../../types/telegramobject.h"

class InputBotInlineMessageID: public TelegramObject
{
	Q_OBJECT

	Q_PROPERTY(TLInt dcId READ dcId WRITE setDcId NOTIFY dcIdChanged)
	Q_PROPERTY(TLLong id READ id WRITE setId NOTIFY idChanged)
	Q_PROPERTY(TLLong accessHash READ accessHash WRITE setAccessHash NOTIFY accessHashChanged)

	Q_ENUMS(Constructors)

	public:
		enum Constructors {
			CtorInputBotInlineMessageID = 0x890c3d89,
		};

	public:
		explicit InputBotInlineMessageID(QObject* parent = 0);
		virtual void read(MTProtoStream* mtstream);
		virtual void write(MTProtoStream* mtstream);
	protected:
		virtual void compileFlags();
	public:
		TLInt dcId() const;
		void setDcId(TLInt dc_id);
		TLLong id() const;
		void setId(TLLong id);
		TLLong accessHash() const;
		void setAccessHash(TLLong access_hash);

	signals:
		void dcIdChanged();
		void idChanged();
		void accessHashChanged();

	private:
		TLInt _dc_id;
		TLLong _id;
		TLLong _access_hash;

};

#endif // INPUTBOTINLINEMESSAGEID_H
