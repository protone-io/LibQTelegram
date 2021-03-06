#ifndef QQUICKMEDIAMESSAGEITEM_H
#define QQUICKMEDIAMESSAGEITEM_H

#include "qquickbaseitem.h"

class QQuickMediaMessageItem : public QQuickBaseItem
{
    Q_OBJECT

    Q_PROPERTY(Message* message READ message WRITE setMessage NOTIFY messageChanged)
    Q_PROPERTY(bool isSticker READ isSticker NOTIFY isStickerChanged)
    Q_PROPERTY(bool isAnimated READ isAnimated NOTIFY isAnimatedChanged)
    Q_PROPERTY(bool isAudio READ isAudio NOTIFY isAudioChanged)
    Q_PROPERTY(bool isVideo READ isVideo NOTIFY isVideoChanged)
    Q_PROPERTY(bool isImage READ isImage NOTIFY isImageChanged)
    Q_PROPERTY(bool isFile READ isFile NOTIFY isFileChanged)
    Q_PROPERTY(bool isWebPage READ isWebPage NOTIFY isWebPageChanged)
    Q_PROPERTY(qreal size READ size WRITE setSize NOTIFY sizeChanged)
    Q_PROPERTY(qreal contentWidth READ contentWidth NOTIFY contentWidthChanged)
    Q_PROPERTY(QString duration READ duration NOTIFY durationChanged)
    Q_PROPERTY(QString venueTitle READ venueTitle NOTIFY venueTitleChanged)
    Q_PROPERTY(QString venueAddress READ venueAddress NOTIFY venueAddressChanged)
    Q_PROPERTY(QString webPageTitle READ webPageTitle NOTIFY webPageTitleChanged)
    Q_PROPERTY(QString webPageDescription READ webPageDescription NOTIFY webPageDescriptionChanged)
    Q_PROPERTY(QString webPageUrl READ webPageUrl NOTIFY webPageUrlChanged)
    Q_PROPERTY(qreal webPageThumbnailHeight READ webPageThumbnailHeight NOTIFY webPageThumbnailHeightChanged)
    Q_PROPERTY(QString videoThumbnail READ videoThumbnail NOTIFY videoThumbnailChanged)
    Q_PROPERTY(bool webPageHasPhoto READ webPageHasPhoto NOTIFY webPageHasPhotoChanged)
    Q_PROPERTY(GeoPoint* geoPoint READ geoPoint NOTIFY geoPointChanged)
    Q_PROPERTY(QQmlComponent* imageDelegate READ imageDelegate WRITE setImageDelegate NOTIFY imageDelegateChanged)
    Q_PROPERTY(QQmlComponent* stickerDelegate READ stickerDelegate WRITE setStickerDelegate NOTIFY stickerDelegateChanged)
    Q_PROPERTY(QQmlComponent* animatedDelegate READ animatedDelegate WRITE setAnimatedDelegate NOTIFY animatedDelegateChanged)
    Q_PROPERTY(QQmlComponent* locationDelegate READ locationDelegate WRITE setLocationDelegate NOTIFY locationDelegateChanged)
    Q_PROPERTY(QQmlComponent* webPageDelegate READ webPageDelegate WRITE setWebPageDelegate NOTIFY webPageDelegateChanged)
    Q_PROPERTY(QQmlComponent* audioDelegate READ audioDelegate WRITE setAudioDelegate NOTIFY audioDelegateChanged)
    Q_PROPERTY(QQmlComponent* fileDelegate READ fileDelegate WRITE setFileDelegate NOTIFY fileDelegateChanged)

    public:
        QQuickMediaMessageItem(QQuickItem *parent = 0);
        Message* message() const;
        bool isSticker() const;
        bool isAnimated() const;
        bool isAudio() const;
        bool isVideo() const;
        bool isImage() const;
        bool isFile() const;
        bool isWebPage() const;
        qreal size() const;
        qreal contentWidth() const;
        QString duration() const;
        QString venueTitle() const;
        QString venueAddress() const;
        QString webPageTitle() const;
        QString webPageDescription() const;
        QString webPageUrl() const;
        qreal webPageThumbnailHeight() const;
        QString videoThumbnail() const;
        bool webPageHasPhoto() const;
        GeoPoint* geoPoint() const;
        void setMessage(Message* message);
        void setSize(qreal size);

    public:
        QQmlComponent* imageDelegate() const;
        void setImageDelegate(QQmlComponent* imagecomponent);
        QQmlComponent* stickerDelegate() const;
        void setStickerDelegate(QQmlComponent* stickercomponent);
        QQmlComponent* animatedDelegate() const;
        void setAnimatedDelegate(QQmlComponent* animatedcomponent);
        QQmlComponent* locationDelegate() const;
        void setLocationDelegate(QQmlComponent* locationcomponent);
        QQmlComponent* webPageDelegate() const;
        void setWebPageDelegate(QQmlComponent* webpagecomponent);
        QQmlComponent* audioDelegate() const;
        void setAudioDelegate(QQmlComponent* audiocomponent);
        QQmlComponent* fileDelegate() const;
        void setFileDelegate(QQmlComponent* filecomponent);

    public slots:
        virtual void download();

    private slots:
        void updateContentWidth(qreal contentwidth);
        void updateContentWidth();
        void scaleToImageSize();
        void scaleToImage();
        void scaleToColumn();
        void scaleToFree();
        void initialize();

    private:
        qreal calcAspectRatio(const QSize &imagesize) const;
        bool canDownload() const;
        void createImageElement();
        void createStickerElement();
        void createAnimatedElement();
        void createLocationElement();
        void createWebPageElement();
        void createAudioElement();
        void createFileElement();

    protected:
        virtual void componentComplete();

    signals:
        void messageChanged();
        void isStickerChanged();
        void isAnimatedChanged();
        void isAudioChanged();
        void isVideoChanged();
        void isImageChanged();
        void isFileChanged();
        void isWebPageChanged();
        void sizeChanged();
        void contentWidthChanged();
        void durationChanged();
        void venueTitleChanged();
        void venueAddressChanged();
        void webPageTitleChanged();
        void webPageDescriptionChanged();
        void webPageUrlChanged();
        void webPageHasPhotoChanged();
        void webPageThumbnailHeightChanged();
        void videoThumbnailChanged();
        void geoPointChanged();
        void imageDelegateChanged();
        void stickerDelegateChanged();
        void animatedDelegateChanged();
        void locationDelegateChanged();
        void webPageDelegateChanged();
        void audioDelegateChanged();
        void fileDelegateChanged();

    private:
        Message* _message;
        qreal _size;
        qreal _contentwidth;

    private:
        QQmlComponent* _imagecomponent;
        QQmlComponent* _stickercomponent;
        QQmlComponent* _animatedcomponent;
        QQmlComponent* _locationcomponent;
        QQmlComponent* _webpagecomponent;
        QQmlComponent* _audiocomponent;
        QQmlComponent* _filecomponent;
};

#endif // QQUICKMEDIAMESSAGEITEM_H
