#include "dc.h"
#include "../mtprotoupdatehandler.h"
#include "../mtprotoreply.h"
#include <QDateTime>
#include <QTimerEvent>

TLLong DC::_lastclientmsgid = 0;

DC::DC(DCConfig *dcconfig, bool filedc, QObject *parent): DCConnection(dcconfig, parent), _mtdecompiler(NULL), _savedrequest(NULL), _lastpacketlen(0), _contentmsgno(-1), _lastmsgid(0), _ownedsessions(0), _timcloseconnection(0), _filedc(filedc)
{
    this->_mtservicehandler = new MTProtoServiceHandler(dcconfig, this);

    connect(this->_mtservicehandler, &MTProtoServiceHandler::ackRequest, this, &DC::onAckRequest);
    connect(this->_mtservicehandler, &MTProtoServiceHandler::migrateDC, this, &DC::migrateDC);
    connect(this->_mtservicehandler, &MTProtoServiceHandler::sessionPasswordNeeded, this, &DC::sessionPasswordNeeded);
    connect(this->_mtservicehandler, &MTProtoServiceHandler::invalidPassword, this, &DC::invalidPassword);
    connect(this->_mtservicehandler, &MTProtoServiceHandler::phoneCodeError, this, &DC::phoneCodeError);
    connect(this->_mtservicehandler, &MTProtoServiceHandler::saltChanged, this, &DC::onServerSaltChanged);
    connect(this->_mtservicehandler, &MTProtoServiceHandler::deltaTimeChanged, this, &DC::onDeltaTimeChanged);
    connect(this->_mtservicehandler, &MTProtoServiceHandler::ack, this, &DC::onAck);
    connect(this->_mtservicehandler, &MTProtoServiceHandler::floodLock, this, &DC::onDcFloodClock);
    connect(this->_mtservicehandler, &MTProtoServiceHandler::unauthorized, this, &DC::onDCUnauthorized);
    connect(this->_mtservicehandler, SIGNAL(serviceHandled(MTProtoReply*)), this, SLOT(handleReply(MTProtoReply*)));

    connect(this, &DC::connected, this, &DC::onDCConnected);
    connect(this, &DC::readyRead, this, &DC::onDCReadyRead);
}

bool DC::fileDc() const
{
    return this->_filedc;
}

MTProtoRequest *DC::lastRequest() const
{
    if(!this->_pendingrequests.contains(this->_lastmsgid))
        return NULL;

    return this->_pendingrequests[this->_lastmsgid];
}

void DC::sendPlain(MTProtoStream *mtstream)
{
    MTProtoRequest req(this->_dcconfig);
    req.setBody(mtstream); // Take ownership

    this->send(&req);
}

TLInt DC::generateContentMsgNo()
{
    this->_contentmsgno = this->_contentmsgno + 2;
    return this->_contentmsgno;
}

void DC::assignMessageId(MTProtoRequest* req)
{
    TLLong unixtime = (CurrentDeltaTime(this->_dcconfig->deltaTime()) << 32LL) + (CurrentTimeStampMillis & 0xFFFFFFF0);
    TLLong msgid = 0, ticks = 4 - (unixtime % 4);

    if(!(unixtime % 4))
        msgid = unixtime;
    else
        msgid = unixtime + ticks;

    DC::_lastclientmsgid = this->_lastmsgid = msgid = qMax(msgid, DC::_lastclientmsgid + 4);

    if((msgid % 4) != 0)
        qFatal("Message %llx not divisible by 4 (yields %lld)", msgid, msgid % 4);

    req->setMessageId(msgid);
}

void DC::checkSyncronization(MTProtoReply *mtreply)
{
    TLLong servertime = ServerTime(mtreply->messageId());
    TLLong clienttime = CurrentDeltaTime(this->_dcconfig->deltaTime());

    if(clienttime <= (servertime - 30))
        qWarning("DC %d Message %llx has a date at least 30 seconds in the future than current date", this->_dcconfig->dcid(), mtreply->messageId());
    else if(clienttime >= (servertime + 300))
        qWarning("DC %d Message %llx was sent at least 300 seconds ago", this->_dcconfig->dcid(), mtreply->messageId());
}

