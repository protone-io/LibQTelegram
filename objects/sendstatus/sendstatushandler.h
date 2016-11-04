#ifndef SENDSTATUSHANDLER_H
#define SENDSTATUSHANDLER_H

#define SendStatusHandler_instance SendStatusHandler::instance()
#define SendStatusHandler_hasSendStatus(dialog) SendStatusHandler::instance()->hasSendStatus(dialog)
#define SendStatusHandler_dialogSendStatus(dialog) SendStatusHandler::instance()->dialogSendStatus(dialog)

#include <QObject>
#include <QHash>
#include "../../autogenerated/types/types.h"
#include "sendstatusobject.h"

class SendStatusHandler : public QObject
{
    Q_OBJECT

    private:
        explicit SendStatusHandler(QObject *parent = 0);

    public:
        static SendStatusHandler* instance();
        bool hasSendStatus(Dialog* dialog) const;
        QString dialogSendStatus(Dialog* dialog) const;

    private slots:
        void onTyping(Update* update);
        void onSendStatusExpired();

    signals:
        void sendStatusUpdated(Dialog* dialog);

    private:
        QHash<Dialog*, SendStatusObject*> _dialogstatus;
        static SendStatusHandler* _instance;
};

#endif // SENDSTATUSHANDLER_H
