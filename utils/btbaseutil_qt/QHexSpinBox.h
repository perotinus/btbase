#include <QSpinBox>
#include <QRegExpValidator>

class QHexSpinBox : public QSpinBox
{

Q_OBJECT

public:
    QHexSpinBox(QWidget *parent = 0);

protected:
    QValidator::State validate(QString &text, int &pos) const;
    int valueFromText(const QString &text) const;
    QString textFromValue(int value) const;

private:
    QRegExpValidator *validator;

};
