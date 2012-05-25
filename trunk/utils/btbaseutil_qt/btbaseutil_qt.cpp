#include <QApplication>
#include <QtGui>
#include "BT_QWidget.h"
#include "btlibiface.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    BTLibIface btli;

    QPushButton *read = new QPushButton("&Read TBase");
    QPushButton *writeNamed = new QPushButton("Write &type");
    QPushButton *writeMM = new QPushButton("Write &major-minor");

    QObject::connect(read, SIGNAL(clicked()), &btli, SLOT(readBase())); 

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(read);

    BT_QWidget window;
    window.setLayout(layout);
   
    window.show();
    return app.exec();
}
