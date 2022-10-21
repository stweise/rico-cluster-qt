// SPDX-License-Identifier: BSD-3-Clause
#include "mainwindow.h"

#include "./ui_mainwindow.h"
#include "graphicsscene.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  scene = new GraphicsScene(this);
  ui->graphicsView->setScene(scene);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_actionSave_triggered() { scene->save(); }

void MainWindow::on_actionLoad_triggered() { scene->load(); }

void MainWindow::on_actionPrint_triggered() { scene->print(); }

void MainWindow::on_actionSave_as_triggered() { scene->saveAs(); }

void MainWindow::on_actionExport_triggered() { scene->exportToDot(); }



void MainWindow::on_actionHow_to_use_triggered()
{

    // let us do nothing right now;
    qDebug() << "the section how to use it was triggered";
}


void MainWindow::on_actionAbout_triggered()
{
    // This dialog is automatically moved to that "About application" section in osx menu bar
    qDebug() << "this shows the about dialog";
}

