#pragma once

#include <qwidget.h>
#include <qplaintextedit.h>
#include <qpushbutton.h>

class TextSerializerWidget : public QWidget
{
    Q_OBJECT

public:
    TextSerializerWidget(QWidget *parent = Q_NULLPTR);
    void setActive(bool value);

private:
    QPlainTextEdit *textInput;
    QPushButton *buttonSend;
    bool isActive;

private slots:
    void sendButtonClicked();

signals:
    void dataSerialized(QByteArray data);
};

