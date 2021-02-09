#include "gamemodel.h"
#include <QDebug>

GameModel::GameModel()
{
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &GameModel::handleUpdate);
}

void GameModel::setGameMode(const QString& gameMode)
{
    if(gameMode.compare(QString("Challenge")) || gameMode.compare(QString("Free practise")))
        this->gameMode = gameMode;
}

QString GameModel::getGameMode() const
{
    return gameMode;
}

QString GameModel::prepareNewGame(const QString& language)
{
    setWordSet(language);
    floatingWords = QVector<QString>();
    currentInputWord = QString("");
    timeSeconds = 0;
    timeMilliseconds = 0;
    speed = 1;
    typedEntries = 0;
    missedWords = 0;
    typingErrors = 0;
    timer->start(intervalMilliseconds);
    distribution = std::uniform_int_distribution<>(0, wordSet.size()-1);
    const QString newWord = getRandomWord();
    floatingWords.append(newWord);
    return newWord;
}

void GameModel::stopGame()
{
    timer->stop();
    wordSet = QVector<QString>();
}

GameState GameModel::getGameState() const
{
    GameState state;
    state.timeSec = static_cast<int>(timeSeconds);
    state.speed = speed;
    state.typingErrors = typingErrors;
    state.netWPM = calculateNetWPM();
    state.currentInputWord = currentInputWord;
    state.lives = maxLives - missedWords;
    return state;
}

void GameModel::handleKeyClicked(const QString& key)
{
    if(key.compare(enter) != 0)
    {
        currentInputWord += key;
    }
    else if(currentInputWord.size() > 0)
    {
        const int foundWordIndex = floatingWords.indexOf(currentInputWord);
        qDebug() << currentInputWord << " " << foundWordIndex;
        if(foundWordIndex != -1)
        {
            typedEntries += currentInputWord.size();
            floatingWords.removeAt(foundWordIndex);
            // handle speed changes for challenge mode
            emit deleteFloatingWord(foundWordIndex);
        }
        else
        {
            typingErrors += 1;
        }
        currentInputWord = QString("");
    }
}

void GameModel::handleWordOutOfBounds(const QString& word)
{
    const int wordIndex = floatingWords.indexOf(word);
    floatingWords.removeAt(wordIndex);
    missedWords += 1;
    emit deleteFloatingWord(wordIndex);
}

void GameModel::handleSpeedChangeRequest(const int& speed)
{
    if((speed-10) * (speed-1) <= 0)
        this->speed = speed;
}

void GameModel::handleUpdate()
{
    const float intervalMilliseconds = static_cast<float>(this->intervalMilliseconds);
    timeSeconds += intervalMilliseconds / 1000;
    timeMilliseconds += intervalMilliseconds;
    if(timeMilliseconds >= calculateNewWordInterval())
    {
        timeMilliseconds = 0;
        const QString newWord = getRandomWord();
        floatingWords.append(newWord);
        qDebug() << floatingWords;
        emit update(getGameState(), newWord);
    }
    else
    {
        emit update(getGameState(), QString(""));
    }
}

int GameModel::calculateNetWPM() const
{
    if(timeSeconds > 0)
        return static_cast<int>(((static_cast<float>(typedEntries)/5) - static_cast<float>(typingErrors)) / (timeSeconds/60));
    return 0;
}

float GameModel::calculateNewWordInterval() const
{
    return static_cast<float>(5100 - (460*(speed-1)));
}

QString GameModel::getWordSetPath(const QString& language) const
{
    if(language.compare(QString("English")) == 0)
        return QString(":/wordsets/English.txt");

    return QString(":/wordsets/English.txt");
}

void GameModel::setWordSet(const QString& language)
{
    QFile file(getWordSetPath(language));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        wordSet = QVector<QString>();
        return;
    }
    QTextStream in(&file);
    while (!in.atEnd())
        wordSet.append(in.readLine());
}

QString GameModel::getRandomWord()
{
    return wordSet.at(distribution(generator));
}

