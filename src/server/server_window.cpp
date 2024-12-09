#include "server_window.h"

#include <QtCore/QDateTime>
#include <QtCore/QTimerEvent>
#include <QtNetwork/QUdpSocket>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QMessageBox>

#include "grpc_worker.h"

constexpr int kBroadcastInterval = 1000;
constexpr quint16 kBroadcastPort = 10001;

ServerWindow::ServerWindow(QWidget *parent) : QMainWindow(parent)
{
  auto centralWidget = new QWidget{};
  auto layout = new QGridLayout{centralWidget};

  auto startButton = new QPushButton{"Start"};
  connect(startButton, &QPushButton::clicked, this, &ServerWindow::OnStartClicked);

  m_textEditLog = new QTextEdit{};
  m_editPort = new QLineEdit{"50051"};

  layout->addWidget(m_textEditLog, 0, 0, 1, 2);
  layout->addWidget(new QLabel{"Port"}, 1, 0, 1, 1);
  layout->addWidget(m_editPort, 1, 1, 1, 1);
  layout->addWidget(startButton, 2, 1, 1, 1);

  setCentralWidget(centralWidget);

  // scan network interfaces
  findInterfaces();
}

ServerWindow::~ServerWindow()
{
}

void ServerWindow::timerEvent(QTimerEvent* event) {
    if (!event || event->timerId() != m_broadcastTimer) {
        QObject::timerEvent(event);
        return;
    }

    if (QDateTime::currentSecsSinceEpoch() - m_lastRequestTimestamp > 15) {
        sendBroadcastMessage();
    }
}

void ServerWindow::findInterfaces() {
    m_textEditLog->append("Find interfaces:");
    for (const auto& interface : QNetworkInterface::allInterfaces())
    {
        if (!(interface.flags() & QNetworkInterface::IsRunning))
            continue;

        for (const auto& entry : interface.addressEntries())
        {
            if (!entry.broadcast().isNull()) {
                m_addresses.append(entry);
                m_textEditLog->append(entry.ip().toString());
            }
        }
    }
    m_textEditLog->append("---------------------------");
}

void ServerWindow::sendBroadcastMessage() {
    m_textEditLog->append("Broadcast");
    for (const auto& entry : qAsConst(m_addresses)) {
        if (auto broadcast = entry.broadcast(); !broadcast.isNull()) {
            m_udpSocket.writeDatagram(m_broadcastDatagram, broadcast, kBroadcastPort);
        }
    }
}

void ServerWindow::startBroadcast() {
    // send first broadcast and start timer
    sendBroadcastMessage();
    m_broadcastTimer = startTimer(kBroadcastInterval);
}

void ServerWindow::stopBroadcast() {
    killTimer(m_broadcastTimer);
}

void ServerWindow::OnStartClicked() {
  if (m_rpcWorker && 
      QMessageBox::Yes != QMessageBox::question(this, "Server is already running", "Do you want to re-run it?")) {
      return;
  }

  // stop the previous broadcast if there is any
  stopBroadcast();

  auto port = m_editPort->text().toInt();
  m_broadcastDatagram = QByteArray::number(port);
  m_rpcWorker = std::make_unique<gRpcWorker>(m_addresses, port);

  connect(m_rpcWorker.get(), &gRpcWorker::started, [this]() { m_textEditLog->append("Start listening..."); });
  connect(m_rpcWorker.get(), &gRpcWorker::request, [this](const QString& peer) {
      m_lastRequestTimestamp = QDateTime::currentSecsSinceEpoch();
      m_textEditLog->append("Request was handled from " + peer);
  });

  // start grpc server and broadcast
  m_rpcWorker->start();
  startBroadcast();
}