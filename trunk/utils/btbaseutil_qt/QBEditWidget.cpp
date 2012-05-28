#include <QtGui>

#include "QBEditWidget.h"
#include "ui_qbeditwidget.h"
#include "btlibiface.h"
#include "QHexSpinBox.h"

extern "C" {
#include "../../src/libbtbase.h"
}


//------------
//Constructor
//------------
QBEditWidget::QBEditWidget()
{
    ui = new Ui::QBEditWidget();
    ui->setupUi(this);
    changed = false;
    count = 0;

    ui->tre_bases->setColumnWidth(0,250);

    QObject::connect( ui->btn_add, SIGNAL(clicked()), this, SLOT(addItem()) );
    QObject::connect( ui->btn_del, SIGNAL(clicked()), this, SLOT(delItem()) );
    QObject::connect( ui->tre_bases, SIGNAL(itemClicked(QTreeWidgetItem *)), this, SLOT(editItem(QTreeWidgetItem *)) );
    QObject::connect( ui->btn_save, SIGNAL(clicked()), this, SLOT(save()) );
    QObject::connect( ui->btn_close, SIGNAL(clicked()), this, SLOT(closeBtn()) );

}

//----------------------------------------------------------------------------
//protected
//----------------------------------------------------------------------------

//closeEvent
void 
QBEditWidget::closeEvent(QCloseEvent *event)
{
    //Bring up save panel if there were changes.
}

void
QBEditWidget::fillTree()
{


    QStringList csv;
    csv = data.split(";;");

    QTreeWidget *t = ui->tre_bases;
    int length = (csv.length()+1)/2;

    for (int i=0; i<length; i++) {
        
        QTreeWidgetItem *w = new QTreeWidgetItem(t);
        QString s = csv.takeFirst();
        //w->setText(0, csv.takeFirst());
        w->setText(0, s);
        w->setFlags(w->flags() | Qt::ItemIsEditable);

        QHexSpinBox *hsb = new QHexSpinBox();
        hsb->setRange(0,255);

        QObject::connect(hsb, SIGNAL(valueChanged(int)), this,
            SLOT(spinBoxEdited(int)));

        if (!csv.isEmpty()) {
            bool ok;
            hsb->setValue(csv.takeFirst().toInt(&ok, 16));
        } else {
            hsb->setValue(0);
        }
 
        t->addTopLevelItem(w);

        count++; 
        t->setItemWidget(w, 1, hsb);
    }

    changed = false;
}

void
QBEditWidget::addItem()
{
    changed = true;
    QTreeWidgetItem *w = new QTreeWidgetItem(ui->tre_bases);
    w->setText(0, tr("Base"));
    w->setFlags(w->flags() | Qt::ItemIsEditable);

    QHexSpinBox *hsb = new QHexSpinBox();
    hsb->setRange(0,255);
    
    QObject::connect(hsb, SIGNAL(valueChanged(int)), this,
        SLOT(spinBoxEdited(int)));
    ui->tre_bases->addTopLevelItem(w);

    count++; 

    ui->tre_bases->setItemWidget(w, 1, hsb);

}

void
QBEditWidget::editItem(QTreeWidgetItem *item)
{
    changed = true;
    ui->tre_bases->editItem(item); 
}

void
QBEditWidget::delItem()
{
    changed = true;
    count--;

    QTreeWidget *t = ui->tre_bases;

    QTreeWidgetItem *w = t->currentItem();
    int idx = t->indexOfTopLevelItem(w);

    t->takeTopLevelItem(idx);

}

void
QBEditWidget::save()
{
    changed = false;

    QTreeWidget *t = ui->tre_bases;
    //Create the 

    QString line;

    for (int i=0; i<count; i++) {
        QTreeWidgetItem *w = t->topLevelItem(i);
        QString name = w->text(0);
        QHexSpinBox *hsb_v = (QHexSpinBox*)(t->itemWidget(w, 1));
        QString v = QString::number(hsb_v->value(), 16).toUpper();
        //t->takeTopLevelItem(0);

        line.append(name); 
        line.append(";;");
        line.append(v);
        //Don't append a separator to the last item
        if (i < count-1)
            line.append(";;");

        QMessageBox m;
        m.setText(line);
        m.exec();
    }

    QFile f(fname);
    if (!f.open(QIODevice::WriteOnly)) {

        QMessageBox m;
        QString s("Could not save file:\n");
        s.append(fname); 
        m.setText(s);
        m.setIcon(QMessageBox::Warning);
        m.exec();

        return;
    }

    QTextStream ostream(&f);

    ostream << line;

    f.close();

}

void
QBEditWidget::closeBtn()
{
    if (changed) {
        //Save confirm
        QMessageBox msgBox;
        msgBox.setText("The base file has been modified.");
        msgBox.setInformativeText(
            "Do you want to save your changes?");
        msgBox.setStandardButtons(QMessageBox::Save | 
            QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);

        int ret = msgBox.exec();

        switch (ret) {
            case QMessageBox::Save: save(); 
            case QMessageBox::Discard: close(); break;
            case QMessageBox::Cancel: break;
            default: close(); break;
        }

    } else {
        close();
    }


}

void 
QBEditWidget::spinBoxEdited(int i)
{
    changed = true;
}
