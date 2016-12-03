#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QList>
#include "message.h"

class QJsonObject;

class Database : public QObject
{
     Q_OBJECT

public:
    Database(QString databasePath);
    ~Database();
    bool open(QString databasePath);
    const QList<Message*>& messageList() const;
    const QList<QString>& companionsList() const;
    void addMessage(Message*);
    bool addCompanion(QString);
    bool open();
    void sortMessages();
    bool createNewBase();

private:    
    bool saveDatabase() const;
    void parseOptions(QJsonObject& jsonObject);
    void parseCompanions(QJsonObject& jsonObject);
    void parseMessages(QJsonObject& jsonObject);
    void optionsToJsonObject(QJsonObject &) const;
    void companionsToJsonObject(QJsonObject &) const;
    void messagesToJsonObject(QJsonObject &) const;
    bool companionExist(const QString & companion) const;

    QList<Message*> _messageList;
    QList<QString> _companionsList;
    QString _databasePath;
    int _nextMessageId;

signals:
    void messagesChanged() const;
    void companionsChanged() const;
};

#endif // DATABASE_H
