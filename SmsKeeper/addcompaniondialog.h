#ifndef ADDCOMPANIONDIALOG_H
#define ADDCOMPANIONDIALOG_H

#include <QDialog>
#include "database.h"

namespace Ui {
class AddCompanionDialog;
}

class AddCompanionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddCompanionDialog(Database &database, QWidget *parent = 0);
    ~AddCompanionDialog();

public slots:
    void accept();
    void enableAddButton(const QString &);

private:
    Ui::AddCompanionDialog *ui;
    Database &_database;
};

#endif // ADDCOMPANIONDIALOG_H
