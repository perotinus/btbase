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
        m.setIcon(QMessageBox::Critical);
        m.setText("NFC device not found.");
        m.exec();
        exit(-1);
    }

}


void 
BTLibIface::readBase() 
{
    int btype_int = btbaselib_readbase();
    
    if (btype_int == BSETUP_NOBASE) {
        QMessageBox m;
        m.setIcon(QMessageBox::Warning);
        m.setText("BattleTag base not found.");
        m.exec();
        return;
    }

    //Lossy cast OK - if the function succeeded,
    //there will only be values between 1 and 256
    unsigned char btype = (unsigned char)btype_int;

    QString s = QString("Found base: ");

    QString bName = baseBytes.key(btype, QString::number(btype,16).toUpper());

    s.append(bName);

    QMessageBox m;
    m.setIcon(QMessageBox::Information);
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
        m.setIcon(QMessageBox::Warning);
        m.setText("BattleTag base not found.");
        m.exec();
        return;

    } else {

        QMessageBox m;
        
        m.setIcon(QMessageBox::Information);
        QString s("BattleTag base successfully written: ");
        QString bName = baseBytes.key(v, QString::number(v,16).toUpper());
        s.append(bName);
/*
        switch (v) {
            case 0x30: s.append("MedKit (flat)"); break;
            case 0x31: s.append("MedKit (rounded)"); break;
            case 0x40: s.append("Ammo pack (4 bullets)"); break;
            case 0x41: s.append("Ammo pack (2 bullets)"); break;
            case 0x50: s.append("T-Base 1 (red)"); break;
            case 0x51: s.append("T-Base 2 (blue)"); break;
            case 0x52: s.append("T-Base 3 (yellow)"); break;
            case 0x53: s.append("T-Base 4 (green)"); break;
            default: s.append(QString::number(v, 16).toUpper()); break;
        }
*/
        m.setText(s);
        m.exec();
        return;

    }

}
