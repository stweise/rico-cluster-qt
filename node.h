// SPDX-License-Identifier: BSD-3-Clause
#ifndef NODE_H
#define NODE_H
#include <QDebug>
#include <QGraphicsItem>
#include <QPainter>
#include <QPointF>

class Node :public QGraphicsItem
{
public:
    Node(QPointF center, QString label);
    Node(QJsonObject json);
    ~Node();

    virtual QRectF boundingRect() const;
    virtual void paint(QPainter * painter,
               const QStyleOptionGraphicsItem * option,
               QWidget * widget);
    virtual int type() const ;
    QJsonObject returnJsonObj();

    int ID;
    QString nodelabel;

private:
    enum { Type = UserType + 1 };
};

#endif // NODE_H
