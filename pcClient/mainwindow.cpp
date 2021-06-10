#include "mainwindow.h"
#include <qgridlayout.h>
#include <qpushbutton.h>
#include <qmessagebox.h>

#include <qdebug.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    serializers = new QStackedWidget(this);
    textSerializer = new TextSerializerWidget(this);
    textDeserializer = new TextDeserializerWidget(this);

    connect(textSerializer, &TextSerializerWidget::dataSerialized, this, &MainWindow::sendSerializedData);
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
    connect(ui.actionShowAboutUsInfo, &QAction::triggered, this,
        [this]() {
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
    );

    QGridLayout *centralWidgetLayout = new QGridLayout(this);

    // ORDER IS IMPORTANT HERE! Should be same as enum DisplayWidget
    serializers->addWidget(textSerializer);
    serializers->addWidget(textDeserializer);

    centralWidgetLayout->addWidget(serializers);

    ui.centralWidget->setLayout(centralWidgetLayout);
}

void MainWindow::sendSerializedData(QByteArray data)
{
    //qDebug() << data;
}

void MainWindow::showStatusbarMessage(QString message)
{
    ui.statusBar->showMessage(message);
}