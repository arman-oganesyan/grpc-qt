#pragma once

#include <QtCore/QPointer>
#include <QtNetwork/QUdpSocket>
#include <QtNetwork/QNetworkAddressEntry>
#include <QtWidgets/QMainWindow>

#include <memory>

class QTextEdit;
class QLineEdit;
class gRpcWorker;

class ServerWindow : public QMainWindow
{
  Q_OBJECT
public:
    explicit ServerWindow(QWidget *parent = nullptr);
    ~ServerWindow() override;

protected:
    void timerEvent(QTimerEvent* event) override;

private:
    void findInterfaces();
    void sendBroadcastMessage();

    void startBroadcast();
    void stopBroadcast();

private Q_SLOTS:
  void OnStartClicked();

private:
  QPointer<QTextEdit> m_textEditLog;
  QPointer<QLineEdit> m_editPort;
  std::unique_ptr<gRpcWorker> m_rpcWorker;
  QList<QNetworkAddressEntry> m_addresses;

  QUdpSocket m_udpSocket;
  int m_broadcastTimer;
  QByteArray m_broadcastDatagram;
  quint64 m_lastRequestTimestamp;
};