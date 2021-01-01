#include "graphicsview.h"

#include <QtDebug>

GraphicsView::GraphicsView(QWidget *parent)
    : QGraphicsView::QGraphicsView(parent) {
  // qDebug() << "Hello from GraphicsView";
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
