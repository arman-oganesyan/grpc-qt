#include "button_delegate.h"

#include <QtGui/QPainter>
#include <QtGui/QMouseEvent>
#include <QtWidgets/QApplication>
#include <QtWidgets/QStyle>

#include "connections_model.h"

auto toButtonStyle(const QStyleOptionViewItem& option, const QModelIndex& index) {
    QStyleOptionButton buttonOption;
    buttonOption.rect = option.rect.adjusted(3, 3, -3, -3);
    buttonOption.text = index.data().toString();
    buttonOption.state = QStyle::State_Enabled;
    return buttonOption;
}

ButtonDelegate::ButtonDelegate(QObject* parent)
    : QStyledItemDelegate(parent)
{
}

void ButtonDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {
    auto buttonOption = toButtonStyle(option, index);
    QApplication::style()->drawControl(QStyle::CE_PushButton, &buttonOption, painter);
}

bool ButtonDelegate::editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index) {
    if (event->type() == QEvent::MouseButtonRelease) {
        auto* mouseEvent = static_cast<QMouseEvent*>(event);
        if (option.rect.contains(mouseEvent->pos())) {
            emit clicked(index);
            return true;
        }
    }
    return false;
}

QSize ButtonDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    auto buttonOption = toButtonStyle(option, index);
    return QApplication::style()->sizeFromContents(QStyle::CT_PushButton, &buttonOption, {});
}
