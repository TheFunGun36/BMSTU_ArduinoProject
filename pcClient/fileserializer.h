#pragma once
#include <qwidget.h>
#include <qplaintextedit.h>
#include <qpushbutton.h>
#include <qfiledialog.h>
#include <qlabel.h>
#include <qstring.h>

class FileSerializerWidget : public QWidget
{
    Q_OBJECT

public:
    FileSerializerWidget(QWidget* parent = Q_NULLPTR);

private:
    QLabel *label;
    QPushButton *buttonSend;
    QPushButton *buttonChoice;
    QFileDialog *fileDialog;
    QString filePath;

public slots:
    void buttonSetEnabled();
    void buttonSetDisabled();

private slots:
    void sendButtonClicked();
    void choiceButtonClicked();

signals:
    void dataSerialized(QByteArray data);
};

