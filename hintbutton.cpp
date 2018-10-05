#include "hintbutton.h"
#include <QString>
//初始化
HintButton::HintButton(QWidget *parent) :
    QPushButton(parent)
{
    this->setEnabled(false);
    setText(QString("提示(0)"));
    connect(this, SIGNAL(clicked()), this, SLOT(reduceHint()));
}
//游戏开始时的初始化
void HintButton::hintStart(int rest_times) {
    times = rest_times;//设置提示次数
    setText(QString().sprintf("提示(%d)", times));
    if(times==0)
        this->setEnabled(false);
    else
        this->setEnabled(true);
}
//游戏结束
void HintButton::hintEnd() {
    this->setEnabled(false);
    setText(QString("提示(0)"));
}
//提示次数减少
void HintButton::reduceHint() {
    if(--times == 0) this->setEnabled(false);
    setText(QString().sprintf("提示(%d)", times));
}
