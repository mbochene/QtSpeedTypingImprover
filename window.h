#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QFile>
#include <QVector>
#include <QGraphicsScene>
#include <QGraphicsSimpleTextItem>
#include <random>
#include <chrono>
#include "gamemodel.h"
#include "qgamewidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Window; }
QT_END_NAMESPACE

class Window : public QWidget
{
    Q_OBJECT

    public:
        Window(QWidget *parent = nullptr);
        ~Window();

    signals:
        void gameModeChosen(const QString& gameMode);
        void languageChosen(const QString& language);
        void quitClicked();
        void keyClicked(const QKeyEvent* keyEvent);
        void wordOutOfBounds(const QString& word);
        void speedChangeRequest(const int& speed);

    public slots:
        void handleGameReady(const QString& gameMode, const GameState& state, const QString& newWord);
        void handleUpdate(const GameState& state, const QString& newWord);
        void handleDeleteFloatingWord(const int& wordIndex);
        void handleGameOver();

    private slots:
        void handleChallengeClicked();
        void handleFreePractiseClicked();
        void handleQuitClicked();
        void handleKeyClicked(const QKeyEvent* keyEvent);

    private:
        Ui::Window *ui;
        std::mt19937 generator = std::mt19937(std::chrono::system_clock::now().time_since_epoch().count());
        std::uniform_int_distribution<> heightDistribution = std::uniform_int_distribution<>(0, 10);
        const QString challengeAsStr = QString("Challenge");
        const QString speedText = QString("Speed: ");
        const QString timeText = QString("Time: ");
        QString inputLabelText = QString("");
        QString gameMode;
        QGraphicsScene* scene;
        QVector<QGraphicsSimpleTextItem*> floatingWords;
        int previousChosenHeightLevel = -1;
        int speed;
        int lives;

        void updateWidgets(const GameState& state);
        void updateInputLabel(const QString& text);
        void addNewWord(const QString& newWord);
        int getRandomHeight();
        void moveFloatingWords(const int& speed);
        bool isWordVisible(const QGraphicsSimpleTextItem* item) const;
        void deleteFloatingWordAt(const int& index);
        void cleanGameScene();
};
#endif // WINDOW_H
