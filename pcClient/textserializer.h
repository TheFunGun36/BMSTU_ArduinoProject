#pragma once
#include <qwidget.h>
#include <qplaintextedit.h>
#include <qpushbutton.h>

class TextSerializerWidget : public QWidget
{
    Q_OBJECT

public:
    TextSerializerWidget(QWidget *parent = Q_NULLPTR);

private:
    QPlainTextEdit *textInput;
    QPushButton *buttonSend;

private slots:
    void sendButtonClicked();

signals:
    void dataSerialized(QByteArray data);
};

