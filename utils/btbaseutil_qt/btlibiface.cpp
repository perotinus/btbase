#include <QApplication>
#include <QMessageBox>
#include <QString>

extern "C" {
#include "../../src/libbtbase.h"
}

#include "btlibiface.h"

BTLibIface::BTLibIface() 
{
    mm = 0;
        
    int status = btbaselib_setup();
    
    if (status != BSETUP_OK) {
        QMessageBox m;
        m.setText("NFC device not found.");
        m.exec();
        exit(-1);
    }

}

void 
BTLibIface::setmm(unsigned char x) 
{
        mm = x; 
}

void 
BTLibIface::readBase () 
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
        case 0x30: s.append("MED1"); break;
        case 0x31: s.append("MED2"); break;
        case 0x40: s.append("AMMO1"); break;
        case 0x41: s.append("AMMO2"); break;
        case 0x50: s.append("TBASE1"); break;
        case 0x51: s.append("TBASE2"); break;
        case 0x52: s.append("TBASE3"); break;
        case 0x53: s.append("TBASE4"); break;
        default: s.append(QString::number(btype, 16)); break;
    }

    QMessageBox m;
    m.setText(s);
    m.exec();
    return;
}
