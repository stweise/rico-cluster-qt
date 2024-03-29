// SPDX-License-Identifier: BSD-3-Clause
#include "graphicsscene.h"

enum CustomItemType { ITEMNODE = 65537, ITEMEDGE = 65538 };

GraphicsScene::GraphicsScene(QObject *parent)
    : QGraphicsScene::QGraphicsScene(parent) {
  // qDebug() << "FOo";
  setItemIndexMethod(QGraphicsScene::ItemIndexMethod::NoIndex);
}
GraphicsScene::~GraphicsScene() {}

void GraphicsScene::save() {
  if (saveFileName.isEmpty()) {
    saveAs();
  } else {
    QFile saveFile(saveFileName);
    if (!saveFile.open(QIODevice::WriteOnly)) {
      qWarning("Couldn't open save file.");
    }
    QJsonObject sceneObject;
    writeJsonFromScene(sceneObject);
    saveFile.write(QJsonDocument(sceneObject).toJson());
    saveFile.close();
  }
}

void GraphicsScene::saveAs() {
  QString filter = SAVEFILEDESC;
  QDir examplePath = QDir::home();
  QString absolutePath = examplePath.absoluteFilePath("save.json");
  QString savefilename = QFileDialog::getSaveFileName(
      nullptr, tr("Save File"), absolutePath, filter, &filter);
  if (!savefilename.isEmpty()) {
    QFile saveFile(savefilename);
    if (!saveFile.open(QIODevice::WriteOnly)) {
      qWarning("Couldn't open save file.");
    }
    QJsonObject sceneObject;
    writeJsonFromScene(sceneObject);
    saveFile.write(QJsonDocument(sceneObject).toJson());
    saveFile.close();
    saveFileName = savefilename;
  }
}

void GraphicsScene::load() {
  /* ensures that this is only run, when no nodes are defined and no edges are
   * defined */
  /* this means the current cluster is empty */
  if (edges.size() == 0 && nodes.size() == 0) {
    QString loadfilename = QFileDialog::getOpenFileName(
        nullptr, tr("Open File"), QDir::homePath(), tr(SAVEFILEDESC));
    // only do something when a valid file name was returned
    if (!loadfilename.isEmpty()) {
      QFile loadFile(loadfilename);
      if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
      }
      QByteArray loadData = loadFile.readAll();
      QJsonDocument doc = QJsonDocument::fromJson(loadData);
      QJsonObject json = doc.object();
      int version = 0;
      if (json.contains("version") && json["version"].isDouble())
        version = json["version"].toInt();
      // qDebug() << "Version:" << version;

      // Load Nodes
      QString nodesarrayname = "nodes";
      if (json.contains(nodesarrayname) && json[nodesarrayname].isArray()) {
        QJsonArray nodearray = json[nodesarrayname].toArray();
        for (int i = 0; i < nodearray.size(); ++i) {
          QJsonObject jsonnode = nodearray[i].toObject();
          Node *node = nullptr;
          if (version < 3) {
            QJsonObject local = jsonnode;
            local.insert(QString("category"), QJsonValue(0));
            node = new Node(local);
          }
          if (version >= 3) {
            node = new Node(nodearray[i].toObject());
          }
          // qDebug() << node;
          addItem(node);
          nodes.push_back(node);
        }
      }
      // Load Edges
      QString edgesarrayname = "edges";
      if (json.contains(edgesarrayname) && json[edgesarrayname].isArray()) {
        QJsonArray edgearray = json[edgesarrayname].toArray();
        for (int i = 0; i < edgearray.size(); ++i) {
          QJsonObject jsonedge = edgearray[i].toObject();
          QString edgeIDname = "ID";
          int ID;
          if (jsonedge.contains(edgeIDname) &&
              jsonedge[edgeIDname].isDouble())  // needs an "ID" field
          {
            ID = jsonedge[edgeIDname].toInt();
            QString nodeAIDname = "nodeA-ID";
            int nodeAID;
            if (jsonedge.contains(nodeAIDname) &&
                jsonedge[nodeAIDname].isDouble())  // needs an "nodeA-ID" field
            {
              nodeAID = jsonedge[nodeAIDname].toInt();
              QString nodeBIDname = "nodeB-ID";
              int nodeBID;
              if (jsonedge.contains(nodeBIDname) &&
                  jsonedge[nodeBIDname]
                      .isDouble())  // needs an "nodeB-ID" field
              {
                nodeBID = jsonedge[nodeBIDname].toInt();
                directedSelection dirSel =
                    undirected;  // use this as a default if the property is not
                                 // available (aka version 1)
                QString directedName = "directed";
                if (version >= 2 && jsonedge.contains(directedName) &&
                    jsonedge[directedName].isDouble()) {
                  dirSel = (directedSelection)jsonedge[directedName].toInt();
                }
                // Find nodes by ID from vector of nodes
                std::vector<Node *>::iterator nit = nodes.begin();
                Node *nodeA = nullptr;
                Node *nodeB = nullptr;
                while (nit != nodes.end()) {
                  if ((*nit)->ID == nodeAID) {
                    nodeA = (*nit);
                  }
                  if ((*nit)->ID == nodeBID) {
                    nodeB = (*nit);
                  }
                  ++nit;
                }
                if (nodeA != nullptr && nodeB != nullptr) {
                  Edge *edge = new Edge(nodeA, nodeB, dirSel);
                  edge->ID = ID;
                  addItem(edge);
                  edges.push_back(edge);
                } else {
                  qFatal(
                      "\n Error \n Error:Node numbering inconsistency in array "
                      "of edges.\n");
                }
              }
            }
          }
        }
      }
      loadFile.close();
      saveFileName = loadfilename;
    }
  } else {
    QMessageBox msgBox;
    msgBox.setText(
        "There are already nodes and/or edges present. Refusing to overwrite "
        "your work.");
    msgBox.exec();
  }
}

