#include <QtWidgets/QApplication>

#include "client_window.h"

int main(int argc, char** argv) {
    QApplication app{ argc, argv };

    ClientWindow clientWindow{};
    clientWindow.show();

    return app.exec();
}