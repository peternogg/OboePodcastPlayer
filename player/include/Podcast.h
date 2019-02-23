#ifndef PODCAST_H
#define PODCAST_H

#include <QObject>
#include <QDateTime>

#include <algorithm>

#include "feedpp.h"
#include "Persistable.h"
#include "PodcastItem.h"

class Podcast : public Persistable
{
    Q_OBJECT
public:
    Podcast(QObject* parent = nullptr);
    Podcast(feedpp::feed const& feed, QObject* parent = nullptr);
    virtual ~Podcast() override;

    Q_PROPERTY(QString title READ title WRITE setTitle)
    Q_PROPERTY(QUrl link READ link WRITE setLink)
    Q_PROPERTY(QString description READ description WRITE setDescription)
    Q_PROPERTY(QDateTime lastUpdate READ lastUpdate WRITE setLastUpdate)
    //Q_PROPERTY(std::vector<PodcastItem*> items READ items)
    Q_PROPERTY(QUrl updateLink READ updateLink WRITE setUpdateLink)

    QString title() const;
    QUrl link() const;
    QString description() const;
    QDateTime lastUpdate() const;
    std::vector<PodcastItem*> const& items() const;
    QUrl updateLink() const;

    void setTitle(const QString &title);
    void setLink(const QUrl &link);
    void setDescription(const QString &description);
    void setLastUpdate(const QDateTime &lastUpdate);
    void setItems(std::vector<PodcastItem*> const& items);
    void setUpdateLink(const QUrl& updateLink);

    bool isDifferentFrom(Podcast const& other) const;

    virtual QString table() const override { return "Podcast"; }

private:
    QString _title;
    QUrl _link;
    QString _description;
    QDateTime _lastUpdate;
    QUrl _updateLink;

    std::vector<PodcastItem*> _items;
};

#endif // PODCAST_H
