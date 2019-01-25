#ifndef RSSITEM_H
#define RSSITEM_H

#include <QObject>
#include <QDateTime>
#include <QUrl>

class RSSItem : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString title READ title WRITE setTitle)
    Q_PROPERTY(QString description READ description WRITE setDescription)
    Q_PROPERTY(QDateTime publishDate READ publishDate WRITE setPublishDate)
    Q_PROPERTY(QString guid READ guid WRITE setGuid)
    Q_PROPERTY(QUrl link READ link WRITE setLink)
    Q_PROPERTY(QUrl enclosureLink READ enclosureLink WRITE setEnclosureLink)
    Q_PROPERTY(int64_t enclosureSize READ enclosureSize WRITE setEnclosureSize)
public:
    explicit RSSItem(QObject *parent = nullptr);

signals:

public slots:
};

#endif // RSSITEM_H
