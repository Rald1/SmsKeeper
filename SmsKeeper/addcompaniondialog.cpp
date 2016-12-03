#include "addcompaniondialog.h"
#include "ui_addcompaniondialog.h"
#include <QMessageBox>

AddCompanionDialog::AddCompanionDialog(Database &database, QWidget *parent) : _database(database),
    QDialog(parent),
    ui(new Ui::AddCompanionDialog)
{
    ui->setupUi(this);
    ui->addButton->setEnabled(false);

    connect(ui->cancelButton, SIGNAL(clicked(bool)), this, SLOT(reject()));
    connect(ui->addButton, SIGNAL(clicked(bool)), this, SLOT(accept()));
    connect(ui->lineEdit, SIGNAL(textChanged(QString)), this, SLOT(enableAddButton(QString)));
}

AddCompanionDialog::~AddCompanionDialog()
{
    delete ui;
}

void AddCompanionDialog::accept()
{
    if (!_database.addCompanion(ui->lineEdit->text()))
    {
         QMessageBox messageBox(QMessageBox::Information,
                               tr("Уведомление"),
                               tr("Собеседник уже существует"));
         messageBox.exec();
    }
    QDialog::accept();
}

void AddCompanionDialog::enableAddButton(const QString &str)
{
    ui->addButton->setEnabled(str.length()>0);
}
