#ifndef FILEUPLOADER_H
#define FILEUPLOADER_H

#include <QObject>
#include <QFileInfo>
#include <QMimeDatabase>
#include "../../types/basic.h"
#include "../../mtproto/dc/dcsessionmanager.h"

class FileUploader : public QObject
{
    Q_OBJECT

    public:
        explicit FileUploader(QObject *parent = 0);
        ~FileUploader();
        TLLong fileId() const;
        QString caption() const;
        QString fileName() const;
        QString md5hash() const;
        QString mimeType() const;
        TLInt partsCount() const;
        bool isBigFile() const;
        void setCaption(const QString& caption);
        void upload(const QString &filepath);

    private:
        bool calculatePartsLength(const QFileInfo* fileinfo);
        void calculatePartsCount(const QFileInfo* fileinfo);
        void getNextPart(TLBytes &data);

    private slots:
        void uploadPart();
        void onSaveFilePartReplied(MTProtoReply* mtreply);

    signals:
        void failed();
        void pending();
        void completed();

    private:
        DCSession* _dcsession;
        QString _caption;
        QString _filename;
        QString _md5hash;
        QString _mimetype;
        bool _isbigfile;
        QFile _file;
        TLLong _fileid;
        TLLong _partsize;
        TLInt _partscount;
        TLInt _partnum;

    private:
        static QMimeDatabase _mimedb;
};

#endif // FILEUPLOADER_H