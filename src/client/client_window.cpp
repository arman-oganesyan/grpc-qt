#include "client_window.h"

#include <QtNetwork/QNetworkInterface>
#include <QtNetwork/QUdpSocket>
#include <QtNetwork/QNetworkDatagram>
#include <QtWidgets/QTableView>

#include "connections_model.h"
#include "delegates/button_delegate.h"

ClientWindow::ClientWindow(QWidget* parent)
    : QMainWindow(parent)
{
    m_model = new ConnectionsModel{ this };
    m_buttonDelegate = new ButtonDelegate{ this };
    connect(m_buttonDelegate.get(), &ButtonDelegate::clicked, m_model.get(), &ConnectionsModel::connectToServer);
    m_view = new QTableView{ this };
    m_view->setModel(m_model);
    m_view->setItemDelegateForColumn(3, m_buttonDelegate);

    m_view->setColumnWidth(+ConnectionsModelColumns::Ip, 125);
    m_view->setColumnWidth(+ConnectionsModelColumns::LastPing, 150);
    m_view->setColumnWidth(+ConnectionsModelColumns::Status, 50);
    m_view->setColumnWidth(+ConnectionsModelColumns::Action, 100);

    setCentralWidget(m_view);

    for (const auto& interface : QNetworkInterface::allInterfaces())
    {
        if (!(interface.flags() & QNetworkInterface::IsRunning))
            continue;

        for (const auto& entry : interface.addressEntries())
        {
            if (!entry.broadcast().isNull()) {
                auto udpSocket = std::make_unique<QUdpSocket>();
                connect(udpSocket.get(), &QUdpSocket::readyRead, this, &ClientWindow::OnReadyRead);
                if (udpSocket->bind(entry.ip(), 10001))
                {
                    m_udpSockets.push_back(std::move(udpSocket));
                }
            }
        }
    }

    resize(480, 320);
}

ClientWindow::~ClientWindow() {
}

void ClientWindow::OnReadyRead() {
    if (auto socket = qobject_cast<QUdpSocket*>(sender()); socket) {
        auto datagram = socket->receiveDatagram();
        if (auto port = datagram.data().toInt(); port > 0) {
            m_model->addConnectionItem(datagram.senderAddress().toString(), datagram.data().toInt());
        }
    }
}