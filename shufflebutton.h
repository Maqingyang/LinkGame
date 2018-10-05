#ifndef SHUFFLEBUTTON_H
#define SHUFFLEBUTTON_H

#include <QPushButton>

class ShuffleButton : public QPushButton
{
    Q_OBJECT
public:
    explicit ShuffleButton(QWidget *parent = 0);
    void shuffleStart(int rest_times=0);
    void shuffleEnd();
    int getTimes(){return times;}
public slots:
    void reduceShuffle();
private:
    int times;//剩余重排次数
};

#endif // SHUFFLEBUTTON_H
