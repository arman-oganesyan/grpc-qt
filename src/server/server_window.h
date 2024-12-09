#include <QtWidgets/QMainWindow>
#include <QtCore/QPointer>
#include <QtNetwork/QUdpSocket>

class QTextEdit;
class QUdpSocket;

class ServerWindow : public QMainWindow
{
  Q_OBJECT
public:
  ServerWindow(QWidget *parent = nullptr);

private:
  void OnStartClicked();

private:
  QPointer<QTextEdit> m_textEditLog;
};