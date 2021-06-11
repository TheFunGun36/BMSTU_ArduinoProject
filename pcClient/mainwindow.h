#pragma once

#include <qmainwindow.h>
#include <qtextedit.h>
#include <qstackedwidget.h>
#include "ui_mainwindow.h"
#include "textserializer.h"
#include "textdeserializer.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = Q_NULLPTR);

private:
    enum class DisplayWidget
    {
        TextSend,
        TextRecieve,
        ImageSend,
        ImageRecieve,
        FileSend,
        FileRecieve
    };

    QStackedWidget *serializers;
    TextSerializerWidget *textSerializer;
    TextDeserializerWidget *textDeserializer;
    Ui::MainWindowClass ui;

public slots:
    void sendSerializedData(QByteArray data);
    void showStatusbarMessage(QString message);
    void switchColorTheme();
};
