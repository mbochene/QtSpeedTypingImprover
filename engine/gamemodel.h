#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include <QObject>
#include <QTimer>
#include <QVector>
#include <QFile>
#include <QTextStream>
#include <random>
#include <chrono>

struct GameState
{
    int timeSec, speed, typingErrors, lives, netWPM;
    QString currentInputWord;
};

struct GameOverScore
{
    int timeSec, typingErrors, netWPM, typedEntries;
};

class GameModel : public QObject
{
    Q_OBJECT
    public:
        explicit GameModel();

        void setGameMode(const QString& gameMode);
        QString getGameMode() const;
        QString prepareNewGame(const QString& language);
        void stopGame();
        GameState getGameState() const;
        GameOverScore getGameOverScore() const;

    public slots:
        void handleKeyPressed(const QString& key);
        void handleWordOutOfBounds(const QString& word);
        void handleSpeedChangeRequest(const int& speed);

    signals:
        void update(const GameState& state, const QString& newWord);
        void deleteFloatingWord(const int& wordIndex);
        void gameOver(const GameOverScore& score);

    private slots:
        void handleUpdate();

    private:
        const QString enter = QString("\r");
        const QString challenge = QString("Challenge");
        std::mt19937 generator = std::mt19937(std::chrono::system_clock::now().time_since_epoch().count());
        std::uniform_int_distribution<> distribution;
        const int intervalMilliseconds = 17;
        QTimer* timer;

        QVector<QString> wordSet;
        QVector<QString> floatingWords;
        QString gameMode, currentInputWord;

        float timeSeconds, timeMilliseconds;
        int speed, typedEntries, missedWords, typingErrors, challengeSpeedLevel;
        const int maxLives = 5, minSpeed = 1, maxSpeed = 5;

        void setChallengeSpeedLevel(const QString& language);
        int calculateNetWPM() const;
        float calculateNewWordInterval() const;
        QString getWordSetPath(const QString& language) const;
        void setWordSet(const QString& language);
        QString getRandomWord();
};

#endif // GAMEMODEL_H
