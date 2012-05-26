#include <QtGui>

#include "BT_QMainWindow.h"
#include "QBEditWidget.h"
#include "ui_btbaseutil_qt.h"
#include "btlibiface.h"

extern "C" {
#include "../../src/libbtbase.h"
}


//------------
//Constructor
//------------
BT_QMainWindow::BT_QMainWindow()
{
    ui = new Ui::MainWindow();
    ui->setupUi(this);
    
    btli = new BTLibIface();

    QObject::connect( ui->btn_read, SIGNAL(clicked()), this, SLOT(readClicked()) ); 
    QObject::connect( ui->btn_wrmm, SIGNAL(clicked()), 
                      this, SLOT(writeMMClicked())      ); 
    QObject::connect( ui->btn_wrtype, SIGNAL(clicked()), 
                      this, SLOT(writeTypeClicked())    ); 

    //Menu connections
    QObject::connect( ui->actionNew_base_list,    SIGNAL(triggered()), this, SLOT(newBaseFile()) );
    QObject::connect( ui->actionEdit_base_list,   SIGNAL(triggered()), this, SLOT(editBaseFile()) );
    QObject::connect( ui->actionImport_base_list, SIGNAL(triggered()), this, SLOT(importBaseFile()) );
    QObject::connect( ui->actionBTBaseUtil_help,  SIGNAL(triggered()), this, SLOT(help()) );
    QObject::connect( ui->actionAbout,            SIGNAL(triggered()), this, SLOT(about()) );

}

//----------------------------------------------------------------------------
//protected
//----------------------------------------------------------------------------

//closeEvent
void 
BT_QMainWindow::closeEvent(QCloseEvent *event)
{
    btbaselib_close();
}

//-----------------------------------------------------------------------------
//slots
//-----------------------------------------------------------------------------

void
BT_QMainWindow::readClicked() 
{
   btli->readBase();
} 

void
BT_QMainWindow::writeTypeClicked() 
{
    int n = ui->cb_wrtype->currentIndex();

    unsigned char mm;
    switch (n) {
        case 0: mm = 0x40; break;
        case 1: mm = 0x41; break;
        case 2: mm = 0x30; break;
        case 3: mm = 0x31; break;
        case 4: mm = 0x50; break;
        case 5: mm = 0x51; break;
        case 6: mm = 0x52; break;
        case 7: mm = 0x53; break;
        default:  
            QMessageBox m;
            m.setText("No base type selected.");
            m.exec();
            return;
        break;
    }

    btli->writeBase(mm);

}

void 
BT_QMainWindow::writeMMClicked()
{

    int major = ui->sb_wrmaj->value();
    int minor = ui->sb_wrmin->value();

    btli->writeBase((unsigned char)((major << 4) + (minor & 0x0f)));

}


void 
BT_QMainWindow::openEditWindow(QString fname)
{
    QBEditWidget *ew = new QBEditWidget;
    ew->setFname(fname);

    //ew.setCaption(tr("Editing base list"));
    ew->show();
    
}

void 
BT_QMainWindow::newBaseFile()
{
    QString fname = QFileDialog::getSaveFileName(this, tr("Create base list"),
                        "~", tr("Base list files (*.blist)"));

    if (fname != NULL) {
        openEditWindow(fname);
    }
}

void 
BT_QMainWindow::editBaseFile()
{
    QString fname = QFileDialog::getOpenFileName(this, tr("Open base list"),
                        "~", tr("Base list files (*.blist)"));

    if (fname == NULL) {
        return;
    }
  
    QFile file(fname);

    QStringList csv;

    if (file.open(QIODevice::ReadOnly)) {
        QString s = file.readAll();
        s.remove("\n");
        csv = s.split(',');
        file.close();
    }

    openEditWindow(fname);

}

void 
BT_QMainWindow::importBaseFile()
{
}

void 
BT_QMainWindow::help()
{
}

void 
BT_QMainWindow::about()
{
    QMessageBox m;
    m.setIcon(QMessageBox::Information);
    m.setText("BTBaseUtil - version 0.2\n\nCopyright (c) 2012 Jonathan MacMillan <dvorakviolinist@gmail.com>");
    m.exec();
}
