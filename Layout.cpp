#include "Layout.h"
#include "ui_Layout.h"
#include "GameStructure.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFont>
#include <QString>
#include <QMessageBox>
#include <QDebug>

Layout::Layout(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Layout)
{
    ui->setupUi(this);
    //设置字体
    font = QFont("UTF-8", 20, 5);
    //设置大小
    //初始化进度条
    progressbar=new QProgressBar;
    progressbar->setMinimum(0);
    progressbar->setMaximum(200);
    //创建暂停按钮
    stopButton=new QPushButton;
    stopButton->setFont(font);
    stopButton->setEnabled(true);
    stopButton->setText(QString("停止"));
    stopButton->hide();
    //初始化计时器
    counter=new QTimer(this);
    //初始化Qlabel
    hurryPic_1=QPixmap(":/new/prefix4/others/timg_opaque.png");
    hurryLabel_1=new QLabel(this);
    hurryLabel_1->setPixmap(hurryPic_1);
    hurryLabel_1->setFixedSize(100,80);
    hurryLabel_1->setScaledContents(true);
    hurryLabel_1->move(650,20);
    hurryLabel_1->hide();
    hurryPic_2=QPixmap(":/new/prefix4/others/time_opaque2.png");
    hurryLabel_2=new QLabel(this);
    hurryLabel_2->setPixmap(hurryPic_2);
    hurryLabel_2->setFixedSize(100,80);
    hurryLabel_2->setScaledContents(true);
    hurryLabel_2->move(650,20);
    hurryLabel_2->hide();

    //设置开始、结束按钮
    startButton=new QPushButton;
    startButton->setFont(font);
    startButton->setEnabled(true);
    endButton=new QPushButton;
    endButton->setFont(font);
    startButton->setText(QString("出发"));
    endButton->setText(QString("结束"));
    endButton->setEnabled(false);
    //设置提示、洗牌按钮
    hintButton=new HintButton;
    hintButton->setFont(font);
    shuffleButton=new ShuffleButton;
    shuffleButton->setFont(font);
    //设置左布局
    leftLayout = new QVBoxLayout;
    game = new GameStructure;
    leftLayout->addWidget(game);
    leftLayout->addWidget(progressbar);
    //设置右布局
    rightLayout = new QVBoxLayout;
    rightLayout->addWidget(startButton);
    rightLayout->addWidget(endButton);
    rightLayout->addWidget(hintButton);
    rightLayout->addWidget(shuffleButton);
    //设置总体布局
    wholeLayout = new QHBoxLayout(this);
    wholeLayout->addLayout(leftLayout);
    wholeLayout->addLayout(rightLayout);
    wholeLayout->setSpacing(10);
    //连接
    connect(startButton,SIGNAL(clicked(bool)),game,SLOT(new_game()));
    connect(endButton,SIGNAL(clicked()),game,SLOT(gameEnd()));
    connect(stopButton,SIGNAL(clicked(bool)),this,SLOT(replayStop()));
    connect(game,SIGNAL(gameIsOn(int,int,int)),this,SLOT(gameStartSet(int,int,int)));
    connect(game, SIGNAL(gameIsOver()),this,SLOT(gameEndSet()));
    connect(game,SIGNAL(removeOnePair()),this,SLOT(incProgressBar()));
    connect(game,SIGNAL(removeOnePair()),this,SLOT(gameCopy()));
    connect(hintButton, SIGNAL(clicked()), game, SLOT(showHint()));
    connect(shuffleButton, SIGNAL(clicked()), game, SLOT(shuffleUntilExist()));
    connect(this,SIGNAL(progressBarIsZero()),game,SLOT(gameEnd()));
    connect(counter,SIGNAL(timeout()),this,SLOT(runProgressBar()));
}

