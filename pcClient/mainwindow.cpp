#include "mainwindow.h"
#include <qgridlayout.h>
#include <qpushbutton.h>
#include <qmessagebox.h>

#include <qdebug.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    comWorker = new COMWorker(this);
    comWorker->openPort("COM7");
    ui.setupUi(this);

    serializers = new QStackedWidget(this);
    textSerializer = new TextSerializerWidget(this);
    textDeserializer = new TextDeserializerWidget(this);

    connect(textSerializer, &TextSerializerWidget::dataSerialized, comWorker, &COMWorker::sendArrayBegin);
    connect(ui.actionSetTextSendMode, &QAction::triggered, this,
        [this]() {
            serializers->setCurrentIndex(static_cast<int>(DisplayWidget::TextSend));
        }
    );
    connect(ui.actionSetTextRecieveMode, &QAction::triggered, this,
        [this]() {
            serializers->setCurrentIndex(static_cast<int>(DisplayWidget::TextRecieve));
        }
    );

    connect(ui.actionShowAboutUsInfo, &QAction::triggered, this, &MainWindow::showInfoMessage);
    connect(ui.actionSwitchColorTheme, &QAction::triggered, this, &MainWindow::switchColorTheme);

    QGridLayout *centralWidgetLayout = new QGridLayout(this);

    // ORDER IS IMPORTANT HERE! Should be same as enum DisplayWidget
    serializers->addWidget(textSerializer);
    serializers->addWidget(textDeserializer);

    centralWidgetLayout->addWidget(serializers);

    ui.centralWidget->setLayout(centralWidgetLayout);

    connect(comWorker, &COMWorker::workError, this, &MainWindow::showErrorMessage);
    connect(comWorker, &COMWorker::newStatusMessage, this, &MainWindow::showStatusbarMessage);

    connect(comWorker, &COMWorker::startArraySending, textSerializer, &TextSerializerWidget::buttonSetDisabled);
    connect(comWorker, &COMWorker::arraySent, textSerializer, &TextSerializerWidget::buttonSetEnabled);

    connect(comWorker, &COMWorker::startArrayReceiving, textSerializer, &TextSerializerWidget::buttonSetDisabled);
    connect(comWorker, &COMWorker::arrayReceived, textSerializer, &TextSerializerWidget::buttonSetEnabled);

    connect(comWorker, &COMWorker::workError, textSerializer, &TextSerializerWidget::buttonSetEnabled);

    connect(comWorker, &COMWorker::arraySent, this, &MainWindow::endSending);
    connect(comWorker, &COMWorker::arrayReceived, this, &MainWindow::endReceiving);
}

void MainWindow::endReceiving(QByteArray msg)
{
    QMessageBox::information(this, "Сообщение получено", "Перейдите в окно для прочтения полученного сообщения");
    serializers->setCurrentIndex(static_cast<int>(DisplayWidget::TextRecieve));
    textDeserializer->showDeserializedData(msg);
}

void MainWindow::endSending()
{
    QMessageBox::information(this, "Выполнено", "Отправка сообщения завершена");
}

void MainWindow::showStatusbarMessage(QString message)
{
    ui.statusBar->showMessage(message);
}

void MainWindow::showInfoMessage()
{
    QMessageBox::information(
        this, "О нас",
        "Данная программа создана в рамках проектной практики в МГТУ им. Баумана студентами 1-го курса:\n"
        "Чепрасов К. М.\n"
        "Солопов Ю. В.\n"
        "Калашков П. А.\n"
        "Скороходова М. К.\n"
        "Комаров Н. С.\n"
    );
}

void MainWindow::showErrorMessage(ErrorCode code)
{
    switch (code)
    {
    case ErrorCode::OpenFailed:
        QMessageBox::information(this, "Ошибка", "Не удалось открыть SerialPort");
        break;
    case ErrorCode::SendFailed:
        QMessageBox::information(this, "Ошибка", "Не удалось отправить сообщение");
        break;
    case ErrorCode::ReceiveFailed:
        QMessageBox::information(this, "Ошибка", "Не удалось принять сообщение");
        break;
    default:
        break;
    }
}

void MainWindow::switchColorTheme()
{
    static bool isDarkTheme = false;

    if (isDarkTheme)
    {
        QFile file;
        file.setFileName(":/MainWindow/stylesheet/lightTheme.css");
        file.open(QFile::ReadOnly);
        this->setStyleSheet(file.readAll());
        file.close();
        isDarkTheme = false;
    }
    else
    {
        QFile file;
        file.setFileName(":/MainWindow/stylesheet/darkTheme.css");
        file.open(QFile::ReadOnly);
        this->setStyleSheet(file.readAll());
        file.close();
        isDarkTheme = true;
    }
}
