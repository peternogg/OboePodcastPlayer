#ifndef UTIL_H
#define UTIL_H

#include <QString>

inline QString msToTimestamp(qint64 msecs) {
    qint64 hours = msecs / (60 * 60 * 1000);
    msecs %= 60 * 60 * 1000;

    qint64 minutes = msecs / (60 * 1000);
    msecs %= 60 * 1000;

    qint64 seconds = msecs / 1000;

    return QString("%1:%2:%3")
            .arg(hours, 2, 10, QChar('0'))
            .arg(minutes, 2, 10, QChar('0'))
            .arg(seconds, 2, 10, QChar('0'));
}

#endif // UTIL_H
