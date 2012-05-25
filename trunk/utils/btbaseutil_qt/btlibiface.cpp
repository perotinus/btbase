#include <QApplication>
#include <QMessageBox>
#include <QString>

extern "C" {
#include "../../src/libbtbase.h"
}

#include "btlibiface.h"

BTLibIface::BTLibIface() 
{
    int status = btbaselib_setup();
    
    if (status != BSETUP_OK) {
        QMessageBox m;
        m.setText("NFC device not found.");
        m.exec();
        exit(-1);
    }

}


void 
BTLibIface::readBase() 
{
    unsigned char btype = btbaselib_readbase();
    
    if (btype == BSETUP_NOBASE) {
        QMessageBox m;
        m.setText("BattleTag base not found.");
        m.exec();
        return;
    }

    QString s = QString("Found base: ");
    switch (btype) {
        case 0x30: s.append("MedKit 1"); break;
        case 0x31: s.append("MedKit 2"); break;
        case 0x40: s.append("Ammo pack (4 bullets)"); break;
        case 0x41: s.append("Ammo pack (2 bullets)"); break;
        case 0x50: s.append("T-Base 1 (red)"); break;
        case 0x51: s.append("T-Base 2 (blue)"); break;
        case 0x52: s.append("T-Base 3 (yellow)"); break;
        case 0x53: s.append("T-Base 4 (green)"); break;
        default: s.append(QString::number(btype, 16)); break;
    }

    QMessageBox m;
    m.setText(s);
    m.exec();
    return;
}

void 
BTLibIface::writeBase(unsigned char v) 
{

    int retval = btbaselib_writebase(v);
    
    if (retval == BSETUP_NOBASE) {

        QMessageBox m;
        m.setText("BattleTag base not found.");
        m.exec();
        return;

    } else {

        QMessageBox m;
        
        QString s("BattleTag base successfully written: ");
        switch (v) {
            case 0x30: s.append("MedKit 1"); break;
            case 0x31: s.append("MedKit 2"); break;
            case 0x40: s.append("Ammo pack (4 bullets)"); break;
            case 0x41: s.append("Ammo pack (2 bullets)"); break;
            case 0x50: s.append("T-Base 1 (red)"); break;
            case 0x51: s.append("T-Base 2 (blue)"); break;
            case 0x52: s.append("T-Base 3 (yellow)"); break;
            case 0x53: s.append("T-Base 4 (green)"); break;
            default: s.append(QString::number(btype, 16)); break;
        }

        m.setText(s);
        m.exec();
        return;

    }

}
