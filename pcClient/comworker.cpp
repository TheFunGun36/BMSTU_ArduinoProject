#include "comworker.h"

COMWorker::COMWorker(QObject* parent)
	: QObject(parent)
{
	bufferSize = 4;
	comInputSize = 0;
	state = State::Idle;	
	ardReadyReadSymbol = '\xcc';
	serialPort = new QSerialPort(this);

	connect(serialPort, &QSerialPort::readyRead, this, &COMWorker::onComInformationReceive);
			
	/*foreach(const QSerialPortInfo & info, QSerialPortInfo::availablePorts())
	{
		QSerialPort port;
		port.setPort(info);
		if (port.open(QIODevice::ReadWrite))
		{
			qDebug() << "Name" + info.portName();
		}
	}*/
}

COMWorker::~COMWorker()
{
	serialPort->close();
}

ErrorCode COMWorker::openPort(QString name)
{
	if (serialPort->isOpen())
	{
		serialPort->close();
	}

	serialPort->setPortName(name);
	serialPort->setBaudRate(QSerialPort::Baud9600);
	if (!serialPort->open(QIODevice::ReadWrite))
	{
		return ErrorCode::OpenFailed;
	}

	return ErrorCode::Ok;
}

void COMWorker::sendArrayBegin(QByteArray arr)
{
	state = State::Sending;

	for (int i = 0; i < arr.size(); i += bufferSize)
	{
		int pkgSize = std::min(arr.size() - i, static_cast<int>(bufferSize));
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

	sendArray();
}

void COMWorker::onComInformationReceive() 
{	
	QByteArray msg;

	switch (state)
	{
	case State::Idle:
		state = State::Receiving;		
		receiveArray();
		break;
	case State::Receiving:
		receiveArray();
		break;
	case State::Sending:
		qDebug() << "ABOBA 3";
		msg = serialPort->readAll();
		if (msg.size() != 1 || msg[0] != ardReadyReadSymbol) 
		{
			state = State::Idle;
			emit workError(ErrorCode::SendFailed);
			break;
		}
		sendArray();
		break;	
	default:
		break;
	}
}

void COMWorker::sendArray()
{
	if (serialPort->write(packageQueue.dequeue()) < 0)
	{
		state = State::Idle;
		emit workError(ErrorCode::SendFailed);
		return;
	}

	if (packageQueue.isEmpty()) 
	{
		state = State::Idle;
		emit arraySent();
	}
}

void COMWorker::receiveArray()
{		
	char sizeByte = serialPort->read(1)[0];
	
	if (sizeByte < 0) 
	{
		emit workError(ErrorCode::ReceiveFailed);
		state = State::Idle;
		return;
	}

	char msgSize = sizeByte > 0 ? sizeByte : bufferSize;

	QByteArray msg = serialPort->read(msgSize);
	arrayToReceive.append(msg);	

	if (sizeByte != 0) 
	{
		msg = arrayToReceive;
		arrayToReceive.clear();
		state = State::Idle;
		emit arrayReceived(msg);			
	}
}
