#include "window.h"
#include "ui_window.h"
#include <QDebug>

Window::Window(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Window)
{
    ui->setupUi(this);
    const qreal graphicsViewWidth = ui->graphicsView->contentsRect().width();
    const qreal graphicsViewHeight = ui->graphicsView->contentsRect().height();
    scene = new QGraphicsScene(0,0, graphicsViewWidth, graphicsViewHeight, ui->graphicsView);
    ui->graphicsView->setScene(scene);

    connect(ui->challengeButton, &QPushButton::released, this, &Window::handleChallengeClicked);
    connect(ui->freePractiseButton, &QPushButton::released, this, &Window::handleFreePractiseClicked);
    connect(ui->englishButton, &QPushButton::released, this, [=](){ emit languageChosen(QString("English")); });
    connect(ui->polishButton, &QPushButton::released, this, [=](){ emit languageChosen(QString("Polish")); });
    connect(ui->backButton, &QPushButton::released, this, [=](){ ui->stackedWidget->setCurrentIndex(0); });
    connect(ui->quitButton, &QPushButton::released, this, &Window::handleQuitClicked);
    connect(ui->gamePage, &QGameWidget::keyClicked, this, &Window::handleKeyClicked);
    connect(ui->fasterButton, &QPushButton::released, this, [=](){ emit speedChangeRequest(speed + 1); });
    connect(ui->slowerButton, &QPushButton::released, this, [=](){ emit speedChangeRequest(speed - 1); });
}

Window::~Window()
{
    delete ui;
}

void Window::handleGameReady(const QString& gameMode, const GameState& state, const QString& newWord)
{
    previousChosenHeightLevel = -1;
    this->gameMode = gameMode;
    const bool speedButtonsVisible = gameMode.compare(challengeAsStr) != 0;
    ui->slowerButton->setVisible(speedButtonsVisible);
    ui->fasterButton->setVisible(speedButtonsVisible);
    updateWidgets(state);
    addNewWord(newWord);
    speed = state.speed;
    ui->stackedWidget->setCurrentIndex(2);
}

void Window::handleUpdate(const GameState& state, const QString& newWord)
{
    speed = state.speed;
    lives = state.lives;
    updateWidgets(state);
    addNewWord(newWord);
    moveFloatingWords(state.speed);
}

void Window::handleDeleteFloatingWord(const int& wordIndex)
{
    deleteFloatingWordAt(wordIndex);
}

void Window::handleGameOver()
{
   //////////////// TODO : pokazywanie wynikow
   cleanGameScene();
}

void Window::handleChallengeClicked()
{
    emit gameModeChosen(QString("Challenge"));
    ui->stackedWidget->setCurrentIndex(1);
}

void Window::handleFreePractiseClicked()
{
    emit gameModeChosen(QString("Free practise"));
    ui->stackedWidget->setCurrentIndex(1);
}

void Window::handleQuitClicked()
{
    emit quitClicked();
    cleanGameScene();
}

void Window::handleKeyClicked(const QKeyEvent* keyEvent)
{
    qDebug() << keyEvent->text();
    emit keyClicked(keyEvent);
}

void Window::updateWidgets(const GameState& state)
{
    ui->speedLabel->setText(speedText + QString::number(state.speed));
    ui->netWPMValueLabel->setText(QString::number(static_cast<double>(state.netWPM)));
    ui->errorsValueLabel->setText(QString::number(state.typingErrors));
    if(this->gameMode.compare(challengeAsStr) != 0)
        ui->livesValueLabel->setText(QString("Inf"));
    else
        ui->livesValueLabel->setText(QString::number(state.lives));
    updateInputLabel(state.currentInputWord);
    ui->timeLabel->setText(timeText + QString::number(state.timeSec));
}

void Window::updateInputLabel(const QString& text)
{
    if(text.compare(inputLabelText) != 0)
    {
        inputLabelText = text;
        QFont f = ui->inputLabel->font(); //Get label font
        const QFontMetrics metrics(f);
        const QSize size = metrics.size(0, text); //Get size of text
        const float widthFactor = static_cast<float>(ui->inputLabel->width()) / static_cast<float>(size.width()); //Get the width factor
        const float heightFactor =  static_cast<float>(ui->inputLabel->height()) / static_cast<float>(size.height()); //Get the height factor
        const double factor = static_cast<double>(qMin(widthFactor, heightFactor)); //To fit contents in the screen select as factor
                                                                                    //the minimum factor between width and height
        f.setPointSizeF(0.8 * f.pointSizeF() * factor); //Set font size
        ui->inputLabel->setText(text);
        ui->inputLabel->setFont(f); //Set the adjusted font to the label
    }
}

void Window::addNewWord(const QString& newWord)
{
    if(newWord.size() > 0)
    {
        QGraphicsSimpleTextItem* floatingWord = scene->addSimpleText(newWord);
        floatingWord->setPos(static_cast<int>(scene->width()), getRandomHeight());

        const QBrush brush(Qt::green);
        floatingWord->setBrush(brush);
        const QFont font("Consolas", 14);
        floatingWord->setFont(font);
        floatingWords.append(std::move(floatingWord));
    }
}

int Window::getRandomHeight()
{
    int newHeightLevel = heightDistribution(generator);
    while(newHeightLevel == previousChosenHeightLevel)
        newHeightLevel = heightDistribution(generator);
    previousChosenHeightLevel = newHeightLevel;
    return 10 + 32 * newHeightLevel;
}

void Window::moveFloatingWords(const int& speed)
{
    for(QGraphicsSimpleTextItem* floatingWord : floatingWords)
    {
        floatingWord->setPos(floatingWord->x() - (0.5 + (speed * 0.12)), floatingWord->y());
        if(isWordVisible(floatingWord))
        {
            const qreal red = 255 * ((scene->width() - floatingWord->x()) / scene->width());
            const qreal green = 255 * (floatingWord->x() / scene->width());
            if((red - 255) * red <= 0 && (green - 255) * green <= 0)
            {
                QColor color(static_cast<int>(red), static_cast<int>(green), 0);
                floatingWord->setBrush(QBrush(color));
            }
            else
            {
                floatingWord->setBrush(QBrush(Qt::red));
            }
        }
        else
        {
            emit wordOutOfBounds(floatingWord->text());
            qDebug() << lives;
            if(lives == 1)
                return;
        }
    }
}

bool Window::isWordVisible(const QGraphicsSimpleTextItem* item) const
{
    return item->x() + item->boundingRect().width() >= 0;
}

void Window::deleteFloatingWordAt(const int& index)
{
    QGraphicsSimpleTextItem* floatingWord = floatingWords.at(index);
    scene->removeItem(floatingWord);
    floatingWords.removeAt(index);
}

void Window::cleanGameScene()
{
    ui->stackedWidget->setCurrentIndex(0);
    scene->clear();
    floatingWords.clear();
}
