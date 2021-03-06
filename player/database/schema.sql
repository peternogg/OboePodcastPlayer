create table Podcast (
    id              integer primary key,
    title           text,
    link            text not null,
    description     text,
    lastUpdate      date not null,
    updateLink      text
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
    lastTimestamp   integer
);

create table Settings (
    id              integer primary key,
    name            text unique not null,
    value           text
);

insert into Settings (name, value) values
    ('subscriptionUpdatePeriod', '60'),   -- 60 minutes
    ('jumpForwardAmount', '15000'),       -- 15 seconds (in milliseconds)
    ('jumpBackwardAmount', '15000'),      -- 15 seconds (in milliseconds)
    ('automaticDownload', 'true');
