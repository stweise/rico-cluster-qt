#ifndef INPUTDIALOG_H
#define INPUTDIALOG_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QLabel>
#include <QPlainTextEdit>
#include <QWidget>

class InputDialog : public QDialog {
  Q_OBJECT
 public:
  explicit InputDialog(QWidget *parent = nullptr, QString dialogLabel = "",
                       QString defaultText = "");
  ~InputDialog();
  void keyPressEvent(QKeyEvent *e);
  QString getText();

 signals:

 private:
  QLabel *label;
  QPlainTextEdit *lineEdit;
  QDialogButtonBox *buttons;
  QString text;

 private slots:
  void acceptAndStore();
};

#endif  // INPUTDIALOG_H
