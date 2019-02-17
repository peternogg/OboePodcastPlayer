create table Podcast (
    id              integer primary key,
    title           text,
    link            text not null,
    description     text,
    lastUpdate      date not null
);

create table PodcastItem (
    id              integer primary key,
    guid            text not null,
    -- Which podcast does this episode belong to?
    parentPodcast   integer not null references Subscriptions(id),
    
    title           text,
    description     text,
    pubDate         datetime, -- Published on
    enclosureUrl    text, -- A link to an audio file
    enclosureType   text, -- MIME type of enclosure
    downloadPath    text, -- The path for the downloaded file
    lastTimestamp   integer,
    listenedState   text,
    downloadState   text
);
