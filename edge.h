// SPDX-License-Identifier: BSD-3-Clause
#ifndef EDGE_H
#define EDGE_H
#include "node.h"
#include <QDebug>
#include <QGraphicsItem>
#include <QPainter>
#include <QPointF>

class Edge :public QGraphicsItem
{
public:
    Edge(Node * pA, Node* pB);
    ~Edge();

    virtual QRectF boundingRect() const;
    virtual void paint(QPainter * painter,
               const QStyleOptionGraphicsItem * option,
               QWidget * widget);
    virtual int type() const ;
    QJsonObject returnJsonObj();

    int ID;
    Node * nodeA;
    Node * nodeB;
private:
    enum { Type = UserType + 2 };
};

#endif // EDGE_H
