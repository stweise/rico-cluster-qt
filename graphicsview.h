#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QAction>
#ifndef NDEBUG
#include <QDebug>
#endif
#include <QGraphicsView>
#include <QKeyEvent>
#include <QKeySequence>
#include <QObject>

class GraphicsView : public QGraphicsView {
  Q_OBJECT
 public:
  explicit GraphicsView(QWidget *parent = nullptr);

  void keyPressEvent(QKeyEvent *event);
  void keyReleaseEvent(QKeyEvent *event);
  void wheelEvent(QWheelEvent *event);

 public slots:
  void zoomIn();
  void zoomOut();

 signals:
};

#endif  // GRAPHICSVIEW_H
