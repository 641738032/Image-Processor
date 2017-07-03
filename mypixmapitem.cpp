#include"mypixmapitem.h"
#include<iostream>
using namespace std;

MyPixmapItem::MyPixmapItem(QGraphicsPixmapItem *parent):
       QGraphicsPixmapItem(parent),
     RATE(0.1)
{

}

void MyPixmapItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    //cout<<"press"<<endl;
}

void MyPixmapItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
     cout<<"move"<<endl;
     setPos(pos() +
            mapToParent(event->pos()) -
            mapToParent(event->lastPos())
            );
}

void MyPixmapItem::wheelEvent(QGraphicsSceneWheelEvent *event)
{
     cout<<"wheel"<<endl;
     int delta = event->delta();
     double factor = scale();
     if(delta>0)
         factor *= (1 + RATE);
     if(delta<0)
         factor *= (1 - RATE);
     setTransformOriginPoint(boundingRect().width()/2,boundingRect().height()/2);
     setScale(factor);     
}

void MyPixmapItem::reset()//图片复原
{
    setPos(0,0);
    setScale(1.0);
}
