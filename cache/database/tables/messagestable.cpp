#include "messagestable.h"

MessagesTable::MessagesTable(QObject *parent) : DatabaseTable("messages", parent)
{

}

void MessagesTable::createSchema()
{
    this->createTable("id INTEGER PRIMARY KEY, dialogid INTEGER, date INTEGER, isout INTEGER, message BLOB", "message");
}

void MessagesTable::insertQuery(QSqlQuery &queryobj, TelegramObject *telegramobject)
{
    Message* message = qobject_cast<Message*>(telegramobject);

    QByteArray data;
    message->serialize(data);

    queryobj.bindValue(":id", TelegramHelper::identifier(message));
    queryobj.bindValue(":dialogid", TelegramHelper::messageToDialog(message));
    queryobj.bindValue(":date", message->date());
    queryobj.bindValue(":isout", message->isOut());
    queryobj.bindValue(":message", data);

    this->execute(queryobj);
}

void MessagesTable::removeDialogMessages(TLInt dialogid, TLVector<MessageId>& deletedmessages)
{
    if(!this->prepareDelete(dialogid, deletedmessages))
        return;

    CreateQuery(queryobj);

    if(!this->prepare(queryobj, "DELETE FROM " + this->name() + " WHERE dialogid = :dialogid"))
        return;

    queryobj.bindValue(":dialogid", dialogid);
    this->execute(queryobj);
}

Message *MessagesTable::topMessage(TLInt dialogid, QHash<MessageId, Message *> &messages, QObject *parent)
{
    CreateQuery(queryobj);

    if(!this->prepare(queryobj, "SELECT * FROM " + this->name() + " WHERE dialogid = :dialogid ORDER BY date DESC LIMIT 1"))
        return NULL;

    queryobj.bindValue(":dialogid", dialogid);

    if(!this->execute(queryobj) || !queryobj.first())
        return NULL;

    Message* message = this->loadMessage(queryobj, messages, parent);
    TLInt chatid = TelegramHelper::messageIsMigratedFrom(message);

    if(chatid)
        return this->topMessage(chatid, messages, parent);

    return message;
}

QList<Message *> MessagesTable::messagesForDialog(TLInt dialogid, QHash<MessageId, Message *> &messages, int offset, int limit, bool* hasmigration, QObject *parent) const
{
    QList<Message*> result;
    CreateQuery(queryobj);

    if(!this->prepare(queryobj, "SELECT * FROM " + this->name() + " WHERE dialogid = :dialogid ORDER BY id DESC LIMIT :limit OFFSET :offset"))
        return result;

    queryobj.bindValue(":dialogid", dialogid);
    queryobj.bindValue(":limit", limit);
    queryobj.bindValue(":offset", offset);

    if(!this->execute(queryobj))
        return result;

    this->loadMessages(queryobj, result, messages, hasmigration, parent);
    return result;
}

QList<Message *> MessagesTable::lastMessagesForDialog(Dialog *dialog, QHash<MessageId, Message *> &messages, QObject *parent) const
{
    QList<Message*> result;
    TLInt dialogid = TelegramHelper::identifier(dialog);
    MessageId minmsgid = TelegramHelper::identifier(dialog->readInboxMaxId(), dialog);

    CreateQuery(queryobj);

    if(!this->prepare(queryobj, "SELECT * FROM " + this->name() + " WHERE dialogid = :dialogid AND id >= :minmsgid ORDER BY date DESC"))
        return result;

    queryobj.bindValue(":dialogid", dialogid);
    queryobj.bindValue(":minmsgid", minmsgid);

    if(!this->execute(queryobj))
        return result;

    this->loadMessages(queryobj, result, messages, NULL, parent);
    return result;
}

TLInt MessagesTable::messagesCount(Dialog* dialog, TLInt minid, TLInt maxid) const
{
    CreateQuery(queryobj);

    TLInt messageminid = TelegramHelper::identifier(minid, dialog);
    TLInt messagemaxid = TelegramHelper::identifier(maxid, dialog);

    if(!this->prepare(queryobj, "SELECT id FROM " + this->name() + " WHERE dialogid = :dialogid AND isout = 0 AND id >= :minid AND id < :maxid"))
        return 0;

    TLInt dialogid = TelegramHelper::identifier(dialog);
    queryobj.bindValue(":dialogid", dialogid);
    queryobj.bindValue(":minid", messageminid);
    queryobj.bindValue(":maxid", messagemaxid);

    if(!this->execute(queryobj))
        return 0;

    int count = 0;

    while(queryobj.next())
        count++;

    return count;
}

bool MessagesTable::prepareDelete(TLInt dialogid, TLVector<MessageId> &deletedmessages)
{
    CreateQuery(queryobj);

    if(!this->prepare(queryobj, "SELECT id FROM " + this->name() + " WHERE dialogid = :dialogid"))
        return false;

    queryobj.bindValue(":dialogid", dialogid);

    if(!this->execute(queryobj))
        return false;

    while(queryobj.next())
    {
        MessageId messageid = queryobj.value("id").toLongLong();

        if(!deletedmessages.contains(messageid))
            continue;

        deletedmessages << messageid;
    }

    return queryobj.numRowsAffected() > 0;
}

void MessagesTable::loadMessages(QSqlQuery &queryobj, QList<Message*>& result, QHash<MessageId, Message *> &messages, bool* hasmigration, QObject *parent) const
{
    if(hasmigration)
        *hasmigration = false;

    while(queryobj.next())
    {
        MessageId messageid = queryobj.value("id").toLongLong();
        Message* message = NULL;

        if(!messages.contains(messageid))
        {
            QByteArray data = queryobj.value("message").toByteArray();
            message = new Message(parent);
            message->unserialize(data);
            messages[messageid] = message;
        }
        else
            message = messages[messageid];

        if(hasmigration && TelegramHelper::messageIsMigratedFrom(message))
            *hasmigration = true;

        result << message;
    }
}

Message *MessagesTable::loadMessage(QSqlQuery &queryobj, QHash<MessageId, Message *> &messages, QObject *parent) const
{
    MessageId messageid = queryobj.value("id").toLongLong();

    if(messages.contains(messageid))
        return messages[messageid];

    QByteArray data = queryobj.value("message").toByteArray();
    Message* message = new Message(parent);
    message->unserialize(data);

    messages[messageid] = message;
    return message;
}
