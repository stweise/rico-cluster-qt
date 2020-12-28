// SPDX-License-Identifier: BSD-3-Clause
#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QBrush>
#include <QDebug>
#include <QGraphicsEllipseItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsTextItem>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPen>
#include <QPointF>
#include <vector>

#include "edge.h"
#include "node.h"

#ifndef SAVEFILEDESC
#define SAVEFILEDESC "rico-cluster-qt cluster (*.json)"
#endif
#ifndef EXPORTDOTFILEDESC
#define EXPORTDOTFILEDESC "graphviz dot digraph (*.dot)"
#endif

class GraphicsScene : public QGraphicsScene {
  Q_OBJECT

 public:
  explicit GraphicsScene(QObject *parent = 0);
  ~GraphicsScene();
  virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent);
  virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
  virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
  virtual void keyPressEvent(QKeyEvent *keyEvent);
  virtual void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
  void save();
  void saveAs();
  void load();
  void print();
  void writeJsonFromScene(QJsonObject &json);
  void exportToDot();

 private:
  std::vector<Edge *> edges;
  std::vector<Node *> nodes;
  QString saveFileName;
};

#endif  // GRAPHICSSCENE_H
