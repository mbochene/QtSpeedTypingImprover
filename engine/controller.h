#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include "window.h"
#include "gamemodel.h"
#include "abstractkeyboardhandler.h"
#include "englishkeyboardhandler.h"
#include <memory>

class Controller : public QObject
{
    Q_OBJECT
    public:
        explicit Controller(Window* window, GameModel* gameModel);

    signals:
        void gameReady(const QString& gameMode, const GameState& state, const QString& newWord);
        void update(const GameState& state, const QString& newWord);
        void keyPressed(const QString& key);

    private slots:
        void handleGameModeChosen(const QString& gameMode);
        void handleLanguageChosen(const QString& language);
        void handleKeyEvent(const QKeyEvent* keyEvent);
        void handleQuitClicked();

    private:
        Window* window;
        GameModel* gameModel;
        std::unique_ptr<AbstractKeyboardHandler> keyboardHandler;
};

#endif // CONTROLLER_H
