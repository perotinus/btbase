#include <QtGui>

#include "BT_QMainWindow.h"
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

    btli->writeBase((unsigned char)((minor << 4) + (major & 0x0f)));

}

