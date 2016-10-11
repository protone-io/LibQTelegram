#include "filecache.h"
#include "../../autogenerated/telegramapi.h"
#include "../../types/telegramhelper.h"
#include "telegramcache.h"
#include <QCryptographicHash>
#include <QStandardPaths>
#include <QDir>

#define CACHE_FOLDER "cache"

FileCache* FileCache::_instance = NULL;

FileCache::FileCache(QObject *parent) : QObject(parent), _currentobject(NULL)
{
    this->_storagepath = QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + QDir::separator() + CACHE_FOLDER;

    QDir dir;
    dir.mkpath(this->_storagepath);
}

FileCache *FileCache::instance()
{
    if(!FileCache::_instance)
        FileCache::_instance = new FileCache();

    return FileCache::_instance;
}

FileObject *FileCache::fileObject(Dialog *dialog)
{
    TLInt id = TelegramHelper::identifier(dialog);

    if(TelegramHelper::isChat(dialog) || TelegramHelper::isChannel(dialog))
    {
        Chat* chat = TelegramCache_chat(id);

        if(chat)
            return this->fileObject(chat);

        return NULL;
    }

    User* user = TelegramCache_user(id);

    if(!user)
        return NULL;

    return this->fileObject(user);
}

FileObject *FileCache::fileObject(User *user)
{
    if(!user->photo() || (user->photo()->constructorId() == TLTypes::UserProfilePhotoEmpty))
        return NULL;

    UserProfilePhoto* userprofilephoto = user->photo();
    return this->fileObject(userprofilephoto->photoSmall(), userprofilephoto->photoBig());
}

FileObject *FileCache::fileObject(Chat *chat)
{
    if(!chat->photo() || (chat->photo()->constructorId() == TLTypes::ChatPhotoEmpty))
        return NULL;

    ChatPhoto* chatphoto = chat->photo();
    return this->fileObject(chatphoto->photoSmall(), chatphoto->photoBig());
}

QString FileCache::createFileId(FileLocation *filelocation)
{
    QByteArray indata, outdata;
    TLLong id = filelocation->localId();

    indata.append(reinterpret_cast<const char*>(&id), sizeof(TLLong));
    outdata = QCryptographicHash::hash(indata, QCryptographicHash::Md5);
    return outdata.toHex();
}

FileObject *FileCache::fileObject(FileLocation *locthumbnail, FileLocation *locfile)
{
    QString fileid = this->createFileId(locfile);

    if(this->_filemap.contains(fileid))
        return this->_filemap[fileid];

    FileObject* fileobject = new FileObject(this->_storagepath, this);
    connect(fileobject, &FileObject::downloadCompleted, this, &FileCache::processQueue);

    fileobject->setThumbnailLocation(locthumbnail);
    fileobject->setFileLocation(locfile);
    fileobject->setThumbnailId(this->createFileId(locthumbnail));
    fileobject->setFileId(fileid);

    this->_filemap[fileid] = fileobject;

    if(fileobject->loadCache())
        return fileobject;

    this->_queue << fileobject;

    if(!this->_currentobject)
        this->processQueue();

    return fileobject;
}

void FileCache::processQueue()
{
    if(this->_queue.isEmpty())
    {
        this->_currentobject = NULL;
        return;
    }

    this->_currentobject = this->_queue.takeFirst();
    this->_currentobject->downloadThumbnail();
}