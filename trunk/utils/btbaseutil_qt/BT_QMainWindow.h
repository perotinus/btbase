#ifndef BT_QMAINWINDOW_H
#define BT_QMAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QtGui>

#include "ui_btbaseutil_qt.h"

#include "btlibiface.h"

class BT_QMainWindow: public QMainWindow
{

Q_OBJECT

public:
    BT_QMainWindow();

protected:
    void closeEvent(QCloseEvent *event);
    void openEditWindow(QString fname, QString data, bool populateTable);

public slots:
    void readClicked();
    void writeTypeClicked();
    void writeMMClicked();

    //Menu slots
    void newBaseFile();
    void editBaseFile();
    void importBaseFile();
    void help();
    void about();

private:
    BTLibIface *btli;
    Ui::MainWindow *ui;


};

#endif
