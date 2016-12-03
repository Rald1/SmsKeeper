#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QFile>
#include <QString>
#include <QList>

#include "database.h"
#include "message.h"
#include "addmessagedialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool isBaseOpen() const;

private:
    void  fillCompanionComboBox();
    void  clearCompanionComboBox();
    Database *_database;
    Ui::MainWindow *ui;
    AddMessageDialog * addMessageDialog;
    bool _baseOpen;

private slots:
    void showAddMessageDialog();
    void typeMessages(QString companion = "Все");
    void updateMessages();
    void updateCompanions();

public slots:
};

#endif // MAINWINDOW_H
