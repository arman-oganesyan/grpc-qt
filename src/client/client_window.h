#pragma once

#include <QtWidgets/QMainWindow>

// fwd declarations
class QTableView;
class QUdpSocket;
class ButtonDelegate;
class ConnectionsModel;

class ClientWindow : public QMainWindow {
Q_OBJECT
public:
    explicit ClientWindow(QWidget* parent = nullptr);

private Q_SLOTS:
    void OnReadyRead() const;

private:
    QUdpSocket* m_udpSocket;

    ConnectionsModel* m_model;
    QTableView* m_view;
    ButtonDelegate* m_buttonDelegate;
};
