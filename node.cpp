// SPDX-License-Identifier: BSD-3-Clause
#include "node.h"

#include <QApplication>
#include <QFontMetrics>
#include <QJsonObject>
#include <QString>

static int instantiationCounter = 0;
int nodeDrawingStyle = 2;  // 1 - ellipse, 2 - rectangle

Node::Node(QPointF center, QString label)
    : QGraphicsItem::QGraphicsItem(nullptr) {
  QGraphicsItem::setPos(center);
  setFlag(ItemIsSelectable);
  setFlag(ItemIsMovable);
  nodelabel = label;
  this->setZValue(2);
  ID = instantiationCounter;
  qDebug() << "Node Constructor ID: " << ID;
  instantiationCounter++;
}

Node::Node(QJsonObject json) : QGraphicsItem::QGraphicsItem(nullptr) {
  setFlag(ItemIsSelectable);
  setFlag(ItemIsMovable);
  QString nodelabelname = "nodelabel";
  if (json.contains(nodelabelname) && json[nodelabelname].isString()) {
    nodelabel = json[nodelabelname].toString();
  }
  QString nodecentername = "nodecenter";
  if (json.contains(nodecentername) && json[nodecentername].isObject()) {
    QJsonObject jsoncenter = json[nodecentername].toObject();
    QPoint center =
        QPoint(jsoncenter["x"].toDouble(), jsoncenter["y"].toDouble());
    QGraphicsItem::setPos(center);
  }
  QString IDname = "ID";
  if (json.contains(IDname) && json[IDname].isDouble()) {
    ID = json[IDname].toInt();
    // ensure that instantiationCounter is at least as high as highest ID
    // already in saved file
    if (instantiationCounter < ID) {
      instantiationCounter = ID;
    }
  }
  this->setZValue(2);
  qDebug() << "Node Constructor ID: " << ID;
}

Node::~Node() {
  qDebug() << "Node Destructor ID: " << ID;
  // QGraphicsItem::~QGraphicsItem();
}
QJsonObject Node::returnJsonObj() {
  QJsonObject obj;
  obj.insert(QString("ID"), QJsonValue(ID));
  obj.insert(QString("nodelabel"), QJsonValue(nodelabel));
  QJsonObject center;
  center.insert(QString("x"), QJsonValue(scenePos().x()));
  center.insert(QString("y"), QJsonValue(scenePos().y()));
  obj.insert(QString("nodecenter"), QJsonValue(center));
  return obj;
}

QRectF Node::boundingRect() const {
  // default fontsize for drawing is determined, then we use a bounding box of
  // the text drawn with an increased font size to determine a bounding box that
  // will fit our content
  int increasedFontSize = QApplication::font().pointSize();
  if (nodeDrawingStyle == 1)  // ellipse
  {
    increasedFontSize += 12;
  } else if (nodeDrawingStyle == 2)  // rect
  {
    increasedFontSize += 4;
  }
  QFont font("Helvetica", increasedFontSize);
  QFontMetrics fm(font);
  QRectF rect = fm.boundingRect(QRect(), Qt::AlignLeft, nodelabel, 0, nullptr);
  // 0,0 is the center of our node
  rect.moveCenter(QPointF(0, 0));
  return rect.normalized();
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                 QWidget *widget) {
  QRectF rect = boundingRect();
  QPen pen(Qt::black);
  pen.setWidth(3);
  painter->setPen(pen);
  // painter->setFont(QFont("Helvetica",10));
  QBrush brush(QColor(206, 206, 206));
  if (QGraphicsItem::isSelected()) {
    brush.setColor(QColor(255, 173, 155));
  }
  painter->setBrush(brush);
  if (nodeDrawingStyle == 1) {
    painter->drawEllipse(rect);
  } else if (nodeDrawingStyle == 2) {
    pen.setWidth(2);
    painter->drawRoundedRect(rect, 5, 5);
  }
  painter->drawText(rect, Qt::AlignCenter, nodelabel);
  // qDebug() << painter->fontInfo().pointSize();
}

int Node::type() const {
  // Enable the use of qgraphicsitem_cast with this item.
  return Type;
}
