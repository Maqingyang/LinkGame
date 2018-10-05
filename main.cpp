#include "Layout.h"
#include "MainWindow.h"
#include <QApplication>
#include <QPixmap>
#include <QDebug>
#include <QWidget>
#include <QString>

int main(int argc, char *argv[])
{
    QApplication *app = new QApplication(argc, argv);

    MainWindow *mw=new MainWindow;

    app->setActiveWindow(mw);
    mw->show();




    return app->exec();
}
