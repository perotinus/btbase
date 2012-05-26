#include <QApplication>

#include "BT_QMainWindow.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    //Setup the main window
    BT_QMainWindow *window = new BT_QMainWindow;

    window->show();
    window->setWindowTitle("BTBaseUtil");
    return app.exec();
}


