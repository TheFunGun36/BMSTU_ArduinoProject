#include "fileserializer.h"
#include <qgridlayout.h>
#include <qmessagebox.h>
#include <qsizepolicy.h>
#include <qlabel.h>
#include <qwidget.h>
#include <qfiledialog.h>
#include <qfile.h>

FileSerializerWidget::FileSerializerWidget(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    QHBoxLayout *layout2 = new QHBoxLayout(this);

    label = new QLabel("Выберите файл, а затем нажмите кнопку \"отправить\", чтобы начать его отправить", this);
    buttonSend = new QPushButton("Отправить", this);
    buttonChoice = new QPushButton("Выбрать", this);
    fileDialog = new QFileDialog(this);
    filePath = "";

    connect(buttonSend, &QPushButton::clicked, this, &FileSerializerWidget::sendButtonClicked);
    connect(buttonChoice, &QPushButton::clicked, this, &FileSerializerWidget::choiceButtonClicked);

    layout->addWidget(label, 0, Qt::AlignLeft);
    layout->addLayout(layout2);
    layout2->addWidget(buttonChoice, 0, Qt::AlignLeft);
    layout2->addWidget(buttonSend, 0, Qt::AlignLeft);
    layout2->addStretch(1);
    layout->addStretch(1);
}

void FileSerializerWidget::setActive(bool value)
{
    isActive = value;
}

void FileSerializerWidget::sendButtonClicked()
{
    if (!isActive)
    {
        QMessageBox::information(this, "Ошибка", "Недостпно отправление данных\nво время другого процесса");
        return;
    }

    if (filePath.isEmpty())
    {
        QMessageBox::information(this, "Ошибка", "Необходимо выбрать файл перед отправкой");
        return;
    }

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
    if (!isActive)
    {
        QMessageBox::information(this, "Ошибка", "Недоступна смена файла\nво время другого процесса");
        return;
    }

    QString newFilePath = fileDialog->getOpenFileName();

    if (!newFilePath.isEmpty())
    {
        filePath = newFilePath;
        label->setText(filePath);
    }
}