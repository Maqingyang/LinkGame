#include "shufflebutton.h"
#include <QString>
#include <QPushButton>

ShuffleButton::ShuffleButton(QWidget *parent) :
    QPushButton(parent)
{
    this->setEnabled(false);
    setText("洗牌(0)");
    connect(this, SIGNAL(clicked()), this, SLOT(reduceShuffle()));
}
//游戏开始时的初始化
void ShuffleButton::shuffleStart(int rest_times) {
    times = rest_times;
    setText(QString().sprintf("洗牌(%d)", times));
    if(times==0)
        this->setEnabled(false);
    else
        this->setEnabled(true);
}
//游戏结束
void ShuffleButton::shuffleEnd() {
    this->setEnabled(false);
    setText(QString("洗牌(0)"));
}
//提示次数减少
void ShuffleButton::reduceShuffle() {
    if(--times == 0) this->setEnabled(false);
    setText(QString().sprintf("洗牌(%d)", times));
}
