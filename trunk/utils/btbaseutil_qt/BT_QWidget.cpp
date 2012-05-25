#include "BT_QWidget.h"
extern "C" {
#include "../../src/libbtbase.h"
}

BT_QWidget::BT_QWidget() {}

void BT_QWidget::closeEvent(QCloseEvent *event) {
    btbaselib_close();
}
