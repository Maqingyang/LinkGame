#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "Layout.h"
#include <QPainter>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setMinimumSize(900, 700);
    setMaximumSize(900, 700);
    backGround=QPixmap(":/new/prefix2/background/backgound3.jpg").scaled(this->size());
    music_hawaii=new QSound(":/new/prefix3/music/Annie_s W_onderland.wav");
    music_sunny=new QSound(":/new/prefix3/music/Evening.wav");
    music_trip=new QSound(":/new/prefix3/music/piano.wav");
    music_test=new QSound(":/new/prefix3/music/piano.wav");
    music_now=music_test;
    music_now->setLoops(QSound::Infinite);
    music_now->play();
    lay = new Layout(this);

    this->setCentralWidget(lay);

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.drawPixmap(this->rect(),backGround);
}



void MainWindow::on_save1_triggered()
{
   lay->gameSave(1);


}

void MainWindow::on_save2_triggered()
{
    lay->gameSave(2);

}

void MainWindow::on_save3_triggered()
{
    lay->gameSave(3);

}

void MainWindow::on_save4_triggered()
{
    lay->gameSave(4);

}

void MainWindow::on_load1_triggered()
{
    lay->gameLoad(1);
}

void MainWindow::on_load2_triggered()
{
    lay->gameLoad(2);

}

void MainWindow::on_load3_triggered()
{
    lay->gameLoad(3);

}

void MainWindow::on_load4_triggered()
{
    lay->gameLoad(4);

}

void MainWindow::on_back4_triggered()
{
    backGround=QPixmap(":/new/prefix2/background/background4.jpg").scaled(this->size());
    this->repaint();

}

void MainWindow::on_back1_triggered()
{
    backGround=QPixmap(":/new/prefix2/background/background1.jpg").scaled(this->size());
    this->repaint();

}

void MainWindow::on_back2_triggered()
{
    backGround=QPixmap(":/new/prefix2/background/background2.jpg").scaled(this->size());
    this->repaint();

}

void MainWindow::on_back3_triggered()
{
    backGround=QPixmap(":/new/prefix2/background/backgound3.jpg").scaled(this->size());
    this->repaint();

}

void MainWindow::on_actiontrip_triggered()
{
    music_now->stop();
    music_now=music_trip;
    music_now->setLoops(QSound::Infinite);
    music_now->play();
}

void MainWindow::on_actionsunny_triggered()
{
    music_now->stop();
    music_now=music_sunny;
    music_now->setLoops(QSound::Infinite);
    music_now->play();
}

void MainWindow::on_actionHawaii_triggered()
{
    music_now->stop();
    music_now=music_hawaii;
    music_now->setLoops(QSound::Infinite);
    music_now->play();
}
