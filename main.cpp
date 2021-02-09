#include "window.h"
#include "controller.h"
#include "gamemodel.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Window w;
    GameModel gameModel;
    Controller controller(&w, &gameModel);
    w.show();
    return app.exec();
}
