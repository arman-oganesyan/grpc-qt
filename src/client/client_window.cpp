#include "client_window.h"

#include <QtNetwork/QNetworkInterface>
#include <QtNetwork/QNetworkDatagram>
#include <QtNetwork/QUdpSocket>
#include <QtWidgets/QTableView>

#include "connections_model.h"
#include "delegates/button_delegate.h"

ClientWindow::ClientWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_udpSocket(new QUdpSocket(this))
{
    m_model = new ConnectionsModel{ this };
    m_buttonDelegate = new ButtonDelegate{ this };
    connect(m_buttonDelegate, &ButtonDelegate::clicked, m_model, &ConnectionsModel::connectToServer);
    m_view = new QTableView{ this };
    m_view->setModel(m_model);
    m_view->setItemDelegateForColumn(3, m_buttonDelegate);

    m_view->setColumnWidth(+ConnectionsModelColumns::Ip, 125);
    m_view->setColumnWidth(+ConnectionsModelColumns::LastPing, 150);
    m_view->setColumnWidth(+ConnectionsModelColumns::Status, 50);
    m_view->setColumnWidth(+ConnectionsModelColumns::Action, 100);

    setCentralWidget(m_view);

    connect(m_udpSocket, &QUdpSocket::readyRead, this, &ClientWindow::OnReadyRead);
    m_udpSocket->bind(QHostAddress::AnyIPv4, 10001, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);

    resize(480, 320);
}

void ClientWindow::OnReadyRead() const {
    auto datagram = m_udpSocket->receiveDatagram();
    if (auto port = datagram.data().toInt(); port > 0) {
        m_model->addConnectionItem(datagram.senderAddress().toString(), datagram.data().toInt());
    }
}
