#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Layout.h"
#include <QPixmap>
#include <QSound>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
protected:
    virtual void paintEvent(QPaintEvent *event);

private slots:

    void on_save1_triggered();
    void on_save2_triggered();

    void on_save3_triggered();

    void on_save4_triggered();

    void on_load1_triggered();

    void on_load2_triggered();

    void on_load3_triggered();

    void on_load4_triggered();

    void on_back4_triggered();

    void on_back1_triggered();

    void on_back2_triggered();

    void on_back3_triggered();

    void on_actiontrip_triggered();

    void on_actionsunny_triggered();

    void on_actionHawaii_triggered();

signals:
    void signal_on_save1_triggered(int i);

private:
    Ui::MainWindow *ui;
    Layout* lay;
    QPixmap backGround;
    QSound *music_hawaii;
    QSound *music_sunny;
    QSound *music_trip;
    QSound *music_test;
    QSound *music_now;
};

#endif // MAINWINDOW_H
