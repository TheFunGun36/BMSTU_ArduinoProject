#include "comworker.h"

COMWorker::COMWorker(QObject *parent)
    : QObject(parent)
{
    bufferSize = 63;
    comInputSize = 0;
    countPacks = 0;
    state = State::Idle;
    ardReadErrorSymbol = '\xce';
    ardSendStartSymbol = '\xcb';
    ardSendReadySymbol = '\xcc';
    ardSendFinishSymbol = '\xcd';
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
    emit startArraySending();
    countPacks = 0;

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

    sendArray();
}

void COMWorker::onComInformationReceive()
{
    QByteArray msg;

    switch (state)
    {
    case State::Idle:
        countPacks = 0;
        if (serialPort->read(1)[0] != ardSendStartSymbol)
        {
            serialPort->readAll();
            newStatusMessage(QString("Получен неопознанный сигнал"));
            break;
        }
        state = State::Receiving;
        emit startArrayReceiving();
        receiveArray();
        break;
    case State::Receiving:
        if (serialPort->peek(1)[0] == ardReadErrorSymbol)
        {
            serialPort->readAll();
            state = State::Idle;
            newStatusMessage(QString("Получение сообщения прервано"));
            workError(ErrorCode::ReceiveFailed);
            break;
        }
        receiveArray();
        break;
    case State::Sending:
        msg = serialPort->readAll();
        if (msg.size() == 1 || msg[0] == ardSendFinishSymbol)
        {
            newStatusMessage(QString("Сообщение отправлено. Отправлено пакетов: ") + QString(countPacks));
            state = State::Idle;
            emit arraySent();
            break;
        }
        if (msg.size() != 1 || msg[0] != ardSendReadySymbol)
        {
            serialPort->readAll();
            state = State::Idle;
            newStatusMessage(QString("Передача сообщения прервана"));
            emit workError(ErrorCode::SendFailed);
            break;
        }
        sendArray();
        break;
    }
}

void COMWorker::sendArray()
{
    countPacks++;

    if (serialPort->write(packageQueue.dequeue()) < 0)
    {
        serialPort->readAll();
        state = State::Idle;
        newStatusMessage(QString::fromUtf8("Передача сообщения прервана"));
        //newStatusMessage(QString::fromUtf8("Test"));
        emit workError(ErrorCode::SendFailed);
        return;
    }

    if (packageQueue.isEmpty())
    {
        newStatusMessage(QString("Отправляется пакет № ") + QString(countPacks) + QString(" [Последний]"));
        return;
    }

    newStatusMessage(QString("Отправляется пакет № ") + QString(countPacks));
}

void COMWorker::receiveArray()
{
    countPacks++;
    char sizeByte = serialPort->peek(1)[0];
    char msgSize = sizeByte > 0 ? sizeByte : bufferSize;

    if (serialPort->size() >= msgSize)
    {
        serialPort->read(1);

        if (sizeByte < 0)
        {
            newStatusMessage(QString("Получение сообщения прервано"));
            emit workError(ErrorCode::ReceiveFailed);
            state = State::Idle;
            return;
        }

        QByteArray msg = serialPort->read(msgSize);
        arrayToReceive.append(msg);

        if (sizeByte != 0)
        {
            msg = arrayToReceive;
            arrayToReceive.clear();
            state = State::Idle;
            emit arrayReceived(msg);
            return;
        }

        newStatusMessage(QString("Получение пакета № ") + QString(countPacks));
    }
}
