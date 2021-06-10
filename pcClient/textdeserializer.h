#pragma once
#include <qwidget.h>
#include <qplaintextedit.h>

class TextDeserializerWidget : public QWidget
{
public:
    TextDeserializerWidget(QWidget *parent = Q_NULLPTR);

private:
    QPlainTextEdit *textOutput;

public slots:
    void showDeserializedData(QByteArray data);
};

