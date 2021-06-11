#include <qserialport.h>
#include <qserialportinfo.h>
#include <qqueue.h>
#include <qdebug.h>

enum class ErrorCode {
	Ok,
	OpenFailed
};

class COMWorker : public QObject
{
	Q_OBJECT
private:
	int bufferSize;
	QSerialPort serialPort;
	QByteArray arrayToSend;
	QQueue<QByteArray> packageQueue;
	void sendArray();

public:
	ErrorCode openPort(QString name);
	COMWorker(QObject *parent);
	COMWorker::~COMWorker();
	void sendArrayBegin(QByteArray arrayToSend);
	
signals:
	void arraySent();
	void arraySentError(ErrorCode);
};
