#ifndef CARDWIDGET_H
#define CARDWIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QPaintEvent>
#include <QTimer>

//卡片类
//该卡片的行和列
//该卡片的图片
//该卡片是否被选中
//该卡片选中后的效果

namespace Ui {
class CardWidget;
}

class CardWidget : public QWidget
{

    Q_OBJECT

public:
    static const int IMAGESIZE = 49;

    explicit CardWidget(QWidget *parent = 0);
    ~CardWidget();
    int get_row();
    int get_col();
    void set_row(int r);
    void set_col(int c);
    void set_selected(bool select);
    void set_image(QPixmap image);
    void showHint();

//    CardWidget& operator =(const CardWidget &right);
protected:
    virtual void paintEvent(QPaintEvent *);
    virtual void mousePressEvent(QMouseEvent *);

signals:
    void beClicked(CardWidget*);
private:
    Ui::CardWidget *ui;
    int row;//x轴坐标
    int col;//y轴坐标
    bool isSelected;//是否被选中
    QPixmap image;
    QTimer* hintTime;

};

#endif // CARDWIDGET_H
