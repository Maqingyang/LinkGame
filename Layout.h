#ifndef LAYOUT_H
#define LAYOUT_H
#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFont>
#include <QTime>
#include <QTimer>
#include <QProgressBar>
#include "hintbutton.h"
#include "shufflebutton.h"
#include "GameStructure.h"
#include <QLabel>
//游戏的启动
//游戏界面的排版
namespace Ui {
class Layout;
}

//声明链表replay_single
struct replay_node;

typedef struct replay_node replay_single;

class Layout : public QWidget
{
    Q_OBJECT

public:
    explicit Layout(QWidget *parent = 0);
    ~Layout();
    void gameRead();
    void changeBack();
    void changeMusic();
signals:
    void progressBarIsZero();
public slots:
    void gameStartSet(int rest_time, int rest_hint, int rest_shuffle);
    void gameEndSet();
    void runProgressBar();
    void incProgressBar();
    bool gameSave(int);
    bool gameLoad(int);
    void gameCopy();
    void replayStop();
//    void replayStartSet();

private:
    Ui::Layout *ui;
    QLabel* hurryLabel_1;
    QPixmap hurryPic_1;
    QLabel* hurryLabel_2;
    QPixmap hurryPic_2;
    QPushButton* startButton;
    QPushButton* endButton;
    QPushButton* stopButton=0;
    HintButton* hintButton;
    ShuffleButton* shuffleButton;
    QTimer* counter;
    QProgressBar* progressbar;
    QVBoxLayout* leftLayout;
    QVBoxLayout* rightLayout;
    QHBoxLayout* wholeLayout;
    QFont font;//字体
    GameStructure* game=0;
    GameStructure* game_now=0;//记忆game
//回放变量和函数
    bool replayStat=0;
    replay_single *node_start=NULL;
    replay_single *node_end=NULL;
    bool stopButtonStat=0;
    replay_single *createNode();
    void replay();


};
//回放链表
struct replay_node
{
    GameStructure gamePicture;
    struct replay_node *next;

    replay_node( Layout*lay, const GameStructure *game):
        gamePicture(lay,game)
    {
        next=NULL;
    }
};
#endif // LAYOUT_H
