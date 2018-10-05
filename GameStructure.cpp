#include "GameStructure.h"
#include "ui_GameStructure.h"
#include "SearchRoute.h"
#include <QTime>
#include <qrandom.h>
#include <QDebug>
#include <QPainter>
#include <QMessageBox>
GameStructure::GameStructure(QWidget *parent, const GameStructure *game) :
    QWidget(parent),
    ui(new Ui::GameStructure)
{
    ui->setupUi(this);


    //设置行、列
    n = NUMOFN;
    m = NUMOFM;
    setMinimumSize(49 * n, 49 * m);
    setMaximumSize(49 * n, 49 * m);
    //读入图像
    pixList.clear();
    for(int i=0;i< IMAGES_NUM; i++)//读入图片
    {
        QString path;
        path.sprintf(":/new/prefix1/image/%d.jpg",i+1);
        QPixmap q;
        q.load(path);

        pixList<<q;
//        qDebug()<<pixList[i].isNull();
    }
    //初始化种子
    QTime t=QTime::currentTime();
    qsrand(t.second()+1000*t.msec());

    //初始化mat、card集合
    map = QVector<QVector<int>>(n,QVector<int>(m,-1));
    card = QVector<QVector<CardWidget*>>(n,QVector<CardWidget*>(m));
    //初始化其他变量
    preClickedCard=0;
    gameStat =0;
//    qDebug()<<m<<n<<gameStat;
    numOfRest = 0;
    //初始化显示路线计时器
    showRouteTimer=new QTimer(this);
    showRouteTimer->setSingleShot(true);
    connect(showRouteTimer,SIGNAL(timeout()),this,SLOT(repaint()));

    if(game)
    {
//        qDebug()<<"copy game";
//        qDebug()<<"copy map";
        //复制map
        for(int i=0;i<n;i++)
            for(int j=0;j<m;j++)
                map[i][j]=game->map[i][j];
//        qDebug()<<"use copied map to create card";
        //新建卡片
        for(int i=1;i<n-1;i++)
            for(int j=1;j<m-1;j++)
            {

                card[i][j] = new CardWidget(this);
                card[i][j]->setGeometry(i*49,j*49,49,49);
                card[i][j]->set_row(j);
                card[i][j]->set_col(i);
                card[i][j]->set_selected(false);
                //                qDebug()<<i<<" "<<" "<<j<<":"<<map[i][j];
                if(map[i][j]!=-1)
                 card[i][j]->set_image(pixList[map[i][j]]);



            }
        this->hide();
    }

}

GameStructure::~GameStructure()
{
    if(gameStat)
    {
        for(int i=1;i<n-1;i++)
            for(int j=1;j<m-1;j++)
                delete card[i][j];
    }
    delete ui;
}

void GameStructure::paintEvent(QPaintEvent *event)
{
    if(!gameStat) return;
    if(showRouteTimer->isActive())
    {
        QPen pen;
        pen.setColor(Qt::darkMagenta);
        pen.setWidth(4);
        QPainter p(this);
        p.setPen(pen);

        for(int i=0;i<routeX.size()-1;i++)
        {
            QPoint a=getCentre((int)(routeX[i]),(int)(routeY[i]));
            QPoint b=getCentre(routeX[i+1],routeY[i+1]);
            p.drawLine(a,b);

        }

    }
}
void GameStructure::new_game()
{
    //初始化其他变量
    preClickedCard = 0;
    gameStat =1;
    numOfRest = (n-2)*(m-2);
    //初始化矩阵
    for(int i=0;i<n;i++) map[i][0]=map[i][m-1]=-1;
    for(int j=0;j<m;j++) map[0][j]=map[n-1][j]=-1;

    for(int i=1;i<n-1;i++)
        for(int j=1;j<m-1;j++)
        {
            if(i%2 == 0) map[i][j] = map[i-1][j];
            else map[i][j] = qrand() % IMAGES_NUM;
        }
    for(int i=1;i<n-1;i++)
        for(int j=1;j<m-1;j++)
        {

            card[i][j] = new CardWidget(this);
            connect(card[i][j], SIGNAL(beClicked(CardWidget*)), this, SLOT(onClickedCard(CardWidget*)));
            card[i][j]->setGeometry(i*49,j*49,49,49);
            card[i][j]->set_row(j);
            card[i][j]->set_col(i);
            card[i][j]->set_image(pixList[map[i][j]]);
            card[i][j]->show();


        }
    shuffleUntilExist();
    score=0;//初始化分数
    emit gameIsOn(-1,3,3);
}
//emit gameEnd();
//if(restCard == 0) {
//    QMessageBox::information(this, "你胜利了！", "最终得分：" + QString::number(score));
//}
////隐藏游戏画面
//for(int i = 1; i < n - 1; ++i)
//    for(int j = 1; j < m - 1; ++j) delete card[i][j];
//gameIsOn = false;
void GameStructure::gameEnd()
{


    if(numOfRest==0)QMessageBox::information(this,"你赢了！","最终得分：" + QString::number(score));
    else QMessageBox::information(this,"时间到！","大侠重新来过");

    for(int i=1;i<n-1;i++)
        for(int j=1;j<m-1;j++)
        {
//            qDebug()<<i<<j<<card[i][j]->isActiveWindow();
            delete card[i][j];
        }
    score=0;//清零分数



    emit gameIsOver();


}

