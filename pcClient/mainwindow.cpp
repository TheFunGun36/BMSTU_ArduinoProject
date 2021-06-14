#include "mainwindow.h"
#include <qgridlayout.h>
#include <qpushbutton.h>
#include <qmessagebox.h>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    comWorker = new COMWorker(this);
    ui.setupUi(this);  
    this->setWindowTitle("Дверъ");

    serializers = new QStackedWidget(this);
    textSerializer = new TextSerializerWidget(this);
    textDeserializer = new TextDeserializerWidget(this);
    fileSerializer = new FileSerializerWidget(this);
    fileDeserializer = new FileDeserializerWidget(this);

    connect(textSerializer, &TextSerializerWidget::dataSerialized, comWorker, &COMWorker::sendArrayBegin);
    connect(fileSerializer, &FileSerializerWidget::dataSerialized, comWorker, &COMWorker::sendArrayBegin);

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

    connect(ui.actionSetFileSendMode, &QAction::triggered, this,
        [this]() {
        serializers->setCurrentIndex(static_cast<int>(DisplayWidget::FileSend));
        }
    );

    connect(ui.actionSetFileRecieveMode, &QAction::triggered, this,
        [this]() {
        serializers->setCurrentIndex(static_cast<int>(DisplayWidget::FileRecieve));
        }
    );

    connect(ui.menuSwitchPort, &QMenu::aboutToShow, this,
        [this]() {
            ui.menuSwitchPort->clear();
            bool portExist = false;

            Q_FOREACH(const QSerialPortInfo & info, QSerialPortInfo::availablePorts())
            {                
                QString portName = info.portName();
                QAction *action = new QAction(portName, this);
                connect(action, &QAction::triggered, this,
                    [this, portName]() {
                        showErrorMessage(comWorker->openPort(portName)); 
                    }
                );
                ui.menuSwitchPort->addAction(action);
                portExist = true;
            }
            
            if (!portExist)                
                showStatusbarMessage(QString("Ни один порт не найден"));
        }
    );

    connect(ui.actionShowAboutUsInfo, &QAction::triggered, this, &MainWindow::showInfoMessage);
    connect(ui.actionSwitchColorTheme, &QAction::triggered, this, &MainWindow::switchColorTheme);

    QGridLayout *centralWidgetLayout = new QGridLayout(this);

    // ORDER IS IMPORTANT HERE! Should be same as enum DisplayWidget
    serializers->addWidget(textSerializer);
    serializers->addWidget(textDeserializer);
    serializers->addWidget(fileSerializer);
    serializers->addWidget(fileDeserializer);

    centralWidgetLayout->addWidget(serializers);

    ui.centralWidget->setLayout(centralWidgetLayout);
    ui.statusBar->showMessage("Необходимо открыть порт");

    connect(comWorker, &COMWorker::workError, this, &MainWindow::showErrorMessage);
    connect(comWorker, &COMWorker::newStatusMessage, this, &MainWindow::showStatusbarMessage);

    connect(comWorker, &COMWorker::arraySent, this, &MainWindow::endSending);
    connect(comWorker, &COMWorker::arrayReceived, this, &MainWindow::endReceiving);
    connect(comWorker, &COMWorker::onStateChanged, this, &MainWindow::comworkerStateHandler);
}

void MainWindow::endReceiving(QByteArray msg)
{
    char format = msg[0];
    msg.remove(0, 1);

    if (format == 't')
    {
        if (serializers->currentIndex() != static_cast<int>(DisplayWidget::TextRecieve))
        {
            QMessageBox::information(this, "Сообщение получено", "Перейдите в меню для прочтения полученного сообщения");
            serializers->setCurrentIndex(static_cast<int>(DisplayWidget::TextRecieve));
        }

        textDeserializer->showDeserializedData(msg);
    }
    else if (format == 'f')
    {        
        fileDeserializer->saveDeserializedData(msg);
    }
    else
        showErrorMessage(ErrorCode::ReceiveFailed);
}

void MainWindow::endSending()
{
    QMessageBox::information(this, "Выполнено", "Отправка данных завершена");
}

void MainWindow::showStatusbarMessage(QString message)
{
    ui.statusBar->showMessage(message);
}

void MainWindow::comworkerStateHandler(State newState)
{
    if (newState == State::Idle)
    {
        textSerializer->setActive(true);
        fileSerializer->setActive(true);
    }
    else
    {
        textSerializer->setActive(false);
        fileSerializer->setActive(false);
    }
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
        QMessageBox::information(this, "Ошибка", "Не удалось открыть порт");
        break;
    case ErrorCode::SendFailed:
        QMessageBox::information(this, "Ошибка", "Не удалось отправить сообщение");
        break;
    case ErrorCode::ReceiveFailed:
        QMessageBox::information(this, "Ошибка", "Не удалось принять сообщение");
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
