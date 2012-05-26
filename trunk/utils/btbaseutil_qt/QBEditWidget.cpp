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

    //MODIFY THIS-SAMPLE
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
QBEditWidget::addItem()
{
    changed = true;
    QTreeWidgetItem *w = new QTreeWidgetItem(ui->tre_bases);
    w->setText(0, tr("Base"));
    w->setFlags(w->flags() | Qt::ItemIsEditable);

    QHexSpinBox *hsb = new QHexSpinBox();
    hsb->setRange(0,255);
    
    ui->tre_bases->addTopLevelItem(w);

    count++; 

    ui->tre_bases->setItemWidget(w, 1, hsb);

}

void
QBEditWidget::editItem(QTreeWidgetItem *item)
{
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
}

void
QBEditWidget::closeBtn()
{
    close();
}

