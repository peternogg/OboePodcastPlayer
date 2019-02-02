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

    QString _title, _description, _guid;
    QDateTime _publishDate;
    QUrl _link, _enclosureLink;
    int64_t _enclosureSize;
public:
    explicit RSSItem(QObject *parent = nullptr);
    virtual ~RSSItem() {}

    QString const& title() { return _title; }
    QString const& description() { return _description; }
    QDateTime const& publishDate() { return _publishDate; }
    QString const& guid() { return _guid; }
    QUrl const& link() { return _link; }
    QUrl const& enclosureLink() { return _enclosureLink; }
    int64_t enclosureSize() { return _enclosureSize; }

    void setTitle(QString const& title) { _title = title; }
    void setDescription(QString const& description) { _description = description; }
    void setPublishDate(QDateTime const& pubDate) { _publishDate = pubDate; }
    void setGuid(QString const& guid) { _guid = guid; }
    void setLink(QUrl const& url) { _link = url; }
    void setEnclosureLink(QUrl const& url) { _enclosureLink = url; }
    void setEnclosureSize(int64_t size) { _enclosureSize = size; }
signals:

public slots:
};

#endif // RSSITEM_H
