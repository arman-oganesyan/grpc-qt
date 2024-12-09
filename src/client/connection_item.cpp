#include "connection_item.h"

#include <grpc++/grpc++.h>

#include "grpc_stub.h"

ConnectionItem::ConnectionItem(QString host, int port, QObject* parent)
    : QObject(parent)
    , m_host(host)
    , m_port(port)
{}

void ConnectionItem::connectToServer() {
    if (m_stub) {
        m_connected = false;
        m_stub.reset();
    }
    else {
        m_stub = std::make_unique<gRpcStub>(grpc::CreateChannel(toString().toStdString(), grpc::InsecureChannelCredentials()));
        connect(m_stub.get(), &gRpcStub::pingResponse, this, &ConnectionItem::OnPingReceived);
        connect(m_stub.get(), &gRpcStub::pingFailed, this, &ConnectionItem::OnPingFailed);
        m_stub->start();
    }

    emit updated();
}

void ConnectionItem::OnPingReceived() {
    m_connected = true;
    m_lastPing = QDateTime::currentDateTime();
    m_failedPings = 0;
    emit updated();
}

void ConnectionItem::OnPingFailed() {
    if (++m_failedPings >= 3) {
        m_connected = false;
        m_stub.reset();
        emit updated();
    }
}