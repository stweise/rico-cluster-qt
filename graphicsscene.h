// SPDX-License-Identifier: BSD-3-Clause
#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QBrush>
#include <QPen>
#include <QPointF>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include "edge.h"
#include "node.h"
#include <vector>

#ifndef FILEDESC
#define FILEDESC "rico-cluster-qt cluster (*.json)"
#endif

class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit GraphicsScene(QObject *parent=0);
    ~GraphicsScene();
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * mouseEvent);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * mouseEvent);
    virtual void keyPressEvent(QKeyEvent *keyEvent);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent);
    void save();
    void saveAs();
    void load();
    void print();
    void writeJsonFromScene(QJsonObject &json);
    void writeDotFromScene();
private:
    std::vector<Edge*> edges;
    std::vector<Node*> nodes;
    QString saveFileName;
};

#endif // GRAPHICSSCENE_H