void GameStructure::onClickedCard(CardWidget * clickedCard)
{

    if(preClickedCard==clickedCard) return;
    if(preClickedCard == 0) {
        preClickedCard=clickedCard;
        clickedCard->set_selected(true);
        clickedCard->repaint();
        return;
    }
    if(canDelAndDel(preClickedCard,clickedCard))
    {
        qDebug()<<"can be deleted";
        preClickedCard->set_selected(false);
        preClickedCard=0;
        if(numOfRest==0) {
            gameEnd();
            return;
        }
        if(numOfRest==2) return;
        while(!isRemoveExist())
            shuffle();

        return;
    }
    preClickedCard->set_selected(false);
    preClickedCard->repaint();
    clickedCard->set_selected(true);
    clickedCard->repaint();
    preClickedCard=clickedCard;
    return;

}
bool GameStructure::canDelAndDel(CardWidget *pre, CardWidget *curr)
{
    qDebug()<<"judge if can be deleted";




    int sy=pre->get_row();
    int sx=pre->get_col();
    int ey=curr->get_row();
    int ex=curr->get_col();
//    qDebug()<<"judge if the images are the same";
    if(!(map[sx][sy]==map[ex][ey])||map[sx][sy]==-1) return false;
//    qDebug()<<"images are the same,search route";
    SearchRoute route(map);
    if(route.minTurn(sx,sy,ex,ey)==-1) return false;
//    qDebug()<<"the route is searched!";
//    qDebug()<<pre->isVisible();
    pre->hide();
//    qDebug()<<"hide";
    curr->hide();
    map[sx][sy]=map[ex][ey]=-1;
    numOfRest-=2;
    //显示路径
    route.getRoute(routeX,routeY);
    showRouteTimer->start(200);
    this->repaint();
    //增加分数
    score+=100*(route.minTurn(sx,sy,ex,ey)+1);


    emit removeOnePair();

//    qDebug()<<numOfRest;



    return true;



}
bool GameStructure::isRemoveExist()
{
//    qDebug()<<"judge if remove exist";
//    for(int x = 1; x < n - 1; ++x) {
//        for(int y = 1; y < m - 1; ++y) {
//            if(x < i || (x == i && y <= j)) continue;
//            if(mat[i][j] != mat[x][y] || mat[x][y] == -1) continue;

    SearchRoute route(map);
    for(int i=1;i<n-1;i++)
        for(int j=1;j<m-1;j++){
            if(map[i][j]==-1) continue;
            for(int x=1;x<n-1;x++)
                for(int y=1;y<m-1;y++){
                    if(x<i||(x==i&&y<=j)) continue;
                    if(map[i][j]!=map[x][y]||map[x][y]==-1) continue;
                    if(route.minTurn(i,j,x,y)!=-1)
                         {
                         hintStX = i; hintStY = j; hintEdX = x; hintEdY = y;//提示用
                         return true;}
                }
        }
    qDebug()<<"remove not exist!";
    return false;
//    if(x < i || (x == i && y <= j)) continue;
//    if(map[i][j] != map[x][y] || map[x][y] == -1) continue;
//    if(graph->minTurn(i, j, x, y) != -1) {
//        //std::cout<<i<<' '<<j<<' '<<x<<' '<<y<<std::endl;//作弊专用
//        hintStX = i; hintStY = j; hintEdX = x; hintEdY = y;//提示用
//        delete graph;
//        return true;

}
void GameStructure::shuffle()
{
    qDebug()<<"before shuffle";
//     for(int v=0;v<m;v++)
//    {
//            for(int u=0;u<n;u++)
//            qDebug()<<map[u][v];
//        qDebug()<<endl;
//    }
//    qDebug()<<n<<m;

    QVector<int> temp;
    for(int i=1;i<n-1;i++)
        for(int j=1;j<m-1;j++)
        {
            temp.push_back(map[i][j]);
        }
    std::random_shuffle(temp.begin(),temp.end());
    for(int i=1, cnt = 0;i<n-1;i++)
        for(int j=1;j<m-1;j++)
        {
//            qDebug("cnt:%d",cnt);
            map[i][j] = temp[cnt++];
            card[i][j]->set_selected(false);
            if(map[i][j]==-1)
            {
                card[i][j]->hide();
//                qDebug()<<"hide!";
            }
            else {
//                qDebug("card[%d][%d]->set_image(pixList[%d]])",i,j,map[i][j]);
                card[i][j]->set_image(pixList[map[i][j]]);
                card[i][j]->show();
                card[i][j]->repaint();
            }
//            qDebug()<<"finish!";

        }


//    qDebug()<<n<<m;
//    qDebug()<<"after shuffle";
//    for(int u=0;u<n;u++)
//    {
//        for(int v=0;v<m;v++)
//            qDebug()<<map[u][v];
//        qDebug()<<endl;
//    }

}
void GameStructure::shuffleUntilExist()
{
    shuffle();
    while(!isRemoveExist())
        shuffle();

}
void GameStructure::showHint()
{
    card[hintStX][hintStY]->showHint();
    card[hintEdX][hintEdY]->showHint();
}
QPoint GameStructure::getCentre(int i, int j)
{
    QPoint point;
    point.setX(i*49+25);
    point.setY(j*49+25);
    return point;
}
bool GameStructure::gameSave(int i, int rest_time, int rest_hint, int rest_shuffle)
{
//    qDebug()<<"begin saving";
    if(!gameStat){
        QString string;
        string.sprintf("保存失败",i);
        QMessageBox::information(0,string,"请先开始游戏！");
        return 0;
    }
//    qDebug()<<"create save_file";
    QFile* save_file;
    switch (i) {
    case 1:
//        qDebug()<<"create new file";
        save_file=new QFile("data1.txt");
//        qDebug()<<"finish!";
        break;
    case 2:
        save_file=new QFile("data2.txt");
        break;
    case 3:
        save_file=new QFile("data3.txt");
        break;
    case 4:
        save_file=new QFile("data4.txt");
        break;
    default:
        QMessageBox::information(this,"保存失败","没有对应文件");
        return 0;
        break;
    }
//        qDebug()<<"open file";
        save_file->open(QIODevice::WriteOnly|QIODevice::Text);
//        qDebug()<<"finish!";

        QTextStream save_stream(save_file);
        //写入map
        save_stream<<numOfRest<<endl;
        for(int i=0;i<n;i++)
            for(int j=0;j<m;j++)
                save_stream<<map[i][j]<<endl;
        //写入得分
        save_stream<<score<<endl;
        //写入剩余时间、剩余提示次数、剩余洗牌次数
        save_stream<<rest_time<<endl;
        save_stream<<rest_hint<<endl;
        save_stream<<rest_shuffle<<endl;
        //写入结束,关闭文件
        save_file->close();
        delete save_file;
        QString file_name;
        file_name.sprintf("data%d.txt",i);
        QMessageBox::information(this,file_name,"保存成功");
        return 1;




}
bool GameStructure::gameLoad(int i)
{
    if(gameStat){
        QMessageBox::information(this,"载入失败","请先结束游戏");
        return 0;
    }
//打开文件
    QFile *load;
    switch (i) {
    case 1:
        load=new QFile("data1.txt");
        break;
    case 2:
        load=new QFile("data2.txt");
        break;
    case 3:
        load=new QFile("data3.txt");
        break;
    case 4:
        load=new QFile("data4.txt");
        break;
    default:
        QMessageBox::information(this,"","");
        return 0;
        break;
    }
    load->open(QIODevice::ReadOnly|QIODevice::Text);
    QTextStream load_stream(load);
    //初始化其他变量
    preClickedCard = 0;
    gameStat =1;
    numOfRest = load_stream.readLine().toInt();
    //初始化矩阵
    for(int i=0;i<n;i++)
        for(int j=0;j<m;j++)
             map[i][j]=load_stream.readLine().toInt();




//    qDebug()<<"in load game 1";
    for(int i=1;i<n-1;i++)
        for(int j=1;j<m-1;j++)
        {

            card[i][j] = new CardWidget(this);
            connect(card[i][j], SIGNAL(beClicked(CardWidget*)), this, SLOT(onClickedCard(CardWidget*)));
            card[i][j]->setGeometry(i*49,j*49,49,49);
            card[i][j]->set_row(j);
            card[i][j]->set_col(i);
            card[i][j]->hide();
//            qDebug()<<i<<j<<map[i][j];
            if(map[i][j]!=-1)
            {
                card[i][j]->set_image(pixList[map[i][j]]);
                card[i][j]->set_selected(false);
                card[i][j]->show();
                qDebug()<<i<<j<<card[i][j]->isHidden();
            }


        }
//    qDebug()<<"in load game 2"
    this->show();

 //初始化分数
    score=load_stream.readLine().toInt();
    qDebug()<<" loaded score"<<score;
    int rest_time=load_stream.readLine().toInt();
    int rest_hint=load_stream.readLine().toInt();
    int rest_shuffle=load_stream.readLine().toInt();
    load->close();
    emit gameIsOn(rest_time,rest_hint,rest_shuffle);
    return 1;
}
