#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QObject>
#include <QGraphicsView>
#include <QKeyEvent>

class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit GraphicsView(QWidget *parent = nullptr);

void keyPressEvent(QKeyEvent *event);
void keyReleaseEvent(QKeyEvent *event);

signals:

};

#endif // GRAPHICSVIEW_H
