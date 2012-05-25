#include <QWidget>
#include <QCloseEvent>
#include <QtGui>

class BT_QWidget: public QWidget
{

Q_OBJECT

public:
    BT_QWidget();

protected:
    void closeEvent(QCloseEvent *event);
};
