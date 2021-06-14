#include "textserializer.h"
#include <qgridlayout.h>
#include <qmessagebox.h>
#include <qlabel.h>

TextSerializerWidget::TextSerializerWidget(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    textInput = new QPlainTextEdit(this);
    buttonSend = new QPushButton("Отправить", this);    

    connect(buttonSend, &QPushButton::clicked, this, &TextSerializerWidget::sendButtonClicked);

    layout->addWidget(new QLabel("Ниже расположено поле для ввода текста.\n"
        "Введите какой-либо текст, а затем нажмите кнопку \"отправить\", чтобы начать отправку сообщения"));
    layout->addWidget(textInput);
    layout->addWidget(buttonSend, 0, Qt::AlignRight);
}

void TextSerializerWidget::setActive(bool value)
{
    isActive = value;
}

void TextSerializerWidget::sendButtonClicked()
{
    if (!isActive)
    {
        QMessageBox::information(this, "Ошибка", "Недостпно отправление данных\nво время другого процесса");
        return;
    }

    QString str = textInput->toPlainText();

    if (str.isEmpty())
    {
        QMessageBox::information(this, "Отправка текста", "Отправить можно только текст,\nсостоящий как минимум из одного символа");
    }
    else
    {
        QByteArray result;
        result.append('t');
        result.append(str.toUtf8());
        emit dataSerialized(result);
    }
}