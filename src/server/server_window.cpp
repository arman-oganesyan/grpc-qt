#include "server_window.h"

#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>

#include <QtNetwork/QNetworkInterface>
#include <QtNetwork/QUdpSocket>

ServerWindow::ServerWindow(QWidget *parent) : QMainWindow(parent)
{
  auto centralWidget = new QWidget{};
  auto layout = new QGridLayout{centralWidget};

  auto startButton = new QPushButton{"Start"};
  connect(startButton, &QPushButton::clicked, this, &ServerWindow::OnStartClicked);

  m_textEditLog = new QTextEdit{};

  layout->addWidget(m_textEditLog, 0, 0, 1, 2);
  layout->addWidget(new QLabel{"Port"}, 1, 0, 1, 1);
  layout->addWidget(new QLineEdit{}, 1, 1, 1, 1);
  layout->addWidget(startButton, 2, 1, 1, 1);

  setCentralWidget(centralWidget);
}

void ServerWindow::OnStartClicked()
{
  for (const auto &interface : QNetworkInterface::allInterfaces())
  {
    for (const auto &entry : interface.addressEntries())
    {
      auto broadcast = entry.broadcast();
      if (!broadcast.isNull() && !broadcast.isLoopback())
      {
        m_textEditLog->append(broadcast.toString());
        QUdpSocket{}.writeDatagram(QByteArray::number(8001), broadcast, 10001);
      }
    }
  }
}