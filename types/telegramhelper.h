#ifndef TELEGRAMHELPER_H
#define TELEGRAMHELPER_H

#include "basic.h"
#include "../autogenerated/types/types.h"
#include "../types/time.h"

class TelegramHelper
{
    private:
        TelegramHelper();

    public:
        static Message* createMessage(Updates *updates, User *me);
        static Message* createMessage(const QString& text, User *me, Peer *peer);
        static Dialog* createDialog(User* user, QObject* parent = 0);
        static InputFileLocation* inputFileLocation(FileLocation *filelocation);
        static InputFileLocation* inputFileLocation(Document* document);
        static InputNotifyPeer* inputNotifyPeer(Dialog* dialog, TLLong accesshash = 0, QObject* parent = 0);
        static InputPeerNotifySettings* inputPeerNotifySettings(PeerNotifySettings* notifysettings, QObject* parent = 0);
        static InputPeer* inputPeer(Dialog* dialog, TLLong accesshash = 0, QObject* parent = 0);
        static InputPeer* inputPeer(Peer* peer, TLLong accesshash = 0, QObject* parent = 0);
        static InputPeer* inputPeer(Message* message, QObject* parent = 0);
        static InputUser* inputUser(User* user, QObject* parent = 0);
        static PhotoSize* photoSmall(Photo* photo);
        static PhotoSize* photoBig(Photo* photo);
        static ChatPhoto* chatPhoto(Photo* photo, QObject *parent = 0);
        static QString dateString(TLInt timestamp);
        static QString fileSize(TLDouble size);
        static QString fullName(User* user);
        static QString statusText(User* user);
        static DocumentAttribute* documentHas(Document* document, TLConstructor attributector);
        static bool isSticker(Document* document);
        static bool isAnimated(Document* document);
        static bool isFile(Document* document);
        static bool isCloud(Dialog* dialog);
        static bool isChat(Dialog* dialog);
        static bool isChannel(Dialog* dialog);
        static bool isChat(Peer* peer);
        static bool isChannel(Peer* peer);
        static bool messageIsWebPagePending(Message* message);
        static bool messageIsAction(Message* message);
        static MessageId identifier(TLInt messageid, TLInt channelid);
        static MessageId identifier(TLInt messageid, Dialog* dialog);
        static MessageId identifier(Message* message);
        static TLInt identifier(User* user);
        static TLInt identifier(Chat* chat);
        static TLInt identifier(Peer* peer);
        static TLInt identifier(Dialog* dialog);
        static TLInt messageToDialog(Message* message);
        static TLBytes createPasswordHash(const QString& password, const AccountPassword *accountpassword);
};

#endif // TELEGRAMHELPER_H
