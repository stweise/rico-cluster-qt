// SPDX-License-Identifier: BSD-3-Clause
#ifndef NODE_H
#define NODE_H

#include <QApplication>
#ifndef NDEBUG
#include <QDebug>
#endif
#include <QFontMetrics>
#include <QGraphicsItem>
#include <QJsonObject>
#include <QPainter>
#include <QPointF>
#include <QString>

class Node : public QGraphicsItem {
 public:
  Node(QPointF center, QString label, int category);
  Node(QJsonObject json);
  ~Node();

  virtual QRectF boundingRect() const;
  virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                     QWidget* widget);
  virtual int type() const;
  QJsonObject returnJsonObj();
  QString getHexFillColor();

  int ID;
  QString nodelabel;
  int category;

 private:
  enum { Type = UserType + 1 };
};

#endif  // NODE_H
