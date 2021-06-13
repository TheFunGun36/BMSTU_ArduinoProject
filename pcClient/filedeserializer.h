#pragma once
#include <qwidget.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qfiledialog.h>

class FileDeserializerWidget : public QWidget
{
    Q_OBJECT
public:
    FileDeserializerWidget(QWidget* parent = Q_NULLPTR);

public slots:
    void saveDeserializedData(QByteArray data);

private:
    QLabel *label;
    QPushButton *buttonChoice;
    QFileDialog *fileDialog;
    QString sourcePath;

private slots:
    void choiceButtonClicked();

signals:
    void dataSerialized(QByteArray data);
};
