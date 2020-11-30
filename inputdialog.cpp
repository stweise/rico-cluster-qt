#include "inputdialog.h"

#include <QDialog>
#include <QtDebug>
#include <QKeyEvent>
#include <QVBoxLayout>
#include <QPlainTextEdit>
#include <QKeyEvent>
#include <QDialogButtonBox>
InputDialog::InputDialog(QWidget *parent, QString dialogLabel, QString defaultText) : QDialog::QDialog(parent)
{
       setWindowTitle(dialogLabel);
       label = new QLabel(tr(dialogLabel.append(":\n(Ctrl+Enter in text editor for \"Ok\")").toStdString().c_str() ));
       lineEdit = new QPlainTextEdit;
       buttons = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel);
       label->setBuddy(lineEdit);
       QVBoxLayout *topLeftLayout = new QVBoxLayout;
       topLeftLayout->addWidget(label);
       topLeftLayout->addWidget(lineEdit);
       topLeftLayout->addWidget(buttons);
       setLayout(topLeftLayout);
       text = defaultText;
       lineEdit->setPlainText(text);
       connect(buttons, SIGNAL(accepted()), this, SLOT(acceptAndStore()));
       connect(buttons, SIGNAL(rejected()), this, SLOT(reject()));
}
InputDialog::~InputDialog()
{
       delete label;
       delete lineEdit;
       delete buttons;
}

void InputDialog::keyPressEvent(QKeyEvent *e)
{
    if ((e->key()==Qt::Key_Enter || e->key()==Qt::Key_Return) && e->modifiers() == Qt::ControlModifier)
    {
        text = lineEdit->toPlainText();
        accept();
    }
    QDialog::keyPressEvent(e);
}

QString InputDialog::getText()
{
    return text;
}

void InputDialog::acceptAndStore()
{
    text=lineEdit->toPlainText();
    accept();
}
