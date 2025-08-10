#include "connections_model.h"

#include <QtCore/QStringList>
#include <QtGui/QColor>

#include <ranges>

#include "connection_item.h"

ConnectionsModel::ConnectionsModel(QObject* parent)
    : QAbstractTableModel(parent)
{}

void ConnectionsModel::addConnectionItem(QString host, int port)
{
    auto find_result = std::ranges::find_if(m_items, [host, port](const auto& entry) { 
        return entry->host() == host && entry->port() == port;
    });

    if (find_result == m_items.cend()) {
        emit beginInsertRows({}, m_items.size(), m_items.size() + 1);
        auto newItem = new ConnectionItem(host, port, this);

        connect(newItem, &ConnectionItem::updated, this, [this]() {
            if (auto connectionItem = std::ranges::find(m_items, sender()); connectionItem != m_items.end()) {
                auto row = std::distance(m_items.begin(), connectionItem);
                emit dataChanged(index(row, 0), index(row, +ConnectionsModelColumns::ColumnCount - 1));
            }
        });

        m_items.push_back(newItem);
        emit endInsertRows();
    }
}

void ConnectionsModel::connectToServer(const QModelIndex& index)
{
    if (index.isValid() || index.row() < m_items.size()) {
        m_items[index.row()]->connectToServer();
    }
}

int ConnectionsModel::rowCount(const QModelIndex& parent) const
{
    return parent.isValid() ? 0 : m_items.size();

}

int ConnectionsModel::columnCount(const QModelIndex& parent) const
{
    return parent.isValid() ? 0 : +ConnectionsModelColumns::ColumnCount;
}

QVariant ConnectionsModel::data(const QModelIndex& index, int role) const
{
    if (index.isValid() && index.row() >= 0 && index.row() < m_items.size()) {
        const auto& item = m_items.at(index.row());

        if (role == Qt::DisplayRole) {
            switch (static_cast<ConnectionsModelColumns>(index.column())) {
            case ConnectionsModelColumns::Ip:
                return item->toString();
            case ConnectionsModelColumns::LastPing:
                return item->lastPing().isValid() ? item->lastPing().toString(Qt::ISODate) : "-";
            case ConnectionsModelColumns::Status:
                return item->connected() ? "Online" : "Offline";
            case ConnectionsModelColumns::Action:
                return item->isRunning() ? "Disconnect" : "Connect";
            case ConnectionsModelColumns::ColumnCount:
                Q_UNREACHABLE();
            }
        }
        else if (role == Qt::ForegroundRole) {
            if (index.column() == +ConnectionsModelColumns::Status) {
                return QColor{ item->connected() ? Qt::green : Qt::red};
            }
        }
    }

    return {};
}

QVariant ConnectionsModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole || section >= +ConnectionsModelColumns::ColumnCount) {
        return QAbstractTableModel::headerData(section, orientation, role);
    }

    static QStringList kHeader{
        "IP", "Last ping time", "Status", "Action"
    };

    return kHeader[section];
}