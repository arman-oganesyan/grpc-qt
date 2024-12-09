#pragma once

#include <QtCore/QObject>
#include <QtCore/QDateTime>

// fwd declaration
class gRpcStub;

class ConnectionItem : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY(ConnectionItem)
public:
    ConnectionItem(QString host, int port, QObject* parent = nullptr);

    QString host() const { return m_host; }
    int port() const { return m_port; }
    QDateTime lastPing() const { return m_lastPing; }
    bool connected() const { return m_connected; }
    bool isRunning() const { return m_stub.get(); }
    QString toString() const { return QString("%0:%1").arg(host()).arg(port()); }

    void connectToServer();

private Q_SLOTS:
    void OnPingReceived();
    void OnPingFailed();

Q_SIGNALS:
    void updated();

private:
    QString m_host;
    int m_port;
    QDateTime m_lastPing;
    bool m_connected{ false };
    bool m_isRunning{ false };
    int m_failedPings{ 0 };

    std::unique_ptr<gRpcStub> m_stub;
};
