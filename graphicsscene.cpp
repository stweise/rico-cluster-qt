// SPDX-License-Identifier: BSD-3-Clause
#include "graphicsscene.h"
#include "edge.h"
#include "node.h"
#include "inputdialog.h"
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QInputDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLineEdit>
#include <QLineF>
#include <QMessageBox>
#include <QDir>
#include <QtDebug>
#include <QPdfWriter>

enum CustomItemType {ITEMNODE=65537, ITEMEDGE=65538};

GraphicsScene::GraphicsScene(QObject *parent) :QGraphicsScene::QGraphicsScene(parent)
{
    //qDebug() << "FOo";
    this->setItemIndexMethod(QGraphicsScene::ItemIndexMethod::NoIndex);
}
GraphicsScene::~GraphicsScene()
{
}

void GraphicsScene::save()
{
    if(this->saveFileName.isEmpty())
    {
        this->saveAs();
    }
    else
    {
        QFile saveFile(this->saveFileName);
        if (!saveFile.open(QIODevice::WriteOnly)) {
            qWarning("Couldn't open save file.");
        }
        QJsonObject sceneObject;
        writeJsonFromScene(sceneObject);
        saveFile.write(QJsonDocument(sceneObject).toJson());
        saveFile.close();
    }
}

void GraphicsScene::saveAs()
{
    QString filter = SAVEFILEDESC;
    QDir examplePath = QDir::home();
    QString absolutePath = examplePath.absoluteFilePath("save.json");
    QString savefilename = QFileDialog::getSaveFileName(nullptr, tr("Save File"), absolutePath , filter, &filter);
    if (!savefilename.isEmpty())
    {
        QFile saveFile(savefilename);
        if (!saveFile.open(QIODevice::WriteOnly)) {
            qWarning("Couldn't open save file.");
        }
        QJsonObject sceneObject;
        writeJsonFromScene(sceneObject);
        saveFile.write(QJsonDocument(sceneObject).toJson());
        saveFile.close();
        this->saveFileName = savefilename;
    }
}

void GraphicsScene::load()
{
    /* TODO */
    /* ensure that this is only run, when no nodes are defined and no edges are defined*/
    /* this means the current cluster is empty*/
    if ( edges.size()==0 && nodes.size() == 0)
    {
        QString loadfilename = QFileDialog::getOpenFileName(nullptr, tr("Open File"), QDir::homePath(), tr(SAVEFILEDESC));
        //only do something when a valid file name was returned
        if (!loadfilename.isEmpty())
        {
            QFile loadFile(loadfilename);
            if (!loadFile.open(QIODevice::ReadOnly)) {
                qWarning("Couldn't open save file.");
            }
            QByteArray loadData = loadFile.readAll();
            QJsonDocument doc = QJsonDocument::fromJson(loadData);
            QJsonObject json=doc.object();
            int version=0;
            if (json.contains("version") && json["version"].isDouble())
                version = json["version"].toInt();
            //qDebug() << "Version:" << version;

            // Load Nodes
            QString nodesarrayname = "nodes";
            if (json.contains(nodesarrayname) && json[nodesarrayname].isArray()) {
                QJsonArray nodearray = json[nodesarrayname].toArray();
                for (int i = 0; i < nodearray.size(); ++i)
                {
                    Node *node = new Node(nodearray[i].toObject());
                    //qDebug() << node;
                    this->addItem(node);
                    nodes.push_back(node);
                }
            }
            // Load Edges
            QString edgesarrayname = "edges";
            if (json.contains(edgesarrayname) && json[edgesarrayname].isArray()) {
                QJsonArray edgearray = json[edgesarrayname].toArray();
                for (int i = 0; i < edgearray.size(); ++i)
                {
                    QJsonObject jsonedge = edgearray[i].toObject();
                    QString edgeIDname = "ID";
                    int ID;
                    if (jsonedge.contains(edgeIDname) && jsonedge[edgeIDname].isDouble())
                    {
                        ID = jsonedge[edgeIDname].toInt();
                    }
                    QString nodeAIDname = "nodeA-ID";
                    int nodeAID;
                    if (jsonedge.contains(nodeAIDname) && jsonedge[nodeAIDname].isDouble())
                    {
                        nodeAID = jsonedge[nodeAIDname].toInt();
                    }
                    QString nodeBIDname = "nodeB-ID";
                    int nodeBID;
                    if (jsonedge.contains(nodeBIDname) && jsonedge[nodeBIDname].isDouble())
                    {
                        nodeBID = jsonedge[nodeBIDname].toInt();
                    }
                    directedSelection dirSel=undirected; //use this as a default if the property is not available (aka version 1)
                    QString directedName = "directed";
                    if (jsonedge.contains(directedName) && jsonedge[directedName].isDouble())
                    {
                        dirSel = (directedSelection) jsonedge[directedName].toInt();
                    }
                    // Find nodes by ID from vector of nodes
                    std::vector<Node*>::iterator nit = nodes.begin();
                    Node *nodeA;
                    Node *nodeB;
                    while (nit != nodes.end())
                    {
                        if ((*nit)->ID == nodeAID)
                        {
                            nodeA = (*nit);
                        }
                        if ((*nit)->ID == nodeBID)
                        {
                            nodeB = (*nit);
                        }
                        ++nit;
                    }
                    Edge *edge = new Edge(nodeA, nodeB, dirSel);
                    edge->ID = ID;
                    //qDebug() << edge;
                    this->addItem(edge);
                    edges.push_back(edge);
                }
            }
            loadFile.close();
            this->saveFileName = loadfilename;
        }
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("There are already nodes and/or edges present. Refusing to overwrite your work.");
        msgBox.exec();
    }
}

