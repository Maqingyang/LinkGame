#include "SearchRoute.h"
#include <QVector>
#include <QQueue>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <QDebug>

//遍历时用到的四个方向
const int fx[] = {-1, 0, 1, 0};
const int fy[] = {0, -1, 0, 1};
//最小转弯问题中使用到的结点
struct Node {
    int f, x, y;//方向，坐标
    Node() {}
    Node(int f, int x, int y) :
        f(f), x(x), y(y) {}
};

SearchRoute::SearchRoute(const QVector<QVector<int> >& map) :
    map_copy(map)
{
    n = map.size();
    m = map[0].size();
    routeX.setSharable(false);
    routeY.setSharable(false);
}
SearchRoute::~SearchRoute()
{
//    for(int i=0;i<4;i++)
//    {
//        delete[] dis[i];
//        for(int j=0;j<n;j++)
//            delete[] dis[i][j];
//    }
//        delete[] dis;

}

int SearchRoute::minTurn(int stx, int sty, int edx, int edy) {
    //return 0;
    int temp = map_copy[stx][sty];
    map_copy[stx][sty] = map_copy[edx][edy] = -1;


    qDebug()<<"create dis[][][]";
//    for(int i=0;i<4;i++)
//    {
//        dis[i] = new int* [n];
//        for(int j=0;j<n;j++)
//            dis[i][j] = new int[m];
//    }


//    for(int i=0;i<4;i++)
//        for(int j=0;j<n;j++)
//            for(int k=0;k<m;k++)
//                dis[i][j][k]=0;


    memset(dis, 0x3f, sizeof(dis));

    QQueue<Node>* pre = new QQueue<Node>();
    QQueue<Node>* cur = new QQueue<Node>();
    int step = 0;
    for(int i = 0; i < 4; ++i) pre->push_back(Node(i, stx, sty));

    for(int i = 0; i < 4; ++i) dis[i][stx][sty] = 0;



    while(step <= MAXTURN) {
        if(pre->empty()) {
            step = MAXTURN + 1;
            break;
        }


        Node t = pre->front(); pre->pop_front();
        if(dis[t.f][t.x][t.y] != step) continue;
        if(t.x == edx && t.y == edy) break;
        for(int i = 0; i < 4; ++i) {
            if((t.f + 2) % 4 == i) continue;
            int x = t.x + fx[i], y = t.y + fy[i], d = dis[t.f][t.x][t.y] + (t.f != i);//方向不同即转弯
            if(0 <= x && x < n && 0 <= y && y < m && map_copy[x][y] == -1 && d < dis[i][x][y]) {
                dis[i][x][y] = d;
                if(t.f == i) pre->push_back(Node(i, x, y));
                else cur->push_back(Node(i, x, y));
            }
        }



        if(pre->empty()) {
            step++;
            std::swap(pre, cur);
        }
    }
    map_copy[stx][sty] = map_copy[edx][edy] = temp;
    delete pre;
    delete cur;

    if(step > MAXTURN) return -1;

    routeX.clear();
    routeY.clear();
    int x = edx, y = edy, f = 0;
    for(int i = 1; i < 4; ++i)
        if(dis[i][x][y] < dis[f][x][y]) f = i;
    while(x != stx || y != sty) {
        routeX.push_back(x);
        routeY.push_back(y);
        for(int i = 0; i < 4; ++i) {
            if((i + 2) % 4 == f) continue;
            int px = x - fx[f], py = y - fy[f];
            if(dis[f][x][y] == dis[i][px][py] + (f != i)) {
                x = px, y = py, f = i;
                break;
            }
        }
    }
//    qDebug()<<"spot 5";

    routeX.push_back(x);
    routeY.push_back(y);
    return step;
}

void SearchRoute::getRoute(QVector<int> &routeX, QVector<int> &routeY) {
    routeX = this->routeX;
    routeY = this->routeY;
}
