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
#include <QPainterPath>
#include <QRubberBand>

class GraphicsView : public QGraphicsView {
  Q_OBJECT
 public:
  explicit GraphicsView(QWidget *parent = nullptr);

  void keyPressEvent(QKeyEvent *event);
  void keyReleaseEvent(QKeyEvent *event);
  void wheelEvent(QWheelEvent *event);
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);

 private:
  QRubberBand *rubberBand;
  bool rubberBandSelectionStarted;
  QPoint origin;

 public slots:
  void zoomIn();
  void zoomOut();

 signals:
};

#endif  // GRAPHICSVIEW_H
