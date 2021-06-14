#pragma once

#include <qserialport>
#include <qserialportinfo.h>
#include <qqueue.h>

enum class ErrorCode
{
    Ok,
    OpenFailed,
    SendFailed,
    ReceiveFailed
};

enum class State
{
    Idle,
    Sending,
    Receiving,
    PortOpenning
};

class COMWorker : public QObject
{
    Q_OBJECT
private:
    char bufferSize;
    char ardReadErrorSymbol;
    char ardSendStartSymbol;
    char ardSendReadySymbol;
    char ardSendFinishSymbol;
    char ardPortCheckSymbol;
    char countPacks;
    State state;

    QSerialPort *serialPort;
    QByteArray arrayToSend;
    QByteArray arrayToReceive;
    QQueue<QByteArray> packageQueue;

    void sendArray();
    void receiveArray();
    void setState(State value);

public:
    ErrorCode openPort(QString name);
    COMWorker(QObject *parent);
    COMWorker::~COMWorker();

signals:
    void startArraySending();
    void arraySent();
    void startArrayReceiving();
    void arrayReceived(QByteArray msg);
    void newStatusMessage(QString msg);
    void workError(ErrorCode);
    void onStateChanged(State);

private slots:
    void onComInformationReceive();

public slots:
    void sendArrayBegin(QByteArray arr);
};
