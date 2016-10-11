#ifndef MESSAGESMODEL_H
#define MESSAGESMODEL_H

#include "abstract/telegrammodel.h"

class MessagesModel : public TelegramModel
{
    Q_OBJECT

    public:
        explicit MessagesModel(QObject *parent = 0);
        virtual QVariant data(const QModelIndex &index, int role) const;
        virtual int rowCount(const QModelIndex &) const;

    private:
        void sortMessages();
        void telegramReady();

    private:
        QList<Message*> _messages;
};

#endif // MESSAGESMODEL_H