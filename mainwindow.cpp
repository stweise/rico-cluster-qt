// SPDX-License-Identifier: BSD-3-Clause
#include "mainwindow.h"

#include "./ui_mainwindow.h"
#include "graphicsscene.h"
#include <QTextBrowser>

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

void MainWindow::on_actionHow_to_use_triggered() {
  QString helpmessage = "<h2>Help</h2><h3>Edit</h3><table border=1>"
        "<tr><th>Action</th><th>Effect</th></tr>"
        " <tr><td>double-click on empty space</td><td>create a new node</td></tr>"
        " <tr><td>double-click on a node</td><td>update node label (old text is default for new label)</td></tr>"
        " <tr><td>single-click on a node</td><td>selects node</td></tr>"
        " <tr><td>(Windows & Linux) CTRL+mouse left click on a node or edge</td><td>toggles selection on node or edge</td></tr>"
        " <tr><td>(Mac) CMD+mouse left click on a node or edge</td><td>toggles selection on node or edge</td></tr>"
        " <tr><td>E-key + two selected nodes</td><td>nodes are connected by an edge</td></tr>"
        " <tr><td>DELETE/BACKSPACE-key</td><td>deletes edge/node under current selection</td></tr>"
        " <tr><td>select and drag for nodes is supported (extendable by selection)</td><td></td></tr>"
        " <tr><td>A-key while an edge is selected</td><td>adds an arrow, repeated pressing of A cycles through arrow options (A->B, A<-B, A-B)</td></tr>"
        " <tr><td>0 1 ... 5-key with node(s) selected</td><td>nodes are assigned into autocolored categories (0 - is default category)</td></tr>"
        "</table>"
        "<h3>View/Zoom</h3><table border=1>"
        "<tr><th>Action</th><th>Effect</th></tr>"
        "<tr><td>SPACE + mouse drag</td><td>pan on whiteboard (does not pan into empty space yet)</td></tr>"
        "<tr><td>CTRL + +/--keys</td><td>zooms in/out on graph</td></tr>"
        "<tr><td>CTRL + mouse scroll wheel</td><td>zooms in/out on graph</td></tr>"
        "<tr><td>CTRL + 0-key</td><td>fits everything on whiteboard into window</td></tr>"
        "<tr><td>objects are moved towards the edge</td><td>drawing area (canvas) is extending</td></tr>"
        "</table>";
  QDialog *helpdialog = new QDialog(this);
  helpdialog->setSizeGripEnabled(true);
  QTextBrowser *tb = new QTextBrowser(helpdialog);
  tb->setOpenExternalLinks(true);
  tb->setHtml(helpmessage);
  tb->resize(600,500);
  tb->show();
  helpdialog->adjustSize();
  helpdialog->show();
}

void MainWindow::on_actionAbout_triggered() {
  // This dialog is automatically moved to that "About application" section in
  // osx menu bar
  QString aboutmessage = "rico-cluster-qt version 1.6<br><p></p>"
                  "for more details please see:<br><a href='https://github.com/stweise/rico-cluster-qt'>https://github.com/stweise/rico-cluster-qt</a>";
  QMessageBox::about(this, "About", aboutmessage);
}
