#include "fileserializer.h"
#include <qgridlayout.h>
#include <qmessagebox.h>
#include <qlabel.h>
#include <qwidget.h>
#include <qfiledialog.h>
#include <qfile.h>

#include <qdebug.h>

FileSerializerWidget::FileSerializerWidget(QWidget *parent): QWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    QHBoxLayout* layout2 = new QHBoxLayout(this);

    label = new QLabel("Выберите файл, а затем нажмите кнопку \"отправить\", чтобы начать его отправить", this);    
    buttonSend = new QPushButton("Отправить", this);
    buttonChoice = new QPushButton("Выбрать", this);
    fileDialog = new QFileDialog(this);

    connect(buttonSend, &QPushButton::clicked, this, &FileSerializerWidget::sendButtonClicked);
    connect(buttonChoice, &QPushButton::clicked, this, &FileSerializerWidget::choiceButtonClicked);
    
    layout->addWidget(label, 0, Qt::AlignCenter);
    layout->addLayout(layout2);
    layout2->addSpacing(1200);
    layout2->addWidget(buttonChoice, 0, Qt::AlignRight);    
    layout2->addWidget(buttonSend, 0, Qt::AlignRight);
}

void FileSerializerWidget::sendButtonClicked()
{   
    QFile file(filePath);

    if (file.open(QIODevice::ReadOnly))
    {
        QString name = filePath;
        name.remove(0, name.lastIndexOf('/'));

        QByteArray result;
        result.append('f');
        result.append(name.toUtf8());
        result.append('\0');
        result.append(file.readAll());

        file.close();
        emit dataSerialized(result);
    }
    else 
    {
        QMessageBox::information(this, "Ошибка", "Не удалось открыть файл");
    }
}

void FileSerializerWidget::choiceButtonClicked()
{
    filePath = fileDialog->getOpenFileName();        
    
    if (filePath != 0)
        label->setText(filePath);
}

void FileSerializerWidget::buttonSetEnabled()
{
    buttonSend->setEnabled(true);
}

void FileSerializerWidget::buttonSetDisabled()
{
    buttonSend->setDisabled(true);
}