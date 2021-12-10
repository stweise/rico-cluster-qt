// SPDX-License-Identifier: BSD-3-Clause
#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QBrush>
#include <vector>
#ifndef NDEBUG
#include <QDebug>
#endif
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QGraphicsEllipseItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsTextItem>
#include <QInputDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QKeyEvent>
#include <QLineEdit>
#include <QLineF>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPdfWriter>
#include <QPen>
#include <QPointF>
#include <QTextStream>

#include "edge.h"
#include "inputdialog.h"
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
