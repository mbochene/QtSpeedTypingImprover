#include "gamemodel.h"
#include <QDebug>

GameModel::GameModel()
{
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &GameModel::handleUpdate);
    floatingWords = QVector<QString>();
    currentInputWord = QString();
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
    wordSet.clear();
    floatingWords.clear();
    currentInputWord.clear();
}

GameState GameModel::getGameState() const
{
    GameState state;
    state.timeSec = timeSeconds < 99999 ? static_cast<int>(timeSeconds) : 99999;  // so that the number does not go through the label
    state.speed = speed;
    state.typingErrors = typingErrors < 99999 ? typingErrors : 99999;   // so that the number does not go through the label
    state.netWPM = static_cast<int>(calculateNetWPM());
    state.currentInputWord = currentInputWord;
    state.lives = gameMode.compare(challenge) == 0 ? maxLives - missedWords : maxLives;
    return state;
}

GameOverScore GameModel::getGameOverScore() const
{
    GameOverScore score;
    score.netWPM = static_cast<int>(calculateNetWPM());
    score.timeSec = static_cast<int>(timeSeconds);
    score.typedEntries = typedEntries;
    score.typingErrors = typingErrors;
    return score;
}

void GameModel::handleKeyPressed(const QString& key)
{
    if(!timer->isActive())
        return;

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

            if(gameMode.compare(challenge) == 0 && calculateNetWPM() / challengeSpeedLevel > speed)
                handleSpeedChangeRequest(speed + 1);

            emit deleteFloatingWord(foundWordIndex);
        }
        else
        {
            typingErrors += 1;
        }
        currentInputWord.clear();
    }
}

void GameModel::handleWordOutOfBounds(const QString& word)
{
    const int wordIndex = floatingWords.indexOf(word);
    floatingWords.removeAt(wordIndex);
    missedWords += 1;
    if(gameMode.compare(challenge) !=0 || missedWords < maxLives)
    {
        emit deleteFloatingWord(wordIndex);
    }
    else
    {
        stopGame();
        emit gameOver(getGameOverScore());
    }
}

void GameModel::handleSpeedChangeRequest(const int& speed)
{
    if((speed - maxSpeed) * (speed - minSpeed) <= 0)
        this->speed = speed;
}

void GameModel::handleUpdate()
{
    const qreal intervalMilliseconds = this->intervalMilliseconds;
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

qreal GameModel::calculateNetWPM() const
{
    if(timeSeconds > 0)
        return ((static_cast<qreal>(typedEntries)/5) - static_cast<qreal>(typingErrors)) / (timeSeconds/60);
    return 0;
}

qreal GameModel::calculateNewWordInterval() const
{
    const qreal candidate = static_cast<qreal>( 4500 - (1200 * (speed - 1) - 300 * (speed-2)) );
    return candidate > 1000 ? candidate : 1000;
}

QString GameModel::getWordSetPath(const QString& language) const
{
    if(language.compare(QString("English")) == 0)
        return QString(":/wordsets/English.txt");
    else if(language.compare(QString("Polish")) == 0)
        return QString(":/wordsets/Polish.txt");

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
    in.setCodec("UTF-8");
    while (!in.atEnd())
        wordSet.append(in.readLine());
}

QString GameModel::getRandomWord()
{
    return wordSet.at(distribution(generator));
}