Layout::~Layout()
{
    delete ui;
}
void Layout::gameStartSet(int rest_time, int rest_hint, int rest_shuffle)
{
    //删除链表
//    qDebug()<<"before delete link_list in gameStartSet";
       while(node_start)
       {
           node_end=node_start->next;
           delete node_start;
           node_start=node_end;

       }
//    qDebug()<<"finish!";

    qDebug()<<"rest_time:"<<rest_time;
    if(rest_time>=0)
    {
        progressbar->setValue(rest_time);
    }
    else
    progressbar->setValue(progressbar->maximum());



    startButton->setEnabled(false);
    endButton->setEnabled(true);
    hintButton->hintStart(rest_hint);
    shuffleButton->shuffleStart(rest_shuffle);
    counter->start(300);
}
void Layout::gameEndSet()
{
    if(!replayStat)
    {
        if(QMessageBox::question(this,"兄弟","回放录像吗？",QMessageBox::Yes,QMessageBox::No)==QMessageBox::Yes)
            replay();
    }


//    qDebug()<<"5";

//删除链表
    while(node_start)
    {
        node_end=node_start->next;
        delete node_start;
        node_start=node_end;

    }
//    qDebug()<<"6";

    startButton->setEnabled(true);
    endButton->setEnabled(false);
    hintButton->hintEnd();
    shuffleButton->shuffleEnd();
    counter->stop();
    progressbar->setValue(progressbar->maximum());
    replayStat=0;
    game->setGameStat(false);

    //qDebug()<<"7";


}
void Layout::runProgressBar()
{
    progressbar->setValue(progressbar->value()-1);
    if(progressbar->value()<=0)
    {
        emit progressBarIsZero();
        return;
    }
    if(progressbar->value()==180)
        hurryLabel_1->show();
    if(progressbar->value()<=175)
        hurryLabel_1->hide();

    if(progressbar->value()==190)
        hurryLabel_2->show();
    if(progressbar->value()<=185)
        hurryLabel_2->hide();
}
void Layout::incProgressBar()
{
   for(int i=0;i<20;i++)
       progressbar->setValue(progressbar->value()+1);

}
bool Layout::gameSave(int i)
{
    counter->stop();
    bool ret = game->gameSave( i, progressbar->value(),hintButton->getTimes(),shuffleButton->getTimes());
   if(ret) counter->start(300);
    return ret;
}
bool Layout::gameLoad(int i)
{
    return game->gameLoad(i);
}
//void Layout::replayStartSet()
//{
//    endButton->setEnabled(true);
//    startButton->setEnabled(false);
//    hintButton->setEnabled(false);
//    shuffleButton->setEnabled(false);
//    progressbar->setValue(progressbar->maximum());
//    counter->stop();

//}

replay_single* Layout::createNode()
{
        //qDebug()<<"start creating node";
        replay_single *node=new replay_single(this,game);

        return node;
}
void Layout::gameCopy()
{
    if(!node_start)
    {
        node_start=createNode();
        node_end=node_start;
    }
    else
    {
        node_end->next=createNode();
        node_end=node_end->next;
    }
}
void Layout::replay()
{

    if(!node_start) return;
    replayStat=1;
    stopButtonStat=0;

    endButton->setEnabled(false);
    startButton->setEnabled(false);
    hintButton->setEnabled(false);
    shuffleButton->setEnabled(false);
    progressbar->setValue(progressbar->maximum());
    counter->stop();

//    startButton=new QPushButton;
//    startButton->setFont(font);
//    startButton->setEnabled(true);
//    endButton=new QPushButton;
//    endButton->setFont(font);
//    startButton->setText(QString("出发"));
//    endButton->setText(QString("结束"));
//    endButton->setEnabled(false);
    stopButton->show();
    rightLayout->addWidget(stopButton);



    game_now=game;
    leftLayout->removeWidget(game);
    leftLayout->removeWidget(progressbar);
    node_end=node_start;
    game=&node_start->gamePicture;

    while(node_end)
    {
        leftLayout->addWidget(game);
        leftLayout->addWidget(progressbar);

        game->show();

        //延时1s
        QTime t;
        t.start();
        while(t.elapsed()<1000)
        QCoreApplication::processEvents();
        if(stopButtonStat)
            break;
        game->hide();
        node_end=node_end->next;
        game=&node_end->gamePicture;

    }

    leftLayout->removeWidget(game);
    leftLayout->removeWidget(progressbar);
    game=game_now;
    leftLayout->addWidget(game);
    leftLayout->addWidget(progressbar);

    stopButton->hide();
    rightLayout->removeWidget(stopButton);
    replayStat=0;
    stopButtonStat=0;
    if(QMessageBox::question(this,"提示","再看一遍吗",QMessageBox::Yes,QMessageBox::No)==QMessageBox::Yes)
        replay();
    return;
}
void Layout::replayStop()
{
    stopButtonStat=1;
}
