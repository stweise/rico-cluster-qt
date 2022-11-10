#include "graphicsview.h"

#define ZOOMINFACTOR 1.1
#define ZOOMOUTFACTOR 0.90909

GraphicsView::GraphicsView(QWidget *parent)
    : QGraphicsView::QGraphicsView(parent) {
  // qDebug() << "Hello from GraphicsView";

  // create new action that is wired to trigger zoomIn function, when
  // appropriate shortcut is triggered
  QAction *zoomInAction = new QAction(this);
  zoomInAction->setShortcuts(QKeySequence::ZoomIn);
  connect(zoomInAction, &QAction::triggered, this, &GraphicsView::zoomIn);
  this->addAction(zoomInAction);

  // create new action that is wired to trigger zoomOut function, when
  // appropriate shortcut is triggered
  QAction *zoomOutAction = new QAction(this);
  zoomOutAction->setShortcuts(QKeySequence::ZoomOut);
  connect(zoomOutAction, &QAction::triggered, this, &GraphicsView::zoomOut);
  this->addAction(zoomOutAction);

  // creates new rubberband rectangle to perform node selections
  rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
  rubberBandSelectionStarted = false;
}
void GraphicsView::wheelEvent(QWheelEvent *event) {
  if (event->modifiers() == Qt::ControlModifier) {
    // save off old current transformation anchor
    const ViewportAnchor anchor = transformationAnchor();
    // makes sure to zoom under mouse position
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    int angle = event->angleDelta().y();
    if (angle > 0) {
      zoomIn();
    } else {
      zoomOut();
    }
    // reset transformation anchor to old anchor
    setTransformationAnchor(anchor);
  } else {
    QGraphicsView::wheelEvent(event);
  }
}
void GraphicsView::keyPressEvent(QKeyEvent *event) {
  switch (event->key()) {
    case Qt::Key_Space: {  // trigger dragging mode when pressing & holding
                           // space key
      if (!event->isAutoRepeat()) {
        if (dragMode() == QGraphicsView::NoDrag) {
          // qDebug() << "Entering drag mode";
          setDragMode(QGraphicsView::ScrollHandDrag);
          setInteractive(false);
        }
        break;
      }
    }
    case Qt::Key_0: {  // CMD/Control + 0 resets zoom to fit everything
      if (event->modifiers() == Qt::ControlModifier) {

        // resize sceneRect of scene to fit all items, grows by design under normal use only
        QRectF r = scene()->itemsBoundingRect();
        scene()->setSceneRect(r);

        // fit view to updated sceneRect
        fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
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
