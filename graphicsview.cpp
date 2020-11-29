#include "graphicsview.h"

#include <QtDebug>

GraphicsView::GraphicsView(QWidget *parent) : QGraphicsView::QGraphicsView(parent)
{
    //qDebug() << "Hello from GraphicsView";
}
void GraphicsView::keyPressEvent(QKeyEvent *event)
{
    switch(event->key()) {
    case Qt::Key_Space: {
        if (!event->isAutoRepeat())
        {
            if (this->dragMode() == QGraphicsView::NoDrag)
            {
                //qDebug() << "Entering drag mode";
                this->setDragMode(QGraphicsView::ScrollHandDrag);
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

void GraphicsView::keyReleaseEvent(QKeyEvent *event)
{
    switch(event->key()) {
    case Qt::Key_Space: {
        if (!event->isAutoRepeat())
        {
            if (this->dragMode() == QGraphicsView::ScrollHandDrag)
            {
                this->setDragMode(QGraphicsView::NoDrag);
                //qDebug() << "Leaving drag mode";
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
