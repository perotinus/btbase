#ifndef BTLIBIFACE_H
#define BTLIBIFACE_H

#include <QObject>

class BTLibIface : public QObject
{

Q_OBJECT

public:

    BTLibIface();
    void readBase(); 
    void writeBase(unsigned char v);

};

#endif
