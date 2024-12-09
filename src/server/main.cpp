#include <QtWidgets/QApplication>

#include "server_window.h"

int main(int argc, char **argv)
{
  QApplication app{argc, argv};

  ServerWindow serverWindow{};
  serverWindow.show();

  return app.exec();
}