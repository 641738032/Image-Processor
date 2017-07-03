#ifndef MYPIXMAPITEM_H
#define MYPIXMAPITEM_H

#include<QObject>
#include<QGraphicsPixmapItem>
#include<QGraphicsSceneMouseEvent>
#include<QGraphicsSceneWheelEvent>

class MyPixmapItem : public QObject,public QGraphicsPixmapItem//继承QGraphicsPixmapItem这个类
{
    Q_OBJECT

public:
    explicit MyPixmapItem(QGraphicsPixmapItem *parent = 0);
    const double RATE ;
public:
    void reset();



private:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void wheelEvent(QGraphicsSceneWheelEvent *event);


signals:


public slots:

};

#endif // MYPIXMAP_H
