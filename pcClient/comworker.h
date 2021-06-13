#include <qserialport>
#include <qserialportinfo.h>
#include <qqueue.h>
#include <qdebug.h>

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
    Receiving
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
    char countPacks;
    State state;

    QSerialPort *serialPort;
    QByteArray arrayToSend;
    QByteArray arrayToReceive;
    QQueue<QByteArray> packageQueue;

    void sendArray();
    void receiveArray();

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

private slots:
    void onComInformationReceive();

public slots:
    void sendArrayBegin(QByteArray arr);
};
