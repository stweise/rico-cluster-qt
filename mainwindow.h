// SPDX-License-Identifier: BSD-3-Clause
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QDebug>
#include <QMainWindow>

#include "graphicsscene.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class GraphicsScene;
class MainWindow;
}  // namespace Ui
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
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

  void on_actionHow_to_use_triggered();

  void on_actionAbout_triggered();

 private:
  Ui::MainWindow *ui;
  GraphicsScene *scene;
};

#endif  // MAINWINDOW_H
