#include "fileuploader.h"
#include "../../autogenerated/telegramapi.h"
#include "../../crypto/hash.h"
#include "../../crypto/math.h"
#include <QFile>
#include <QUrl>

#define BlockSize    (32 * 1024)
#define TenMegaBytes (10 * 1024 * 1024)

FileUploader::FileUploader(QObject *parent) : QObject(parent), _dcsession(NULL), _mediatype(0), _isbigfile(false), _fileid(0), _partsize(BlockSize), _partscount(0), _partnum(0)
{

}

FileUploader::~FileUploader()
{
    if(this->_dcsession)
    {
        DC_CloseSession(this->_dcsession);
        this->_dcsession = NULL;
    }

    if(this->_file.isOpen())
        this->_file.close();
}

TLLong FileUploader::fileId() const
{
    return this->_fileid;
}

QString FileUploader::caption() const
{
    return this->_caption;
}

QString FileUploader::fileName() const
{
    return this->_filename;
}

QString FileUploader::md5hash() const
{
    return this->_md5hash;
}

TLInt FileUploader::partsCount() const
{
    return this->_partscount;
}

int FileUploader::mediaType() const
{
    return this->_mediatype;
}

bool FileUploader::isBigFile() const
{
    return this->_isbigfile;
}

void FileUploader::setCaption(const QString &caption)
{
    this->_caption = caption;
}

void FileUploader::setMediaType(int mediatype)
{
    this->_mediatype = mediatype;
}

void FileUploader::upload(const QUrl &filepath)
{
    QFileInfo fileinfo(filepath.toLocalFile());

    if(!this->calculatePartsLength(&fileinfo))
        return;

    this->calculatePartsCount(&fileinfo);

    this->_isbigfile = (fileinfo.size() > TenMegaBytes);
    this->_filename = fileinfo.fileName();
    this->_fileid = Math::randomize<TLLong>();
    this->_file.setFileName(filepath.toLocalFile());

    if(!this->_file.open(QFile::ReadWrite))
    {
        qWarning("UPLOAD Cannot open %s", qUtf8Printable(filepath.toLocalFile()));
        emit failed();
        return;
    }

    if(!this->_isbigfile)
    {
        this->_md5hash = md5_hash_hex(this->_file.readAll());
        this->_file.reset();
    }

    this->_dcsession = DC_CreateFileSession(DCConfig_mainConfig);
    connect(this->_dcsession, &DCSession::ready, this, &FileUploader::uploadPart);
    DC_InitializeSession(this->_dcsession);
}

bool FileUploader::calculatePartsLength(const QFileInfo *fileinfo)
{
    TLInt val = (fileinfo->size() + 2999) / 3000;

    while(this->_partsize < val)
        this->_partsize *= 2;

    if(this->_partsize > (512 << 10))
    {
        qWarning("UPLOAD File too big");
        emit failed();
        return false;
    }

    return true;
}

void FileUploader::calculatePartsCount(const QFileInfo* fileinfo)
{
    this->_partscount = fileinfo->size() / this->_partsize;

    if((this->_partscount * this->_partsize) < fileinfo->size())
        this->_partscount++;
}

void FileUploader::getNextPart(TLBytes &data)
{
    this->_file.seek(this->_partnum * this->_partsize);
    data = this->_file.read(this->_partsize);
}

void FileUploader::uploadPart()
{
    TLBytes partdata;
    this->getNextPart(partdata);

    MTProtoRequest* req = NULL;

    if(!this->_isbigfile)
        req = TelegramAPI::uploadSaveFilePart(this->_dcsession, this->_fileid, this->_partnum, partdata);
    else
        req = TelegramAPI::uploadSaveBigFilePart(this->_dcsession, this->_fileid, this->_partnum, this->_partscount, partdata);

    connect(req, &MTProtoRequest::replied, this, &FileUploader::onSaveFilePartReplied);
}

void FileUploader::onSaveFilePartReplied(MTProtoReply *mtreply)
{
    TLConstructor result = mtreply->readTLConstructor();

    if(result == TLTypes::BoolFalse)
    {
        qWarning("UPLOAD File transfer failed (part: %d/%d)", this->_partnum, this->_partscount);
        emit failed();
        return;
    }

    this->_partnum++;

    if(this->_partnum > this->_partscount)
    {
        emit completed();
        return;
    }

    this->uploadPart();
}
