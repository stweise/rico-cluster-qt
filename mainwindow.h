// SPDX-License-Identifier: BSD-3-Clause
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include "graphicsscene.h"

QT_BEGIN_NAMESPACE
namespace Ui { class GraphicsScene; class MainWindow; }
QT_END_NAMESPACE



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionSave_triggered();

    void on_actionLoad_triggered();

    void on_actionPrint_triggered();

    void on_actionSave_as_triggered();

    void on_actionExport_triggered();

private:
    Ui::MainWindow *ui;
    GraphicsScene *scene;
};


#endif // MAINWINDOW_H
