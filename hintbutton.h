#ifndef HINTBUTTON_H
#define HINTBUTTON_H

#include <QPushButton>

class HintButton : public QPushButton
{
    Q_OBJECT
public:
    HintButton(QWidget *parent = 0);
    void hintStart(int rest_times =3);
    void hintEnd();
    int getTimes(){return times;}
public slots:
    void reduceHint();
private:
    int times;//剩余提示次数
};

#endif // HINTBUTTON_H
