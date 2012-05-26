#ifndef QBEDITWIDGET_H
#define QBEDITWIDGET_H

#include <QWidget>
#include <QCloseEvent>
#include <QtGui>

#include "ui_qbeditwidget.h"

class QBEditWidget: public QWidget
{

Q_OBJECT

public:
    QBEditWidget();

protected:
    void closeEvent(QCloseEvent *event);

public slots:

    void addItem();
    void editItem(QTreeWidgetItem *item);
    void delItem();
    void save();
    void closeBtn();
    

public:
    void setFname(QString s) {fname = s;}
    QString getFname() {return fname;}

private:
    Ui::QBEditWidget *ui; 
    QString fname;
    bool changed;
    int count;

};

#endif
