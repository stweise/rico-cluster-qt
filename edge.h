// SPDX-License-Identifier: BSD-3-Clause
#ifndef EDGE_H
#define EDGE_H
#include <QDebug>
#include <QGraphicsItem>
#include <QPainter>
#include <QPointF>

#include "node.h"

enum directedSelection { undirected, AtoB, BtoA };
class Edge : public QGraphicsItem {
 public:
  Edge(Node* pA, Node* pB, directedSelection directed);
  ~Edge();

  virtual QRectF boundingRect() const;
  virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                     QWidget* widget);
  virtual int type() const;
  QJsonObject returnJsonObj();

  int ID;
  Node* nodeA;
  Node* nodeB;
  directedSelection directed;

 private:
  enum { Type = UserType + 2 };
};

#endif  // EDGE_H
