#pragma once

#include <QtCore/QPointer>
#include <QtWidgets/QMainWindow>

#include <memory>
#include <vector>

// fwd declarations
class QTableView;
class QUdpSocket;
class ButtonDelegate;
class ConnectionsModel;

class ClientWindow : public QMainWindow {
Q_OBJECT
public:
    explicit ClientWindow(QWidget* parent = nullptr);
    ~ClientWindow();

private Q_SLOTS:
    void OnReadyRead();

private:
    std::vector<std::unique_ptr<QUdpSocket>> m_udpSockets;

    QPointer<ConnectionsModel> m_model;
    QPointer<QTableView> m_view;
    QPointer<ButtonDelegate> m_buttonDelegate;
};
