#include "filedeserializer.h"
#include <qgridlayout.h>
#include <qmessagebox.h>
#include <qlabel.h>
#include <qwidget.h>
#include <qdir.h>
#include <qfile.h>

FileDeserializerWidget::FileDeserializerWidget(QWidget* parent) : QWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    label = new QLabel("Выберите директорию для сохранения файлов", this);
    buttonChoice = new QPushButton("Выбрать", this);
    fileDialog = new QFileDialog(this);    
      
    connect(buttonChoice, &QPushButton::clicked, this, &FileDeserializerWidget::choiceButtonClicked);

    layout->addWidget(label, 0, Qt::AlignLeft);
    layout->addWidget(buttonChoice, 0, Qt::AlignLeft);
    layout->addStretch(1);
}

void FileDeserializerWidget::saveDeserializedData(QByteArray data)
{    
    int len = data.indexOf('\0');
    QString fileName = QString(len, Qt::Uninitialized);
    for (int i = 0; i < len; i++)
        fileName[i] = data[i];
    data.remove(0, len + 1);

    QString filePath = sourcePath + "/" + fileName;
    QFile file(filePath);    

    if (file.open(QIODevice::WriteOnly))
    {
        file.write(data);
        file.close();
    }
    else
    {
        QMessageBox::information(this, "Ошибка", "Не удалось открыть файл");
    }
    QMessageBox::information(this, "Файл получен", "Файл сохранён по адрессу: " + sourcePath);
}

void FileDeserializerWidget::choiceButtonClicked() 
{
    QString newFilePath = fileDialog->getOpenFileName();

    if (!newFilePath.isEmpty())
    {
        sourcePath = newFilePath;
        label->setText(sourcePath);
    }
}