// SPDX-License-Identifier: BSD-3-Clause
#include <QCoreApplication>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTextStream>
#include <QtDebug>

void read(const QJsonObject &json);
void write(QJsonObject &json);
QString name;
int level = 0;
QVector<int> numbers;
int main(int argc, char *argv[]) {
  QCoreApplication app(argc, argv);
  QStringList args = QCoreApplication::arguments();

  QFile loadFile(QStringLiteral("save.json"));
  if (!loadFile.open(QIODevice::ReadOnly)) {
    qWarning("Couldn't open save file.");
    return -1;
  }

  QByteArray saveData = loadFile.readAll();

  QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));

  read(loadDoc.object());

  QFile saveFile(QStringLiteral("save2.json"));
  if (!saveFile.open(QIODevice::WriteOnly)) {
    qWarning("Couldn't open save2 file.");
    return -1;
  }
  QJsonObject gameObject;
  write(gameObject);
  saveFile.write(QJsonDocument(gameObject).toJson());

  return 0;
}
void read(const QJsonObject &json) {
  if (json.contains("name") && json["name"].isString())
    name = json["name"].toString();

  if (json.contains("level") && json["level"].isDouble())
    level = json["level"].toInt();
  if (json.contains("array") && json["array"].isArray()) {
    QJsonArray array = json["array"].toArray();
    numbers.clear();
    numbers.reserve(array.size());
    for (int i = 0; i < array.size(); ++i) {
      qDebug() << array[i];
      QJsonValue v = array[i];
      int number = v.toInt();
      numbers.append(number);
    }
  }
  qDebug() << name;
  qDebug() << level;
  qDebug() << numbers;
}
void write(QJsonObject &json) {
  json["name"] = name;
  json["level"] = level;
}
