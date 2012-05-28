#ifndef BTLIBIFACE_H
#define BTLIBIFACE_H

#include <QObject>
#include <QStringList>
#include <QMap>

class BTLibIface : public QObject
{

Q_OBJECT

public:

    BTLibIface();
    void readBase(); 
    void writeBase(unsigned char v);

    //void setBaseNames(QStringList l) {baseNames = l;} 
    void setBaseBytes(QMap<QString, unsigned char> m) {baseBytes = m;}
    QMap<QString, unsigned char> getBaseBytes() {return baseBytes;}

private:

    //QStringList baseNames;
    QMap<QString, unsigned char> baseBytes;

};

#endif
