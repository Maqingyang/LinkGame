#ifndef GAMESTRUCTURE_H
#define GAMESTRUCTURE_H

#include <CardWidget.h>
#include <QWidget>
#include <QVector>
#include <Qlist>
#include <QTimer>
#include <QPoint>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
//卡片的排版
//卡片的连接、消去
enum{NUMOFN=14,NUMOFM=11};


namespace Ui {
class GameStructure;
}



class GameStructure : public QWidget
{
    Q_OBJECT

public:
    explicit GameStructure(QWidget *parent = 0,const GameStructure *game=0);
    ~GameStructure();
    bool gameSave(int i,int rest_time,int rest_hint,int rest_shuffle);
    bool gameLoad(int i);
    void setGameStat(bool stat){gameStat=stat;}
//    int test_int =100;
signals:
    void gameIsOn(int rest_time,int rest_shuffle,int rest_hint);
    void gameIsOver();
    void removeOnePair();
    void replayIsOn();
public slots:
    void onClickedCard(CardWidget*);
    void new_game();
    void gameEnd();
    void shuffleUntilExist();
    void shuffle();
    void showHint();
protected:
    bool canDelAndDel(CardWidget* pre,CardWidget* curr);
    bool isRemoveExist();
    virtual void paintEvent(QPaintEvent *event);
    QPoint getCentre(int i,int j);



private:    
    Ui::GameStructure *ui;
    QVector<QVector<int>> map;//提取出对应（i，j）卡片的image类型
    QVector<QVector<CardWidget *>> card;//卡片集合
    int n ,m;//行、列数
    QList<QPixmap> pixList;//图片集合
    static const int IMAGES_NUM = 30;//最大卡片数量
    CardWidget* preClickedCard;//之前选中的卡片
    bool gameStat=0;//游戏状态
    int numOfRest;//剩余卡片数量
    int hintStX, hintStY, hintEdX, hintEdY;//提示用
    QTimer *showRouteTimer;//显示路线计时器
    QVector<int> routeX,routeY;//路线
    int score;







};

#endif // GAMESTRUCTURE_H
