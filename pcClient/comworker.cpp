#include "comworker.h"

COMWorker::COMWorker(QObject *parent)
    : QObject(parent)
{
    bufferSize = 56;
    countPacks = 1;
    state = State::Idle;
    ardReadErrorSymbol = '\xce';
    ardSendStartSymbol = '\xcb';
    ardSendReadySymbol = '\xcc';
    ardSendFinishSymbol = '\xcd';
    ardPortCheckSymbol = '\xc0';
    serialPort = new QSerialPort(this);

    connect(serialPort, &QSerialPort::readyRead, this, &COMWorker::onComInformationReceive);
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

    state = State::PortOpenning;
    serialPort->write(QByteArray(1, ardPortCheckSymbol));
    emit newStatusMessage(QString("Ожидание ответа от микроконтроллера"));

    return ErrorCode::Ok;
}

void COMWorker::sendArrayBegin(QByteArray arr)
{
    if (!serialPort->isOpen())
    {
        newStatusMessage(QString("Необходимо открыть порт"));
        return;
    }

    state = State::Sending;
    arrayToSend.clear();
    packageQueue.clear();
    emit startArraySending();

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
    qDebug() << serialPort->peek(255);
    QByteArray msg;

    switch (state)
    {
    case State::Idle:
        arrayToReceive.clear();
        countPacks = 1;
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
        if (msg.size() == 1 && msg[0] == ardSendFinishSymbol)
        {
            newStatusMessage(QString("Сообщение отправлено. Отправлено пакетов: ") + QString::number(countPacks));
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
    case State::PortOpenning:
        if (serialPort->readAll()[0] == ardPortCheckSymbol)
        {            
            emit newStatusMessage(QString("Порт с микроконтроллером открыт"));            
            state = State::Idle;
            // Unlock Button
            emit workError(ErrorCode::Ok);
        }
        break;
    }
}

void COMWorker::sendArray()
{
    QByteArray toWrite = packageQueue.dequeue();
    qDebug() << toWrite;

    if (serialPort->write(toWrite) != toWrite.size())
    {
        serialPort->readAll();
        state = State::Idle;
        newStatusMessage("Передача сообщения прервана");        
        emit workError(ErrorCode::SendFailed);
        return;
    }

    if (packageQueue.isEmpty())
    {
        newStatusMessage(QString("Отправляется пакет № ") + QString::number(countPacks) + QString(" [Последний]"));
        return;
    }

    newStatusMessage(QString("Отправляется пакет № ") + QString::number(countPacks));

    countPacks++;
}

void COMWorker::receiveArray()
{
    char sizeByte = serialPort->peek(1)[0];
    char msgSize = sizeByte > 0 ? sizeByte : bufferSize;

    newStatusMessage(QString("Получение пакета № ") + QString::number(countPacks) + QString(", размер: ") + QString::number(msgSize));

    if (serialPort->size() > msgSize)
    {
        countPacks++;
        serialPort->read(1);

        if (sizeByte < 0)
        {
            newStatusMessage(QString("Получение сообщения прервано"));
            state = State::Idle;
            emit workError(ErrorCode::ReceiveFailed);
            return;
        }

        QByteArray msg = serialPort->read(msgSize);
        arrayToReceive.append(msg);

        if (sizeByte != 0)
        {
            msg = arrayToReceive;
            state = State::Idle;
            emit arrayReceived(msg);
            return;
        }
    }
}
