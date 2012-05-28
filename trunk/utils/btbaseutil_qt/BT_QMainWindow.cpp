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

    //Default base types
    QStringList baseNames;
    baseNames.append(QString("Ammo pack (4 bullets)"));
    baseNames.append(QString("Ammo pack (2 bullets)"));
    baseNames.append(QString("Medkit (flat)"));
    baseNames.append(QString("Medkit (rounded)"));
    baseNames.append(QString("T-Base 1 (red)"));
    baseNames.append(QString("T-Base 2 (blue)"));
    baseNames.append(QString("T-Base 3 (yellow)"));
    baseNames.append(QString("T-Base 4 (green)"));

    QMap<QString, unsigned char> baseBytes;
    baseBytes[baseNames.at(0)] = 0x40;
    baseBytes[baseNames.at(1)] = 0x41;
    baseBytes[baseNames.at(2)] = 0x30;
    baseBytes[baseNames.at(3)] = 0x31;
    baseBytes[baseNames.at(4)] = 0x50;
    baseBytes[baseNames.at(5)] = 0x51;
    baseBytes[baseNames.at(6)] = 0x52;
    baseBytes[baseNames.at(7)] = 0x53;

    ui->cb_wrtype->addItems(baseNames);

    //btli->setBaseNames(baseNames);
    btli->setBaseBytes(baseBytes);

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
    QString s = ui->cb_wrtype->currentText();

    QMap<QString, unsigned char>map = btli->getBaseBytes();

    unsigned char mm = map.value(s, 0xff);
/*
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
*/
    
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
BT_QMainWindow::openEditWindow(QString fname, QString data, bool populateTable)
{
    QBEditWidget *ew = new QBEditWidget;
    ew->setWindowModality(Qt::WindowModal);

    if (populateTable) {
        ew->setFname(fname);
        ew->setData(data);
        ew->fillTree();
    } else {
        ew->setFname(fname);
    }
    
    //ew.setCaption(tr("Editing base list"));
    ew->show();
    
}

void 
BT_QMainWindow::newBaseFile()
{
    QString fname = QFileDialog::getSaveFileName(this, tr("Create base list"),
                        "~", tr("Base list files (*.blist)"));

    if (fname != NULL) {
        openEditWindow(fname, QString(""), false);
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

    
    QString s;

    if (file.open(QIODevice::ReadOnly)) {
        s = file.readAll();
        s.remove("\n");
        file.close();
    }

    openEditWindow(fname, s, true);

}

void 
BT_QMainWindow::importBaseFile()
{

    //Get a filename from the dialog
    QString fname = QFileDialog::getOpenFileName(this, tr("Open base list"),
                        "~", tr("Base list files (*.blist)"));

    if (fname == NULL) {
        return;
    }
  
    //Get/open the file
    QFile file(fname);

    QString s;

    if (file.open(QIODevice::ReadOnly)) {
        s = file.readAll();
        s.remove("\n");
        file.close();
    }


    //Split to stringlist
    QStringList csv = s.split(";;");
    QMap<QString, unsigned char> map;

    int length = (csv.length()+1)/2;

    for (int i=0; i<length; i++) {

        QString bName = csv.takeFirst();

        bool ok;
        int bInt;

        //Account for malformed CSV files
        if (!csv.isEmpty()) {
            bInt = csv.takeFirst().toInt(&ok, 16);
        } else {
            QMessageBox m;
            m.setIcon(QMessageBox::Warning);
            m.setText("blist file missing last byte.  Setting to 0...");
            m.exec();

            bInt = 0;
        }
         
        //Account for bad numbers   
        if (bInt > 0xff || bInt < 0x00) {
            bInt = 0x00;

            QMessageBox m;
            m.setIcon(QMessageBox::Warning);
            m.setText("Byte value out of range!  Setting to 0...");
            m.exec();
        }

        map[bName] = (unsigned char)bInt;
        
    }

    //Remove all items from the combobox
    int cnt = ui->cb_wrtype->count();
    for (int i=0; i<cnt; i++) {
        ui->cb_wrtype->removeItem(0);
    }

    //Add the new items
    ui->cb_wrtype->addItems(map.keys());

    //Set the correct map in the BTLibIface
    btli->setBaseBytes(map);

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
