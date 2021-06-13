#pragma once

#include <qmainwindow.h>
#include <qtextedit.h>
#include <qstackedwidget.h>
#include "ui_mainwindow.h"
#include "textserializer.h"
#include "textdeserializer.h"
#include "fileserializer.h"
#include "filedeserializer.h"
#include "comworker.h"

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
        FileSend,
        FileRecieve
    };

    QStackedWidget *serializers;
    TextSerializerWidget *textSerializer;
    TextDeserializerWidget *textDeserializer;
    FileSerializerWidget *fileSerializer;
    FileDeserializerWidget *fileDeserializer;
    COMWorker *comWorker;
    Ui::MainWindowClass ui;

public slots:
    void switchColorTheme();

private slots:
    void showStatusbarMessage(QString message);
    void showInfoMessage();
    void showErrorMessage(ErrorCode code);
    void endSending();
    void endReceiving(QByteArray msg);
};
