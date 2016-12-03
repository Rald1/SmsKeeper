#include "addmessagedialog.h"
#include "ui_addmessagedialog.h"
#include "addcompaniondialog.h"
#include <QMessageBox>

AddMessageDialog::AddMessageDialog(Database &database, QWidget *parent) : QDialog(parent),
    _database(database),
    ui(new Ui::AddMessageDialog)

{
    ui->setupUi(this);

    fillCompanionComboBox();

    connect(ui->calendarWidget, SIGNAL(clicked(QDate)), ui->dateEdit, SLOT(setDate(QDate)));
    connect(ui->addNameButton, SIGNAL(clicked(bool)), this, SLOT(showAddCompanionDialog()));
    connect(&_database, SIGNAL(companionsChanged()), this, SLOT(updateCompanions()));
    connect(ui->addButton, SIGNAL(clicked(bool)), this, SLOT(addButtonClicked()));
    connect(ui->cancelButton, SIGNAL(clicked(bool)), this, SLOT(reject()));
    connect(this, SIGNAL(finished(int)), this, SLOT(clear()));
    ui->timeEdit->setMinimumSize(ui->dateEdit->sizeHint());
}


AddMessageDialog::~AddMessageDialog()
{
    delete ui;
}

void AddMessageDialog::addMessage()
{
    QString from, to;

    if(ui->fromMeRadioButton->isChecked())
    {
        from = tr("Меня");
        to = ui->companionComboBox->currentText();
    }
    else
    {
        from = ui->companionComboBox->currentText();
        to = tr("Мне");
    }

    Message *message = new Message(from,
                                   to,
                                   ui->textEdit->toPlainText(),
                                   QDateTime(ui->dateEdit->date(),
                                             ui->timeEdit->time()) );
    _database.addMessage(message);
}

void AddMessageDialog::fillCompanionComboBox()
{
    const QList<QString>& companionsList = _database.companionsList();
    for(auto i = companionsList.begin(); i<companionsList.end(); i++)
    {
        ui->companionComboBox->addItem(*i);
    }
}

void AddMessageDialog::showAddCompanionDialog()
{
    _addCompanionDialog = new AddCompanionDialog(_database, this);
    _addCompanionDialog->show();
}

void AddMessageDialog::updateCompanions()
{
    clearCompanionComboBox();
    fillCompanionComboBox();
}

void AddMessageDialog::clearCompanionComboBox()
{
    while(ui->companionComboBox->count())
    {
        ui->companionComboBox->removeItem(0);
    }
}

bool AddMessageDialog::validation() const
{
    if (ui->companionComboBox->currentText().isEmpty())
    {
        QMessageBox messageBox(QMessageBox::Information,
                           tr("Уведомление"),
                           tr("Добавьте собеседника"));
        messageBox.exec();

        return false;
    }

    QDate startDate(2000, 1, 1);
    if (startDate == ui->dateEdit->date())
    {
        QMessageBox messageBox(QMessageBox::Information,
                           tr("Уведомление"),
                           tr("Выберите дату"));
        messageBox.exec();

        return false;
    }

    QTime startTime(0,0);
    if (startTime == ui->timeEdit->time())
    {
        QMessageBox messageBox(QMessageBox::Information,
                           tr("Уведомление"),
                           tr("Выберите время"));
        messageBox.exec();

        return false;
    }

    if (ui->textEdit->toPlainText().isEmpty())
    {
        QMessageBox messageBox(QMessageBox::Information,
                           tr("Уведомление"),
                           tr("Введите текст сообщения"));
        messageBox.exec();
        return false;
    }

    return true;
}

void AddMessageDialog::addButtonClicked()
{
    if (!validation())
        return;
    addMessage();
    this->accept();
}

void AddMessageDialog::clear()
{
    ui->textEdit->clear();

    if(ui->clearWhenFinishCheckBox->checkState() == Qt::Checked)
    {
        ui->companionComboBox->setCurrentIndex(0);
        ui->toMeRadioButton->setChecked(true);
        ui->dateEdit->setDate(QDate(2000,1,1));
        ui->calendarWidget->setSelectedDate(QDate::currentDate());
        ui->timeEdit->setTime(QTime(0,0));
    }
}
