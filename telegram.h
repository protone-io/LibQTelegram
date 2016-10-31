#ifndef TELEGRAM_H
#define TELEGRAM_H

#include <QObject>
#include "objects/telegraminitializer.h"

class Telegram : public QObject
{
    Q_OBJECT

    Q_PROPERTY(TelegramInitializer* initializer READ initializer WRITE setInitializer NOTIFY initializerChanged)
    Q_PROPERTY(User* me READ me CONSTANT FINAL)
    Q_PROPERTY(int apiLayer READ apiLayer CONSTANT FINAL)

    public:
        explicit Telegram(QObject *parent = 0);
        TelegramInitializer* initializer() const;
        User* me() const;
        int apiLayer() const;
        void setInitializer(TelegramInitializer* initializer);

    public: // C++ side API
        QString dialogTitle(Dialog* dialog) const;
        QString dialogStatusText(Dialog* dialog) const;
        QString messagePreview(Message* message) const;
        QString messageText(Message* message) const;
        TelegramObject* messageFrom(Message *message) const;

    public slots: // Login
        void signIn(const QString& phonecode);
        void signUp(const QString& firstname, const QString& lastname, const QString& phonecode);
        void resendCode();

    private:
        QString messageMediaText(MessageMedia* messagemedia) const;
        QString messageActionText(Message *message) const;
    signals:
        void initializerChanged();
        void signUpRequested();
        void signInRequested();
        void loginCompleted();
        void floodWait(int seconds);
        void phoneCodeError(QString errormessage);

    private:
        TelegramInitializer* _initializer;
};

#endif // TELEGRAM_H