void DC::decompile(int direction, TLLong messageid, const QByteArray& body)
{
    if(!TelegramConfig::config()->debugMode())
        return;

    if(!this->_mtdecompiler)
        this->_mtdecompiler = new MTProtoDecompiler();

    this->_mtdecompiler->decompile(this->_dcconfig->dcid(), direction, messageid, body);
}

TLInt DC::getPacketLength()
{
    uchar len = 0;
    this->getChar(reinterpret_cast<char*>(&len));

    if(len < 0x7F)
        return len * 4;
    else if(len == 0x7F)
    {
        TLInt packetlength = 0;
        this->read(reinterpret_cast<char*>(&packetlength), 3);
        return packetlength * 4;
    }

    qFatal("DC %d Incorrect TCP Package", this->_dcconfig->dcid());
    return 0;
}

void DC::repeatRequest(TLLong msgid)
{
    if(!this->_pendingrequests.contains(msgid))
    {
        qDebug("DC %d Expired request %llx...", this->_dcconfig->dcid(), msgid);
        return;
    }

    qDebug("DC %d Repeating request %llx...", this->_dcconfig->dcid(), msgid);

    MTProtoRequest* req = this->_pendingrequests.take(msgid);
    req->setAcked(false);
    this->send(req);
}

void DC::onDCConnected()
{
    this->_contentmsgno = -1; // Init connection to DC
    MTProtoRequest::resetFirst(this->_dcconfig->dcid());
}

void DC::onDCUnauthorized()
{
    // Reset authorization state
    this->_dcconfig->reset();

    qDeleteAll(this->_pendingrequests);
    this->_pendingrequests.clear();

    this->abort();
    emit unauthorized();
}

void DC::onDcFloodClock(int seconds)
{
    this->_pendingrequests.clear();

    this->abort();
    emit floodLock(seconds);
}

void DC::handleReply(const QByteArray &message)
{
    MTProtoReply mtreply(message, this->_dcconfig);

    if(mtreply.isError())
    {
        qWarning("DC %d ERROR %d", this->_dcconfig->dcid(), qAbs(mtreply.errorCode()));

        if(!this->_pendingrequests.contains(this->_lastmsgid))
            return;

        MTProtoRequest* req = this->_pendingrequests.take(this->_lastmsgid);
        req->setAcked(true);
        req->error();
        req->deleteLater();
        return;
    }

    if(((mtreply.messageId() % 4) != 1) && ((mtreply.messageId() % 4) != 3))
        qFatal("DC %d Invalid server Message %llx (yields %lld, instead of 1 or 3)", this->_dcconfig->dcid(), mtreply.messageId(), mtreply.messageId() % 4);

    this->handleReply(&mtreply);
}

void DC::handleReply(MTProtoReply *mtreply)
{
    if(this->_mtservicehandler->handle(mtreply))
        return;

    MTProtoRequest* req = this->_pendingrequests.take(mtreply->messageId());

    if(req)
        req->setAcked(true);

    if(this->_dcconfig->authorization() >= DCConfig::Authorized)
    {
        this->checkSyncronization(mtreply);
        bool handled = UpdateHandler_instance->handle(mtreply);

        if(!handled)
            this->decompile(MTProtoDecompiler::DIRECTION_IN, mtreply->messageId(), mtreply->cbody());
        else
            mtreply->seekToBody(); // Prepare buffer to be handled in "replied" slots

        if(req)
            emit req->replied(mtreply);
        else if(!handled)
            qWarning("DC %d Request for msg_id %llx not found", this->_dcconfig->dcid(), mtreply->messageId());
    }
    else
        emit authorizationReply(mtreply);

    if(req)
        req->deleteLater();
}

void DC::onAck(const TLVector<TLLong> &msgids)
{
    foreach(TLLong msgid, msgids)
    {
        if(!this->_pendingrequests.contains(msgid))
            continue;

        MTProtoRequest* req = this->_pendingrequests[msgid];
        req->setAcked(true);

        qDebug("DC %d ACK request %llx", this->_dcconfig->dcid(), msgid);
    }
}

