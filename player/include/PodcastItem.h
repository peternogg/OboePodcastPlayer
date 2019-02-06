#ifndef PODCASTITEM_H
#define PODCASTITEM_H

#include <QDateTime>
#include <QObject>
#include <QUrl>

#include "feedpp.h"

class PodcastItem : public QObject
{
    Q_GADGET
public:
    PodcastItem(feedpp::item const& item, QObject *parent = nullptr);
    virtual ~PodcastItem();

    Q_PROPERTY(QString title READ title)
    Q_PROPERTY(QString description READ description)
    Q_PROPERTY(QDateTime pubDate READ pubDate)
    Q_PROPERTY(QString guid READ guid)
    Q_PROPERTY(QUrl enclosureUrl READ enclosureUrl)
    Q_PROPERTY(QString enclosureType READ enclosureType)


    QString title() const;
    QString description() const;
    QDateTime pubDate() const;
    QString guid() const;
    QUrl enclosureUrl() const;
    QString enclosureType() const;

private:
    QString _title;
    QString _description;
    QDateTime _pubDate;
    QString _guid;
    QUrl _enclosureUrl;
    QString _enclosureType;

};

#endif // PODCASTITEM_H
