#include "CardWidget.h"
#include "ui_CardWidget.h"
#include <QPainter>
#include <QPen>
#include <QTimer>
#include <QTime>

CardWidget::CardWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CardWidget)
{
    ui->setupUi(this);
    int row = 0;
    int col = 0;
    bool isSelected = false;
    QPixmap image;
    hintTime=new QTimer(this);
    hintTime->setSingleShot(true);
    connect(hintTime, SIGNAL(timeout()), this, SLOT(repaint()));
}

CardWidget::~CardWidget()
{
    delete ui;
}

 void CardWidget::paintEvent(QPaintEvent *)
{
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1
    QPainter p(this);
    p.drawPixmap(1,1,image);
    if(isSelected)
    {
        QPen pen;
        pen.setColor(Qt::green);
        pen.setWidth(5);
        p.setPen(pen);
        p.drawRoundRect(this->rect());
    }
    if(hintTime->isActive())
    {
        QPen pen;
        pen.setColor(Qt::red);
        pen.setWidth(5);
        p.setPen(pen);
        p.drawEllipse(this->rect());
    }
 }
 void CardWidget::mousePressEvent(QMouseEvent *)
{
    emit beClicked(this);
}


int CardWidget::get_row()
{
    return row;
}

int CardWidget::get_col()
{
    return col;
}

void CardWidget::set_row(int r)
{
    row = r;
}

void CardWidget::set_col(int c)
{
    col = c;
}

void CardWidget::set_selected(bool select)
{
    isSelected = select;
}

void CardWidget::set_image(QPixmap image)
{
    this->image = image;
}

void CardWidget::showHint()
{
    hintTime->start(2000);
    this->repaint();
}
//CardWidget& CardWidget::operator =(const CardWidget &right)
//{
//    if(this==&right) return *this;
////    int row;//x轴坐标
////    int col;//y轴坐标
////    bool isSelected;//是否被选中
////    QPixmap image;
////    QTimer* hintTime;
//    this->row=right.row;
//    this->col=right.col;
//    this->isSelected=right.isSelected;
//    this->image=right.image;
//    this->hintTime=right.hintTime;
//    return *this;
//}