void GraphicsScene::writeJsonFromScene(QJsonObject &json) {
  json["version"] = 3;
  // create array of all the nodes
  QJsonArray nodearray;
  std::vector<Node *>::iterator nit = nodes.begin();
  while (nit != nodes.end()) {
    // get node json representation from each node
    nodearray.append((*nit)->returnJsonObj());
    ++nit;
  }
  // add node array to original QJsonObject
  json.insert(QString("nodes"), nodearray);

  // create array of all the edges
  QJsonArray edgearray;
  std::vector<Edge *>::iterator eit = edges.begin();
  while (eit != edges.end()) {
    // get node json representation from each node
    edgearray.append((*eit)->returnJsonObj());
    ++eit;
  }
  // add node array to original QJsonObject
  json.insert(QString("edges"), edgearray);
}

void GraphicsScene::exportToDot() {
  QString fileTypeFilter = EXPORTDOTFILEDESC;
  QDir homePath = QDir::home();
  QString absoluteHomePath = homePath.absoluteFilePath("export.dot");
  QString exportFileName =
      QFileDialog::getSaveFileName(nullptr, tr("Export File"), absoluteHomePath,
                                   fileTypeFilter, &fileTypeFilter);
  if (!exportFileName.isEmpty()) {
    QFile exportFile(exportFileName);
    QString graphName = QFileInfo(exportFile).baseName();
    if (exportFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
      QTextStream stream(&exportFile);
      stream << "digraph \"" << graphName << "\" {\n";
      std::vector<Node *>::iterator nit = nodes.begin();
      while (nit != nodes.end()) {
        QString cleanedLabel = (*nit)->nodelabel;
        cleanedLabel.replace("\n", "\\n");
        stream << "node" << (*nit)->ID << " [label=\"" << cleanedLabel << "\""
               << ", style=filled, fillcolor=\"" << (*nit)->getHexFillColor()
               << "\""
               << "];\n";
        ++nit;
      }

      std::vector<Edge *>::iterator eit = edges.begin();
      while (eit != edges.end()) {
        if ((*eit)->directed == undirected || (*eit)->directed == AtoB) {
          stream << "node" << (*eit)->nodeA->ID << " -> "
                 << "node" << (*eit)->nodeB->ID;
          if ((*eit)->directed == undirected) {
            stream << " [dir=none]";
          }
          stream << ";\n";
        } else {
          stream << "node" << (*eit)->nodeB->ID << " -> "
                 << "node" << (*eit)->nodeA->ID << ";\n";
        }
        ++eit;
      }
      stream << "}" << '\n';

      exportFile.close();
    }
  }
}

void GraphicsScene::mouseDoubleClickEvent(
    QGraphicsSceneMouseEvent *mouseEvent) {
  //   qDebug() << Q_FUNC_INFO << mouseEvent->scenePos();
  QGraphicsItem *undercursor = itemAt(mouseEvent->scenePos(), QTransform());
  bool ok;
  // check if we are called to modify a node
  if (undercursor != nullptr) {
    int itemtype = undercursor->type();
    // If it is a node
    if (itemtype == ITEMNODE) {
      Node *node = (Node *)undercursor;
      InputDialog d(nullptr, "Modify node label", node->nodelabel);
      ok = !!d.exec();
      QString text = d.getText();
      if (ok && !text.isEmpty()) {
        node->nodelabel = text;
        update();
      }
    }
  } else {
    InputDialog d(nullptr, "Create node label", "");
    ok = !!d.exec();
    QString text = d.getText();
    if (ok && !text.isEmpty()) {
      Node *node = new Node(mouseEvent->scenePos(), text, 0);
      addItem(node);
      nodes.push_back(node);
    }
  }
}

void GraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) {
  //    qDebug() << Q_FUNC_INFO << mouseEvent->scenePos();
  QGraphicsScene::mouseMoveEvent(mouseEvent);
  update();
}

void GraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) {
  // qDebug() << Q_FUNC_INFO << mouseEvent->scenePos();
  // qDebug() << mouseEvent->button();
  QGraphicsScene::mouseReleaseEvent(mouseEvent);
  update();
}

void GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) {
  // qDebug() << mouseEvent->button();
  // qDebug() << mouseEvent->modifiers();
  // qDebug() << mouseEvent->flags();
  QGraphicsScene::mousePressEvent(mouseEvent);
}

void GraphicsScene::keyPressEvent(QKeyEvent *keyEvent) {
  // qDebug() << keyEvent->key();
  QList<QGraphicsItem *> selList;
  // make a copy of the list
  selList.append(selectedItems());
  int key = keyEvent->key();
  if (key == Qt::Key_Delete || key == Qt::Key_Backspace) {
    if (selList.size() != 0) {
      for (int i = 0; i < selList.size(); i++) {
#ifndef NDEBUG
        qDebug() << "i: " << i << " size: " << selList.size();
#endif
        QGraphicsItem *item = selList[i];
        int itemtype = item->type();
        // If it is a node
        if (itemtype == ITEMNODE) {
          // Look for all edges pointing to this node
          // Delete all associated edges
          int nodeID = ((Node *)item)->ID;
          std::vector<Edge *>::iterator it = edges.begin();
          while (it != edges.end()) {
            if (nodeID == (*it)->nodeA->ID || nodeID == (*it)->nodeB->ID) {
              Edge *edge = (*it);
              // erase() invalidates it, using returned safe it
              it = edges.erase(it);
              delete edge;
            } else {
              ++it;
            }
          }
          // Remove node from list of nodes
          std::vector<Node *>::iterator nit = nodes.begin();
          while (nit != nodes.end()) {
            if (nodeID == (*nit)->ID) {
              Node *node = (*nit);
              // erase() invalidates it, using returned safe it
              nit = nodes.erase(nit);
              delete node;
            } else {
              ++nit;
            }
          }
        }
        // If it is an edge
        else if (itemtype == ITEMEDGE) {
          // lookup where it is in vector
          std::vector<Edge *>::iterator it;
          for (it = edges.begin(); it != edges.end(); it++) {
            if (((Edge *)item)->ID == (*it)->ID) {
              edges.erase(it);
              delete item;
              break;
            }
          }
        }
      }
      update();
    }
  }
  if (key == Qt::Key_E) {
    if (selList.size() == 2) {
      // both are nodes/ellipsies
      Node *itemA = dynamic_cast<Node *>(selList.first());
      Node *itemB = dynamic_cast<Node *>(selList.last());
      if (itemA->type() == ITEMNODE && itemB->type() == ITEMNODE) {
        Edge *e = new Edge(itemA, itemB, undirected);
        addItem(e);
        edges.push_back(e);
        update();
        clearSelection();
        update();
      }
    }
  }
  if (key == Qt::Key_0 || key == Qt::Key_1 || key == Qt::Key_2 ||
      key == Qt::Key_3 || key == Qt::Key_4 || key == Qt::Key_5) {
    // walk all selection and only process nodes
    if (selList.size() != 0) {
      for (int i = 0; i < selList.size(); i++) {
        Node *item = dynamic_cast<Node *>(selList[i]);
        if (item != nullptr) {
          if (item->type() == ITEMNODE) {
            int c;
            switch (key) {
              case Qt::Key_0:
                c = 0;
                break;
              case Qt::Key_1:
                c = 1;
                break;
              case Qt::Key_2:
                c = 2;
                break;
              case Qt::Key_3:
                c = 3;
                break;
              case Qt::Key_4:
                c = 4;
                break;
              case Qt::Key_5:
                c = 5;
                break;
              default:
                c = 0;
            }
            item->category = c;
          }
        }
      }
      clearSelection();
      update();
    }
  }
  if (key == Qt::Key_A) {
    if (selList.size() == 1) {
      QGraphicsItem *item = selList[0];
      int itemtype = item->type();
      // If it is a edge
      if (itemtype == ITEMEDGE) {
        Edge *e = dynamic_cast<Edge *>(item);
        switch (e->directed) {
          case undirected:
            e->directed = AtoB;
            break;
          case AtoB:
            e->directed = BtoA;
            break;
          case BtoA:
            e->directed = undirected;
            break;
          default:
            e->directed = undirected;
        }
        update();
      }
    }
  }
  QGraphicsScene::keyPressEvent(keyEvent);
}

void GraphicsScene::print() {
  QString filter = "Pdf file (*.pdf)";
  QDir examplePath = QDir::home();
  QString absolutePath = examplePath.absoluteFilePath("print.pdf");
  QString savefilename = QFileDialog::getSaveFileName(
      nullptr, tr("Save File"), absolutePath, filter, &filter);
  if (!savefilename.isEmpty()) {
    QPdfWriter pdfwriter(savefilename);
    pdfwriter.setPageSize(QPageSize(QPageSize::A4));
    pdfwriter.setPageMargins(QMargins(30, 30, 30, 30));
    pdfwriter.setResolution(300);
    pdfwriter.setPageOrientation(QPageLayout::Landscape);
    QPainter painter(&pdfwriter);
    painter.setFont(QFont("Helvetica", 2));
    render(&painter);
    painter.end();
  }
}
