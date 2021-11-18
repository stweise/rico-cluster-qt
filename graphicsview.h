#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>
#include <QKeyEvent>
#include <QObject>

class GraphicsView : public QGraphicsView {
  Q_OBJECT
 public:
  explicit GraphicsView(QWidget *parent = nullptr);

  void keyPressEvent(QKeyEvent *event);
  void keyReleaseEvent(QKeyEvent *event);
  void wheelEvent(QWheelEvent *event);

 signals:
};

#endif  // GRAPHICSVIEW_H
