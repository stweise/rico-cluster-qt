// SPDX-License-Identifier: BSD-3-Clause
#include "edge.h"
#include <QDebug>
#include <QPointF>
#include <QJsonObject>
static int instantionID = 0;
Edge::Edge(Node* npA, Node* npB)
{
    if (npA->ID != npB->ID)
    {
        nodeA = npA;
        nodeB = npB;
        ID = instantionID;
        instantionID++;
        setFlag(ItemIsSelectable);
        qDebug() << "Edge Constructor ID: " << ID << "From: " << nodeA->ID << " - " << nodeB->ID;
    }
}

Edge::~Edge()
{
    qDebug() << "Edge Destructor ID: " << ID;
    //QGraphicsItem::~QGraphicsItem();
}

QJsonObject Edge::returnJsonObj()
{
        QJsonObject obj;
        obj.insert(QString("ID"), QJsonValue(ID));
        obj.insert(QString("nodeA-ID"), QJsonValue(nodeA->ID));
        obj.insert(QString("nodeB-ID"), QJsonValue(nodeB->ID));
        return obj;
}

QRectF Edge::boundingRect() const
{
    return QRectF(nodeA->scenePos(), nodeB->scenePos()).normalized();
}

void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen pen(Qt::black);
    if (QGraphicsItem::isSelected())
    {
        pen.setColor(QColor(255, 173, 155));
    }
    pen.setWidth(3);
    painter->setPen(pen);
    painter->drawLine(nodeA->scenePos(),nodeB->scenePos());
}

int Edge::type() const
{
    // Enable the use of qgraphicsitem_cast with this item.
    return Type;
}
