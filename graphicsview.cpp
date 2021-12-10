#include "graphicsview.h"

#define ZOOMINFACTOR 1.1
#define ZOOMOUTFACTOR 0.90909

GraphicsView::GraphicsView(QWidget *parent)
    : QGraphicsView::QGraphicsView(parent) {
  // qDebug() << "Hello from GraphicsView";
  QAction *zoomInAction = new QAction(this);
  zoomInAction->setShortcuts(QKeySequence::ZoomIn);
  connect(zoomInAction, &QAction::triggered, this, &GraphicsView::zoomIn);
  this->addAction(zoomInAction);
  QAction *zoomOutAction = new QAction(this);
  zoomOutAction->setShortcuts(QKeySequence::ZoomOut);
  connect(zoomOutAction, &QAction::triggered, this, &GraphicsView::zoomOut);
  this->addAction(zoomOutAction);
  rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
  rubberBandSelectionStarted = false;
}
void GraphicsView::wheelEvent(QWheelEvent *event) {
  if (event->modifiers() == Qt::ControlModifier) {
    const ViewportAnchor anchor = transformationAnchor();
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    int angle = event->angleDelta().y();
    qreal factor;
    if (angle > 0) {
      factor = ZOOMINFACTOR;
    } else {
      factor = ZOOMOUTFACTOR;
    }
    scale(factor, factor);
    setTransformationAnchor(anchor);
  } else {
    QGraphicsView::wheelEvent(event);
  }
}
void GraphicsView::keyPressEvent(QKeyEvent *event) {
  switch (event->key()) {
    case Qt::Key_Space: {
      if (!event->isAutoRepeat()) {
        if (dragMode() == QGraphicsView::NoDrag) {
          // qDebug() << "Entering drag mode";
          setDragMode(QGraphicsView::ScrollHandDrag);
          setInteractive(false);
        }
        break;
      }
    }
    default: {
      QGraphicsView::keyPressEvent(event);
      break;
    }
  }
}

void GraphicsView::zoomIn() { scale(ZOOMINFACTOR, ZOOMINFACTOR); }

void GraphicsView::zoomOut() { scale(ZOOMOUTFACTOR, ZOOMOUTFACTOR); }

void GraphicsView::keyReleaseEvent(QKeyEvent *event) {
  switch (event->key()) {
    case Qt::Key_Space: {
      if (!event->isAutoRepeat()) {
        if (dragMode() == QGraphicsView::ScrollHandDrag) {
          setInteractive(true);
          setDragMode(QGraphicsView::NoDrag);
          // qDebug() << "Leaving drag mode";
        }
        break;
      }
    }
    default: {
      break;
      QGraphicsView::keyReleaseEvent(event);
    }
  }
}

void GraphicsView::mousePressEvent(QMouseEvent *event) {
  if (event->modifiers() == Qt::ShiftModifier &&
      event->button() == Qt::LeftButton) {
    rubberBandSelectionStarted = true;
    origin = event->pos();
#ifndef NDEBUG
    qDebug() << "Rubberband start pos:" << origin;
#endif
    rubberBand->setGeometry(QRect(event->pos(), QSize()));
    rubberBand->show();
  }
  QGraphicsView::mousePressEvent(event);
}

void GraphicsView::mouseMoveEvent(QMouseEvent *event) {
  if (rubberBandSelectionStarted) {
    rubberBand->setGeometry(QRect(origin, event->pos()));
  }
  QGraphicsView::mouseMoveEvent(event);
}

void GraphicsView::mouseReleaseEvent(QMouseEvent *event) {
  // not checking if shift is still pressed as user might have released it in
  // the meantime
  if (rubberBandSelectionStarted && event->button() == Qt::LeftButton) {
    rubberBand->hide();
    rubberBandSelectionStarted = false;
    QPainterPath pp;
    pp.addRect(
        QRectF(this->mapToScene(origin), this->mapToScene(event->pos())));
    this->scene()->setSelectionArea(pp);
    this->scene()->update();
  }
  QGraphicsView::mouseReleaseEvent(event);
}
