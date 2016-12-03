#ifndef MESSAGE_H
#define MESSAGE_H
#include <QString>
#include <sstream>
#include <QDateTime>

class Message
{
    int _id;
    QString _from;
    QString _to;
    QString _text;
    QDateTime _dateTime;
public:
    Message();
    Message(QString from, QString to, QString text, QDateTime dateTime);
    int id() const;
    const QDateTime& dateTime() const;
    QString from() const;
    QString to() const;
    QString text() const;

    void setId(int);
    void setDateTime(QDateTime);
    void setFrom(QString);
    void setTo(QString);
    void setText(QString);
};

#endif // MESSAGE_H
