#pragma once

#include <QtCore/QAbstractTableModel>

#include <vector>

#include "grpc_stub.h"

enum class ConnectionsModelColumns;
class ConnectionItem;

// Tool to convert ConnectionsModelColumns value to underlying type for easier use
constexpr std::underlying_type_t<ConnectionsModelColumns> operator+(ConnectionsModelColumns e)
{
    return static_cast<std::underlying_type_t<ConnectionsModelColumns>>(e);
}

enum class ConnectionsModelColumns {
    Ip,
    LastPing,
    Status,
    Action,
    // Always at the end!
    ColumnCount
};

class ConnectionsModel : public QAbstractTableModel {
    Q_OBJECT
public:
    explicit ConnectionsModel(QObject* parent = nullptr);

    void addConnectionItem(QString host, int port);
    void connectToServer(const QModelIndex& index);

    [[nodiscard]] int rowCount(const QModelIndex& parent) const override;
    [[nodiscard]] int columnCount(const QModelIndex& parent) const override;
    [[nodiscard]] QVariant data(const QModelIndex& index, int role) const override;
    [[nodiscard]] QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    std::vector<ConnectionItem*> m_items;
};