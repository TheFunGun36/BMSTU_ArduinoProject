#include <qserialport.h>
#include <qserialportinfo.h>
#include <qqueue.h>
#include <qdebug.h>

enum class ErrorCode {
	Ok,
	OpenFailed,
	SendFailed,
	ReceiveFailed
};

class COMWorker : public QObject
{
	Q_OBJECT
private:
	int bufferSize;
	QSerialPort serialPort;
	QByteArray arrayToSend;
	QByteArray arrayToReceive;
	QQueue<QByteArray> packageQueue;

public:
	ErrorCode openPort(QString name);
	COMWorker(QObject *parent);
	COMWorker::~COMWorker();
	void sendArrayBegin(QByteArray arr);
	
signals:
	void arraySent();
	void arrayReceived();
	void workError(ErrorCode);

private slots:
	void sendArray();
	void receiveArray();
};
