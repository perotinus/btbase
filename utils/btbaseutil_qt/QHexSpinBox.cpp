#include "QHexSpinBox.h"

QHexSpinBox::QHexSpinBox(QWidget *parent) : QSpinBox(parent)
{
    //Custom range for this app
    setRange(0,15);
    validator = new QRegExpValidator(QRegExp("[0-9A-Fa-f]{1,2}"), this);
}

QValidator::State QHexSpinBox::validate(QString &text, int &pos) const
{
    return validator->validate(text, pos);
}

QString QHexSpinBox::textFromValue(int value) const
{
    return QString::number(value, 16).toUpper();
}

int QHexSpinBox::valueFromText(const QString &text) const
{
    return text.toInt(0, 16);
}    
