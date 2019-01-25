#ifndef RSSFEED_H
#define RSSFEED_H

#include <QObject>
#include <QDateTime>
#include <QUrl>
#include <QImage>

#include "RSSItem.h"

class RSSFeed : QObject
{
    Q_OBJECT

    Q_PROPERTY(QString title READ title WRITE setTitle)
    Q_PROPERTY(QString description READ description WRITE setDescription)
    Q_PROPERTY(QDateTime publishDate READ publishDate WRITE setPublishDate)
    Q_PROPERTY(QUrl link READ link WRITE setLink)
    Q_PROPERTY(QImage coverImage READ coverImage WRITE setCoverImage)
    Q_PROPERTY(std::vector<RSSItem*> items READ items)

    QString _title;
    QString _description;
    QDateTime _publishDate;
    QUrl _link;
    QImage _coverImage;
    std::vector<RSSItem*> _items;
public:    
    explicit RSSFeed(QObject* parent = nullptr);

    QString const& title() const { return _title; }
    QString const& description() const { return _description; }
    QDateTime const& publishDate() const { return _publishDate; }
    QUrl const& link() const { return _link; }
    QImage const& coverImage() const { return _coverImage; }
    std::vector<RSSItem*> const& items() const { return _items; }

    void setTitle(QString const& title) { _title = title; }
    void setDescription(QString const& description) { _description = description; }
    void setPublishDate(QDateTime const& publishDate) { _publishDate = publishDate; }
    void setLink(QUrl const& link) { _link = link; }
    void setCoverImage(QImage const& image) { _coverImage = image; }
};

#endif // RSSFEED_H
