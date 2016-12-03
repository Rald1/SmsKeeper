#ifndef ADDMESSAGEDIALOG_H
#define ADDMESSAGEDIALOG_H

#include <QDialog>
#include "message.h"
#include "database.h"

namespace Ui {
class AddMessageDialog;
}

class AddCompanionDialog;

class AddMessageDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddMessageDialog(Database &dataBase, QWidget *parent = 0);
    ~AddMessageDialog();

private:
    void fillCompanionComboBox();
    void clearCompanionComboBox();
    bool validation() const;
    void addMessage();

    AddCompanionDialog *_addCompanionDialog;
    Database &_database;
    Ui::AddMessageDialog *ui;

public slots:
    void addButtonClicked();
    void showAddCompanionDialog();
    void updateCompanions();
    void clear();
};

#endif // ADDMESSAGEDIALOG_H
