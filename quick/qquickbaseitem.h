#ifndef QQUICKBASEITEM_H
#define QQUICKBASEITEM_H

#include <QtQuick>
#include <QHash>
#include "../config/cache/filecache.h"

class QQuickBaseItem : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(QString version READ version WRITE setVersion NOTIFY versionChanged)
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)
    Q_PROPERTY(QColor foregroundColor READ foregroundColor WRITE setForegroundColor NOTIFY foregroundColorChanged)
    Q_PROPERTY(qreal fontPixelSize READ fontPixelSize WRITE setFontPixelSize NOTIFY fontPixelSizeChanged)
    Q_PROPERTY(bool downloaded READ downloaded NOTIFY downloadedChanged)
    Q_PROPERTY(bool downloading READ downloading NOTIFY downloadingChanged)
    Q_PROPERTY(bool hasThumbnail READ hasThumbnail NOTIFY hasThumbnailChanged)

    public:
        explicit QQuickBaseItem(QQuickItem *parent = 0);
        QString version() const;
        QSize imageSize() const;
        QColor backgroundColor() const;
        QColor foregroundColor() const;
        qreal fontPixelSize() const;
        bool downloaded() const;
        bool downloading() const;
        bool hasThumbnail() const;
        void setVersion(const QString& version);
        void setBackgroundColor(const QColor& color);
        void setForegroundColor(const QColor& color);
        void setFontPixelSize(qreal pixelsize);

    public slots:
        void download();

    protected:
        void createComponent(const QString& componentcode);
        FileObject* createFileObject(TelegramObject* telegramobject);
        QString thumbnail() const;
        QString filePath() const;
        void updateSource(QVariant sourcevalue);

    protected slots:
        void bindToElement();

    signals:
        void backgroundColorChanged();
        void foregroundColorChanged();
        void fontPixelSizeChanged();
        void versionChanged();
        void imageSizeChanged();
        void downloadedChanged();
        void downloadingChanged();
        void hasThumbnailChanged();

    private:
        FileObject* _fileobject;
        QString _version;
        QColor _backcolor;
        QColor _forecolor;
        qreal _pixelsize;

    protected:
        QQuickItem* _mediaelement;

    private:
        static QHash<QByteArray, QQmlComponent*> _components;
};

#endif // QQUICKBASEITEM_H
