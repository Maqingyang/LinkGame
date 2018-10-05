#ifndef SEARCHROUTE_H
#define SEARCHROUTE_H

#include <QVector>
#include <QWidget>
#include "CardWidget.h"
#include "GameStructure.h"


class SearchRoute{
public:
    SearchRoute(const QVector<QVector<int> >& map) ;
    ~SearchRoute();
    int minTurn(int stx, int sty, int edx, int edy);
    void getRoute(QVector<int> &routeX, QVector<int> &routeY);
private:
    int n,m;
    static const int MAXTURN = 2;
    QVector<QVector<int>> map_copy;
    QVector<int> routeX,routeY;
    int dis[4][NUMOFN][NUMOFM];
};

#endif // SEARCHROUTE_H
