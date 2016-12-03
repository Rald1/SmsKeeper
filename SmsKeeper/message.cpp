#include "message.h"

Message::Message()
{}

Message::Message(QString from, QString to, QString text, QDateTime dateTime) : _from(from), _to(to), _text(text), _dateTime(dateTime)
{}

int Message::id() const
{
    return _id;
}

QString Message::from() const
{
    return _from;
}

QString Message::to() const
{
    return _to;
}

QString Message::text() const
{
    return _text;
}

void Message::setId(int id)
{
    _id = id;
}

void Message::setDateTime(QDateTime dateTime)
{
    _dateTime = dateTime;
}

const QDateTime& Message::dateTime() const
{
    return _dateTime;
}

void Message::setFrom(QString from)
{
    _from = from;
}

void Message::setTo(QString to)
{
    _to = to;
}

void Message::setText(QString text)
{
    _text = text;
}
