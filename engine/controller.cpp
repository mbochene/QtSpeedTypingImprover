#include "controller.h"

Controller::Controller(Window* window, GameModel* gameModel): window(window), gameModel(gameModel)
{
    connect(window, &Window::gameModeChosen, this, &Controller::handleGameModeChosen);
    connect(window, &Window::languageChosen, this, &Controller::handleLanguageChosen);
    connect(this, &Controller::gameReady, window, &Window::handleGameReady);
    connect(gameModel, &GameModel::update, window, &Window::handleUpdate);
    connect(this, &Controller::update, window, &Window::handleUpdate);
    connect(window, &Window::keyClicked, this, &Controller::handleKeyEvent);
    connect(this, &Controller::keyPressed, gameModel, &GameModel::handleKeyClicked);
    connect(window, &Window::quitClicked, this, &Controller::handleQuitClicked);
    connect(gameModel, &GameModel::deleteFloatingWord, window, &Window::handleDeleteFloatingWord);
    connect(window, &Window::wordOutOfBounds, gameModel, &GameModel::handleWordOutOfBounds);
    connect(window, &Window::speedChangeRequest, gameModel, &GameModel::handleSpeedChangeRequest);
}

void Controller::handleGameModeChosen(const QString& gameMode)
{
    gameModel->setGameMode(gameMode);
}

void Controller::handleLanguageChosen(const QString& language)
{
    if(language.compare(QString("English")) == 0)
        keyboardHandler = std::unique_ptr<EnglishKeyboardHandler>(new EnglishKeyboardHandler());
    const QString firstWord = gameModel->prepareNewGame(language);
    emit gameReady(gameModel->getGameMode(), gameModel->getGameState(), firstWord);
}

void Controller::handleKeyEvent(const QKeyEvent* keyEvent)
{
    const QString key = keyboardHandler.get()->handleKeyboardPress(keyEvent->text(), keyEvent->modifiers().testFlag(Qt::AltModifier));
    emit keyPressed(key);
}

void Controller::handleQuitClicked()
{
    gameModel->stopGame();
}
