#include "database.h"
#include <QFile>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include <QTextStream>
#include <QIcon>

Database::Database(QString databasePath): _databasePath(databasePath)
{}

Database::~Database()
{
    for (auto i=_messageList.begin(); i<_messageList.end(); i++)
        delete *i;
}

bool Database::open()
{
    QFile file(_databasePath);
    if(file.open(QIODevice::ReadOnly))
    {
        QJsonParseError parseError;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(file.readAll(), &parseError);
        if(parseError.error == QJsonParseError::NoError)
        {
             QJsonObject jsonObject = jsonDoc.object();
             parseOptions(jsonObject);
             parseCompanions(jsonObject);
             parseMessages(jsonObject);
             file.close();
             return true;
        }
        else
        {
            QMessageBox messageBox(QMessageBox::Critical,
                               tr("Ошибка"),
                               tr("Возникли ошибки при чтении базы. Программа будет закрыта. Parse error: ") + parseError.errorString());
            messageBox.exec();
            return false;
        }
    }
    else
    {
        QMessageBox messageBox(QMessageBox::Question,
                           tr("Ошибка"),
                           tr("Отсутствует файл базы данных. Ошибка: ") + file.errorString() + ": " + _databasePath + tr("\nСоздать новую базу?"), QMessageBox::Ok|QMessageBox::Cancel);
        messageBox.exec();

        if(messageBox.result() == QMessageBox::Ok)
        {
            return createNewBase();
        }
        return false;
    }
}

bool Database::open(QString databasePath)
{
    _databasePath = databasePath;
    return open();
}

bool Database::createNewBase()
{
    _nextMessageId = 0;

    return saveDatabase();
}

void Database::parseOptions(QJsonObject& jsonObject)
{
    _nextMessageId = jsonObject["nextMessageId"].toInt();
}

void Database::parseCompanions(QJsonObject& jsonObject)
{
    QJsonArray companions = jsonObject["companions"].toArray();

    for(auto iter = companions.begin(); iter!=companions.end(); iter++)
        _companionsList.append((*iter).toString());
}

void Database::parseMessages(QJsonObject& jsonObject)
{
    QJsonArray messages = jsonObject["messages"].toArray();
    for (auto iter = messages.begin(); iter!=messages.end(); iter++)
    {
        auto message = new Message;
        QJsonObject object = (*iter).toObject();
        message->setId(object["id"].toInt());
        message->setFrom(object["from"].toString());
        message->setTo(object["to"].toString());
        message->setText(object["text"].toString());

        message->setDateTime(QDateTime::fromString(object["datetime"].toString(), "ddMMyyyyHHmm"));

        _messageList.append(message);
    }
}

const QList<Message*>& Database::messageList() const
{
    return _messageList;
}

const QList<QString>& Database::companionsList() const
{
    return _companionsList;
}

void Database::addMessage(Message *message)
{
    message->setId(_nextMessageId++);
    _messageList.append(message);
    sortMessages();
    saveDatabase();
    emit messagesChanged();
}

bool Database::addCompanion(QString companion)
{
    if (companionExist(companion))
        return false;

    _companionsList.append(companion);
    qSort(_companionsList.begin(), _companionsList.end());
    saveDatabase();
    emit companionsChanged();
    return true;
}

bool Database::companionExist(const QString & companion) const
{
    for (auto i = _companionsList.begin(); i!=_companionsList.end(); i++)
    {
        if (!QString::compare(companion, *i, Qt::CaseInsensitive))
            return true;
    }
    return false;
}

bool Database::saveDatabase() const
{
    QFile file(_databasePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QJsonObject mainObject;

        optionsToJsonObject(mainObject);
        companionsToJsonObject(mainObject);
        messagesToJsonObject(mainObject);

        QJsonDocument document(mainObject);

        QTextStream out(&file);
        out.setCodec("UTF-8");
        out << document.toJson();

        file.close();
        return true;
    }
    else
    {
        QMessageBox messageBox(QMessageBox::Critical,
                           tr("Ошибка"),
                           tr("Database::saveDatabase(): не удается открыть базу данных ") + _databasePath);
        messageBox.exec();
        return false;
    }
}

void Database::optionsToJsonObject(QJsonObject& jsonObject) const
{
    jsonObject.insert("nextMessageId", _nextMessageId);
}

void Database::companionsToJsonObject(QJsonObject& jsonObject) const
{
    QJsonArray companionsArray;
    for (auto i = _companionsList.begin(); i!=_companionsList.end(); i++)
        companionsArray.append(*i);

    jsonObject.insert("companions", companionsArray);
}

void Database::messagesToJsonObject(QJsonObject& jsonObject) const
{
    QJsonArray messagesArray;
    for (auto i = _messageList.begin(); i!=_messageList.end(); i++)
    {
        QJsonObject messageObject;
        messageObject.insert("id", (*i)->id());
        messageObject.insert("datetime", (*i)->dateTime().toString("ddMMyyyyHHmm"));
        messageObject.insert("from", (*i)->from());
        messageObject.insert("to", (*i)->to());
        messageObject.insert("text", (*i)->text());

        messagesArray.append(messageObject);
    }

    jsonObject.insert("messages", messagesArray);
}

void Database::sortMessages()
{
    qSort(_messageList.begin(), _messageList.end(), [](Message* i, Message* j)->bool
                                            {
                                                if (i->dateTime().date() < j->dateTime().date())
                                                    return true;
                                                else if (i->dateTime().date() == j->dateTime().date())
                                                         if (i->dateTime().time() < j->dateTime().time())
                                                             return true;

                                                return false;
                                            });
}
