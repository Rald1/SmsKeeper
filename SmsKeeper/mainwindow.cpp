#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextStream>
#include <QIcon>
#include <QtGui>
#include <QTextCodec>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _baseOpen = false;
    _database = new Database("base.txt");
    _baseOpen = _database->open();
    if(!_baseOpen)
        return;

    addMessageDialog = 0;

    fillCompanionComboBox();

    typeMessages();

    connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(showAddMessageDialog()));
    connect(_database, SIGNAL(messagesChanged()), this, SLOT(updateMessages()));
    connect(_database, SIGNAL(companionsChanged()), this, SLOT(updateCompanions()));
    connect(ui->companionComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(typeMessages(QString)));

    setWindowIcon(QIcon(":/icons/sms.png"));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete _database;
}


void MainWindow::typeMessages(QString companion)
{
    QString text;
    QTextStream textStream(&text);

    QString strFrom = tr("От: ");
    QString strTo = tr("Кому: ");
    QString strAll = tr("Все");

    const QList<Message*> & messageList = _database->messageList();

    textStream << "<table align=\"center\">";
    QString row("<tr><td>%1</td><td>%2</td></tr>");

    for (auto i = messageList.begin(); i!=messageList.end(); i++)
    {
        if (companion != strAll)
            if ((*i)->from() != companion && (*i)->to() != companion)
                continue;

        QString message;
        QTextStream messageStream(&message);
        messageStream.setCodec("UTF-8");
        if (companion == strAll)
        {
            if ((*i)->from() == tr("Меня"))
                messageStream << strTo << (*i)->to() << "<br>";
            else
                messageStream << strFrom << (*i)->from() << "<br>";
        }

        messageStream << (*i)->text() << "<br>";
        messageStream << (*i)->dateTime().toString("dd MMM yy, HH:mm") << "<br>";
        if ((*i)->from() == tr("Меня"))
            textStream << row.arg("").arg(message);
        else
            textStream << row.arg(message).arg("");
    }
    textStream << "</table>";

    ui->textEdit->setText(text);
}

void MainWindow::showAddMessageDialog()
{
   if (!addMessageDialog)
        addMessageDialog = new AddMessageDialog(*_database,this);

    addMessageDialog->show();
}

void MainWindow::updateMessages()
{
    typeMessages(ui->companionComboBox->currentText());
}

void MainWindow::updateCompanions()
{
    clearCompanionComboBox();
    fillCompanionComboBox();
}

void MainWindow::fillCompanionComboBox()
{
    const QList<QString>& companionList = _database->companionsList();
    ui->companionComboBox->addItem(tr("Все"));
    for(auto i = companionList.begin(); i<companionList.end(); i++)
    {
        ui->companionComboBox->addItem(*i);
    }
}

void  MainWindow::clearCompanionComboBox()
{
    while(ui->companionComboBox->count())
    {
        ui->companionComboBox->removeItem(0);
    }
}

bool MainWindow::isBaseOpen() const
{
    return _baseOpen;
}
