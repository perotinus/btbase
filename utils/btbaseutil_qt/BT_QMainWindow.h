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

public slots:
    void readClicked();
    void writeTypeClicked();
    void writeMMClicked();

private:
    BTLibIface *btli;
    Ui::MainWindow *ui;
};

#endif
