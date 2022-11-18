// SPDX-License-Identifier: BSD-3-Clause
#include "edge.h"

// use math Windows?
#ifdef _WIN32
#define _USE_MATH_DEFINES
#endif
#include <math.h>
#include "QPaintEngine"

static int instantionID = 0;
Edge::Edge(Node *npA, Node *npB, directedSelection dir) {
  if (npA->ID != npB->ID) {
    nodeA = npA;
    nodeB = npB;
    ID = instantionID;
    instantionID++;
    setFlag(ItemIsSelectable);
#ifndef NDEBUG
    qDebug() << "Edge Constructor ID: " << ID << "From: " << nodeA->ID << " - "
             << nodeB->ID;
#endif
    directed = dir;
  }
}

Edge::~Edge() {
#ifndef NDEBUG
  qDebug() << "Edge Destructor ID: " << ID;
#endif
  // QGraphicsItem::~QGraphicsItem();
}

QJsonObject Edge::returnJsonObj() {
  QJsonObject obj;
  obj.insert(QString("ID"), QJsonValue(ID));
  obj.insert(QString("nodeA-ID"), QJsonValue(nodeA->ID));
  obj.insert(QString("nodeB-ID"), QJsonValue(nodeB->ID));
  obj.insert(QString("directed"), QJsonValue(directed));
  return obj;
}

QRectF Edge::boundingRect() const {
  return QRectF(nodeA->scenePos(), nodeB->scenePos()).normalized();
}

void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                 QWidget *widget) {
  QColor edgecolor = Qt::black;
  if (QPaintEngine::Pdf != painter->paintEngine()->type() ) {
    // make edges recognizable in dark mode, but not when drawing to a pdf
    edgecolor = QPalette().color(QPalette::Text);
  }
  QPen pen(edgecolor);
  if (QGraphicsItem::isSelected()) {
    pen.setColor(QColor(238, 54, 54));  // use red color to signify selection
  }
  pen.setWidth(3);
  painter->setPen(pen);
  painter->drawLine(nodeA->scenePos(), nodeB->scenePos());
  if (directed != undirected) {
    Node *NodeFrom;
    Node *NodeTo;
    if (directed == AtoB)  // from A to B
    {
      NodeFrom = nodeA;
      NodeTo = nodeB;
    } else  // from B to A
    {
      NodeFrom = nodeB;
      NodeTo = nodeA;
    }
    QPainterPath nodePath = NodeTo->mapToScene(NodeTo->shape());
    QPainterPath linePath;
    linePath.moveTo(NodeFrom->scenePos());
    linePath.lineTo(NodeTo->scenePos());
    QPainterPath in = linePath.intersected(nodePath);
    // Take the first element of the intersection and take these "moveTo"
    // coordinates to start drawing
    qreal x = 0.0;
    qreal y = 0.0;
    qreal x1 = NodeTo->scenePos().x();
    qreal y1 = NodeTo->scenePos().y();
    qreal x2 = NodeFrom->scenePos().x();
    qreal y2 = NodeFrom->scenePos().y();
    // qDebug() << NodeTo->scenePos() << NodeFrom->scenePos();
    if (in.elementCount() > 0) {
      // qDebug() << "Coord: " << in.elementAt(0).x << in.elementAt(0).y;
      x = in.elementAt(0).x;
      y = in.elementAt(0).y;
    } else  // usually happens when delta-x or delta-y are zero
    {
      if (fabs(x2 - x1) < 0.0000001)  // vertical
      {
        qreal distance = NodeTo->boundingRect().height() / 2.0;  // vertical
        if (y2 < y1) {
          y = y1 - distance;
        } else {
          y = y1 + distance;
        }
        x = x1;
      } else if (fabs(y2 - y1) < 0.0000001)  // horizontal
      {
        qreal distance = NodeTo->boundingRect().width() / 2.0;
        if (x2 < x1) {
          x = x1 - distance;
        } else {
          x = x1 + distance;
        }
        y = y1;
      } else  // this should never happen, but let us see
      {
        x = 0;
        y = 0;
      }
    }
    // draw polygon for triangle
    QPolygonF poly;
    poly << QPointF(0, 0) << QPointF(20.0, 7.0) << QPointF(20.0, -7.0);
    QTransform t;
    t.translate(x, y);
    qreal angle = atan2(y2 - y1, x2 - x1);
    angle *= 180.0 / M_PI;
    // qDebug() << "Angle: " << angle;
    // qDebug() << "x:" << x << "y:" << y;
    t.rotate(angle);
    QPolygonF poly2 = t.map(poly);
    pen.setWidth(1);
    painter->setPen(pen);
    painter->setBrush(edgecolor);
    painter->drawPolygon(poly2, Qt::WindingFill);
  }
}

int Edge::type() const {
  // Enable the use of qgraphicsitem_cast with this item.
  return Type;
}
