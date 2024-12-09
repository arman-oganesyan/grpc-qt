#pragma once

#include <QtCore/QAbstractTableModel>

#include <memory>
#include <vector>

#include "grpc_stub.h"

enum class ConnectionsModelColumns;
class ConnectionItem;

// Tool to convert ConnectionsModelColumns value to underlying type for easier use
constexpr std::underlying_type<ConnectionsModelColumns>::type operator+(ConnectionsModelColumns e)
{
    return static_cast<std::underlying_type<ConnectionsModelColumns>::type>(e);
}

enum class ConnectionsModelColumns {
    Ip,
    LastPing,
    Status,
    Action,
    // Allways at the end!
    ColumnCount
};

class ConnectionsModel : public QAbstractTableModel {
    Q_OBJECT
public:
    explicit ConnectionsModel(QObject* parent = nullptr);
    ~ConnectionsModel();

    void addConnectionItem(QString host, int port);
    void connectToServer(const QModelIndex& index);

    int rowCount(const QModelIndex& parent) const override;
    int columnCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

private:
    std::vector<std::unique_ptr<ConnectionItem>> m_items;
};