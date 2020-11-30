#ifndef INPUTDIALOG_H
#define INPUTDIALOG_H

#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QPlainTextEdit>
#include <QDialogButtonBox>

class InputDialog : public QDialog
{
    Q_OBJECT
public:
    explicit InputDialog(QWidget *parent = nullptr, QString dialogLabel ="", QString defaultText ="");
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

#endif // INPUTDIALOG_H
