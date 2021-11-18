#include "graphicsview.h"

#include <QtDebug>

#define ZOOMINFACTOR 1.1
#define ZOOMOUTFACTOR 0.90909

GraphicsView::GraphicsView(QWidget *parent)
    : QGraphicsView::QGraphicsView(parent) {
  // qDebug() << "Hello from GraphicsView";
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
          setDragMode(QGraphicsView::ScrollHandDrag);
          setInteractive(false);
        }
        break;
      }
    }
    case Qt::Key_Plus: {
      if (event->modifiers() == Qt::ControlModifier) {
        scale(ZOOMINFACTOR, ZOOMINFACTOR);
        break;
      }
    }
    case Qt::Key_Minus: {
      if (event->modifiers() == Qt::ControlModifier) {
        scale(ZOOMOUTFACTOR, ZOOMOUTFACTOR);
        break;
      }
    }
    default: {
      QGraphicsView::keyPressEvent(event);
      break;
    }
  }
}

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