void DC::onAckRequest(TLLong reqmsgid)
{
    if(!this->_pendingrequests.contains(reqmsgid))
    {
        qWarning("DC %d Cannot ACK request %llx", this->_dcconfig->dcid(), reqmsgid);
        return;
    }

    this->_pendingrequests[reqmsgid]->setAcked(true); // Don't repeat, we have received a reply
}

void DC::onServerSaltChanged(TLLong newserversalt, TLLong reqmsgid)
{
    this->_dcconfig->setServerSalt(newserversalt);
    TelegramConfig_save;

    this->repeatRequest(reqmsgid);
}

void DC::onDeltaTimeChanged(TLLong deltatime, TLLong reqmsgid)
{
    this->_dcconfig->setDeltaTime(deltatime);
    TelegramConfig_save;

    DC::_lastclientmsgid = 0;
    this->repeatRequest(reqmsgid);
}

void DC::send(MTProtoRequest *req)
{
    if(this->state() != DC::ConnectedState)
    {
        qWarning("DC %d Not connected, cannot send queries", this->_dcconfig->dcid());
        return;
    }

    this->assignMessageId(req);

    if(req->encrypted())
    {
        this->_pendingrequests[req->messageId()] = req;

        if(this->_dcconfig->authorization() < DCConfig::Authorized)
        {
            qWarning("DC %d Cannot send encrypted requests", this->_dcconfig->dcid());
            return;
        }

        req->setSeqNo(this->generateContentMsgNo());
    }

    QByteArray reqpayload = req->build();
    this->decompile(MTProtoDecompiler::DIRECTION_OUT, req->messageId(), req->body());

    if(req->encrypted())
    {
        connect(req, &MTProtoRequest::timeout, this, &DC::repeatRequest, Qt::UniqueConnection);
        req->startTimer(QueryTimeout);
    }

    this->write(reqpayload);
}

void DC::keepRequest(MTProtoRequest *req)
{
    if(!req)
    {
        this->_savedrequest = NULL;
        return;
    }

    req->setAcked(false);
    req->setDcConfig(this->_dcconfig);

    this->_savedrequest = req;
}

MTProtoRequest *DC::giveRequest()
{
    MTProtoRequest* req = this->_savedrequest;

    this->_savedrequest = NULL;
    return req;
}

void DC::addSessionRef()
{
    this->_ownedsessions++;
}

void DC::removeSessionRef()
{
    this->_ownedsessions--;

    Q_ASSERT(this->_ownedsessions >= 0);

    if(this->_ownedsessions == 0)
    {
        if((!this->_filedc && (DCConfig_mainConfig->id() == this->_dcconfig->id())) || (this->_timcloseconnection > 0)) // Main DC or timer is already running
            return;

        if(this->_pendingrequests.count() > 0)
            this->freeOwnedRequests();

        this->_timcloseconnection = this->startTimer(CloseDCTimeout);
    }
}

void DC::timerEvent(QTimerEvent *event)
{
    DCConnection::timerEvent(event);

    if(event->timerId() == this->_timcloseconnection)
    {
        if(this->_pendingrequests.count() <= 0)
            this->close();

        killTimer(event->timerId());
        this->_timcloseconnection = 0;
    }
}

void DC::freeOwnedRequests()
{
    QList<MTProtoRequest*> requests = this->_pendingrequests.values();

    foreach(MTProtoRequest* req, requests)
    {
        if(req->config() != this->_dcconfig)
            continue;

        req->deleteLater();
    }
}

void DC::onDCReadyRead()
{
    while(this->bytesAvailable())
    {
        TLInt packetlen = this->_lastpacketlen;  // Store message length (if any)

        if(!packetlen)
            packetlen = this->getPacketLength(); // First message: read length

        if(packetlen < 4)
        {
            qFatal("DC %d Invalid TCP package, length: %d", this->_dcconfig->dcid(), packetlen);
            return;
        }

        if(this->bytesAvailable() < packetlen)   // Incomplete message: store message length and wait for more data
        {
            this->_lastpacketlen = packetlen;
            return;
        }

        QByteArray message = this->read(packetlen);
        this->handleReply(message);
        this->_lastpacketlen = 0;
    }
}
