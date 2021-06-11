#include "comworker.h"

COMWorker::COMWorker(QObject *parent)
	: QObject(parent)
{
	bufferSize = 4;
}

COMWorker::~COMWorker()
{
	serialPort.close();
}

ErrorCode COMWorker::openPort(QString name)
{
	if (serialPort.isOpen())
	{
		serialPort.close();
	}

	serialPort.setPortName(name);
	serialPort.setBaudRate(QSerialPort::Baud9600);
	if (!serialPort.open(QIODevice::ReadWrite)) 
	{
		return ErrorCode::OpenFailed;
	}

	return ErrorCode::Ok;
}

void COMWorker::sendArrayBegin(QByteArray arr)
{
	for (int i = 0; i < arr.size(); i += bufferSize)
	{
		int pkgSize = std::min(arr.size() - i, bufferSize);
		pkgSize++;
		QByteArray pkg(pkgSize, Qt::Initialization::Uninitialized);

		auto itSrc = arr.begin() + i;
		for (auto it = pkg.begin() + 1; it != pkg.end(); it++)
		{
			*it = *itSrc;
			itSrc++;
		}

		if (arr.size() - i > bufferSize)
			pkg[0] = static_cast<char>(0);
		else
			pkg[0] = static_cast<char>(pkgSize - 1);

		packageQueue.enqueue(pkg);
	}

	qDebug() << "DEBUG sendArrayBegin";
	Q_FOREACH(QByteArray baba, packageQueue)
	{		
		qDebug() << baba;
	}

	serialPort.write(packageQueue.dequeue());
	if (packageQueue.isEmpty())
	{
		// Данные отправлены
	}
}

void COMWorker::sendArray()
{
	serialPort.write(packageQueue.dequeue());

	if (packageQueue.isEmpty()) 
	{
		// Данные отправлены
	}
}

void COMWorker::receiveArray()
{
	int pkgSize = serialPort.read(1).toInt();
	int msgSize = pkgSize > 0 ? pkgSize : bufferSize;

	QByteArray msg = serialPort.read(msgSize);
	arrayToReceive.append(msg);

	if (pkgSize != 0) 
	{
		// Данные получены
	}
}
