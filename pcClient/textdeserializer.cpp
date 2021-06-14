#include "textdeserializer.h"
#include <qboxlayout.h>
#include <qlabel.h>

TextDeserializerWidget::TextDeserializerWidget(QWidget *parent) : QWidget(parent)
{
    textOutput = new QPlainTextEdit(this);
    textOutput->setReadOnly(true);

    QVBoxLayout *layout = new QVBoxLayout(this);

    layout->addWidget(new QLabel("В поле ниже будет отображён принятый текст."));
    layout->addWidget(textOutput);
}

void TextDeserializerWidget::showDeserializedData(QByteArray data)
{
    QString text = QString::fromUtf8(data);
    textOutput->setPlainText(text);
}