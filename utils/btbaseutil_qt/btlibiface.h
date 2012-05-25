#include <QObject>

class BTLibIface : public QObject
{
    Q_OBJECT

public:

    BTLibIface();

public slots:

    void setmm(unsigned char x);
    void readBase(); 

private:
    unsigned char mm; 
};
