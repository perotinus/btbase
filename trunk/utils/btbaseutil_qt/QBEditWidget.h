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
    void spinBoxEdited(int i);
    

public:
    void setFname(QString s) {fname = s;}
    //QString getFname() {return fname;}

    void setData(QString s) {data = s;}
    //QString getData() {return data;}
    
    void fillTree();

private:
    Ui::QBEditWidget *ui; 

    QString fname;
    QString data;

    bool changed;
    int count;

};

#endif