void GraphicsScene::writeJsonFromScene(QJsonObject &json)
{
    json["version"] = 2;
    //create array of all the nodes
    QJsonArray nodearray;
    std::vector<Node*>::iterator nit = nodes.begin();
    while (nit != nodes.end())
    {
        //get node json representation from each node
        nodearray.append((*nit)->returnJsonObj());
        ++nit;
    }
    //add node array to original QJsonObject
    json.insert(QString("nodes"),nodearray);

    //create array of all the edges
    QJsonArray edgearray;
    std::vector<Edge*>::iterator eit = edges.begin();
    while (eit != edges.end())
    {
        //get node json representation from each node
        edgearray.append((*eit)->returnJsonObj());
        ++eit;
    }
    //add node array to original QJsonObject
    json.insert(QString("edges"),edgearray);
}

void GraphicsScene::exportToDot()
{
    QString fileTypeFilter = EXPORTDOTFILEDESC;
    QDir homePath = QDir::home();
    QString absoluteHomePath = homePath.absoluteFilePath("export.dot");
    QString exportFileName = QFileDialog::getSaveFileName(nullptr, tr("Export File"), absoluteHomePath , fileTypeFilter, &fileTypeFilter);
    if (!exportFileName.isEmpty())
    {
        QFile exportFile(exportFileName);
        QString graphName = QFileInfo(exportFile).baseName();
        if(exportFile.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream stream(&exportFile);
            stream << "digraph \"" << graphName << "\" {\n";
            std::vector<Node*>::iterator nit = nodes.begin();
            while (nit != nodes.end())
            {
                QString cleanedLabel = (*nit)->nodelabel;
                cleanedLabel.replace("\n","\\n");
                stream << "node" << (*nit)->ID << " [label=\"" << cleanedLabel << "\"];\n";
                ++nit;
            }

            std::vector<Edge*>::iterator eit = edges.begin();
            while (eit != edges.end())
            {
                if ((*eit)->directed == undirected || (*eit)->directed == AtoB)
                {
                    stream << "node" << (*eit)->nodeA->ID << " -> "<< "node" << (*eit)->nodeB->ID;
                    if ((*eit)->directed == undirected)
                    {
                        stream << " [dir=none]";
                    }
                    stream << ";\n";
                }
                else
                {
                    stream << "node" << (*eit)->nodeB->ID << " -> "<< "node" << (*eit)->nodeA->ID<< ";\n";
                }
                ++eit;
            }
            stream << "}" << '\n';

            exportFile.close();
        }
    }
}


void GraphicsScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
 //   qDebug() << Q_FUNC_INFO << mouseEvent->scenePos();
    QGraphicsItem * undercursor = this->itemAt(mouseEvent->scenePos(),QTransform() );
    bool ok;
    //check if we are called to modify a node
    if (undercursor != nullptr)
    {
        int itemtype = undercursor->type();
        //If it is a node
        if (itemtype==ITEMNODE)
				{
					Node* node = (Node*) undercursor;
					InputDialog d(nullptr,"Modify node label", node->nodelabel);
					ok=!!d.exec();
					QString text=d.getText();
					if (ok && !text.isEmpty())
					{
						node->nodelabel=text;
						update();
					}
				}
    }
    else
    {
        InputDialog d(nullptr,"Create node label", "");
        ok=!!d.exec();
        QString text=d.getText();
        if (ok && !text.isEmpty())
        {
            Node *node = new Node(mouseEvent->scenePos(), text);
            this->addItem(node);
            nodes.push_back(node);
        }
    }
}

void GraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
//    qDebug() << Q_FUNC_INFO << mouseEvent->scenePos();
    QGraphicsScene::mouseMoveEvent(mouseEvent);
    update();
}

void GraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
    //qDebug() << Q_FUNC_INFO << mouseEvent->scenePos();
    //qDebug() << mouseEvent->button();
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
    update();
}

void GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    //qDebug() << mouseEvent->button();
    //qDebug() << mouseEvent->modifiers();
    //qDebug() << mouseEvent->flags();
    QGraphicsScene::mousePressEvent(mouseEvent);
}

void GraphicsScene::keyPressEvent(QKeyEvent *keyEvent)
{
    //qDebug() << keyEvent->key();
    QList<QGraphicsItem*> selList;
    //make a copy of the list
    selList.append(this->selectedItems());
    if (keyEvent->key() == Qt::Key_Delete || keyEvent->key() == Qt::Key_Backspace)
    {
        if ( selList.size() != 0)
        {
            for(int i=0;i<selList.size();i++)
            {
                qDebug()<<"i: "<< i <<" size: "<< selList.size();
                QGraphicsItem * item = selList[i];
                int itemtype = item->type();
                //If it is a node
                if (itemtype==ITEMNODE)
                {
                    //Look for all edges pointing to this node
                    //Delete all associated edges
                    int nodeID = ((Node*) item)->ID;
                    std::vector<Edge*>::iterator it = edges.begin();
                    while (it != edges.end())
                    {
                        if( nodeID  == (*it)->nodeA->ID || nodeID  == (*it)->nodeB->ID )
                        {
                            Edge* edge = (*it);
                            //this->removeItem(edge);
                            // erase() invalidates it, using returned safe it
                            it = edges.erase(it);
                            delete edge;
                        }
                        else
                        {
                            ++it;
                        }
                    }
                    //Remove node from list of nodes
                    std::vector<Node*>::iterator nit = nodes.begin();
                    while (nit != nodes.end())
                    {
                        if( nodeID  == (*nit)->ID )
                        {
                            Node* node = (*nit);
                            //this->removeItem(node);
                            // erase() invalidates it, using returned safe it
                            nit = nodes.erase(nit);
                            delete node;
                        }
                        else
                        {
                            ++nit;
                        }
                    }
                }
                //If it is an edge
                else if (itemtype==ITEMEDGE)
                {
                    //lookup where it is in vector
                    std::vector<Edge*>::iterator it;
                    for(it=edges.begin();it!=edges.end();it++)
                    {
                        if( ((Edge*) item)->ID == (*it)->ID)
                        {
                            edges.erase(it);
                            //this->removeItem(item);
                            delete item;
                            break;
                        }
                    }
                }
            }
        update();
        }
    }
    if (keyEvent->key() == Qt::Key_E)
    {
        if ( selList.size() == 2)
        {
            // both are nodes/ellipsies
            Node *itemA = dynamic_cast<Node*>(selList.first());
            Node *itemB = dynamic_cast<Node*>(selList.last());
            if (itemA->type()==ITEMNODE && itemB->type()==ITEMNODE)
            {
                Edge *e = new Edge(itemA, itemB, undirected);
                this->addItem(e);
                edges.push_back(e);
                update();
                this->clearSelection();
                update();
            }
        }
    }
    if (keyEvent->key() == Qt::Key_A)
    {
        if ( selList.size() == 1)
        {
                QGraphicsItem * item = selList[0];
                int itemtype = item->type();
                //If it is a edge
                if (itemtype==ITEMEDGE)
                {
                    Edge *e = dynamic_cast<Edge*>(item);
                    switch(e->directed)
                    {
                        case undirected: e->directed=AtoB; break;
                        case AtoB: e->directed=BtoA; break;
                        case BtoA: e->directed=undirected; break;
                        default: e->directed=undirected;
                    }

                    update();
                }
        }
    }
    QGraphicsScene::keyPressEvent(keyEvent);
}

void GraphicsScene::print()
{
    QString filter = "Pdf file (*.pdf)";
    QDir examplePath = QDir::home();
    QString absolutePath = examplePath.absoluteFilePath("print.pdf");
    QString savefilename = QFileDialog::getSaveFileName(nullptr, tr("Save File"), absolutePath , filter, &filter);
    if (!savefilename.isEmpty())
    {
        QPdfWriter pdfwriter(savefilename);
        pdfwriter.setPageSize(QPageSize(QPageSize::A4));
        pdfwriter.setPageMargins(QMargins(30, 30, 30, 30));
        pdfwriter.setResolution(300);
        pdfwriter.setPageOrientation(QPageLayout::Landscape);
        QPainter painter(&pdfwriter);
        painter.setFont(QFont("Helvetica", 2));
        this->render(&painter);
        painter.end();
    }
}
