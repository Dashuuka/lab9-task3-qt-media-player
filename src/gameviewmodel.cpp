#include "gameviewmodel.h"
#include <QDebug>

GameViewModel::GameViewModel(QObject *parent) : QObject(parent)
{
    newGame();
}

void GameViewModel::setSettingsManager(SettingsManager *sm)
{
    m_settings = sm;
    if (m_settings) {
        connect(m_settings, &SettingsManager::settingsChanged,
                this, &GameViewModel::statsChanged);
    }
    emit statsChanged();
}

int GameViewModel::launchCount()   const { return m_settings ? m_settings->launchCount()   : 0; }
int GameViewModel::gamesPlayed()   const { return m_settings ? m_settings->gamesPlayed()   : 0; }
int GameViewModel::totalAttempts() const { return m_settings ? m_settings->totalAttempts() : 0; }
int GameViewModel::totalScore()    const { return m_settings ? m_settings->totalScore()    : 0; }
int GameViewModel::bestScore()     const { return m_settings ? m_settings->bestScore()     : 0; }
QString GameViewModel::appUuid()   const { return m_settings ? m_settings->appUuid()       : QString(); }

void GameViewModel::newGame()
{
    m_model.startNewGame();
    m_statusText = tr("Guess the number!");
    m_imageSource = QStringLiteral("qrc:/qt/qml/GuessNumberX/icons/question.svg");
    m_animationType = QStringLiteral("fadeIn");
    updateAttemptsText();
    emit statusChanged();
    emit animationRequested();
}

void GameViewModel::setMaxAttempts(int n)
{
    try {
        m_model.setMaxAttempts(n);
        updateAttemptsText();
        emit statusChanged();
    } catch (const std::exception &e) {
        qCritical() << "[GameViewModel] setMaxAttempts failed:" << e.what();
        emit inputError(tr("Attempts must be 1..100"));
    }
}

void GameViewModel::refreshStatus()
{
    updateAttemptsText();
    emit statusChanged();
}

void GameViewModel::submitGuessInt(int value)
{
    try {
        if (m_model.isGameFinished()) {
            newGame();
            return;
        }
        const auto result = m_model.makeGuess(value);
        switch (result) {
        case GameModel::TooLow:
            m_statusText = tr("Too low! (%1)").arg(value);
            m_imageSource = QStringLiteral("qrc:/qt/qml/GuessNumberX/icons/arrow_up.svg");
            m_animationType = QStringLiteral("bounce");
            break;
        case GameModel::TooHigh:
            m_statusText = tr("Too high! (%1)").arg(value);
            m_imageSource = QStringLiteral("qrc:/qt/qml/GuessNumberX/icons/arrow_down.svg");
            m_animationType = QStringLiteral("bounce");
            break;
        case GameModel::Correct: {
            m_statusText = tr("You win!");
            m_imageSource = QStringLiteral("qrc:/qt/qml/GuessNumberX/icons/trophy.svg");
            m_animationType = QStringLiteral("rotate");
            const int score = m_model.calculateScore();
            if (m_settings) {
                m_settings->saveGameResult(m_model.currentAttempts(), score);
                m_settings->appendHistory(m_model.secretNumber(),
                                          m_model.currentAttempts(), score, true);
            }
            emit showResultDialog(tr("Victory!"),
                tr("Number %1 guessed in %2 attempts!\nScore: %3")
                    .arg(m_model.secretNumber())
                    .arg(m_model.currentAttempts())
                    .arg(score));
            break;
        }
        case GameModel::GameOver:
            m_statusText = tr("The number was: %1").arg(m_model.secretNumber());
            m_imageSource = QStringLiteral("qrc:/qt/qml/GuessNumberX/icons/question.svg");
            m_animationType = QStringLiteral("fadeIn");
            if (m_settings) {
                m_settings->saveGameResult(m_model.currentAttempts(), 0);
                m_settings->appendHistory(m_model.secretNumber(),
                                          m_model.currentAttempts(), 0, false);
            }
            emit showResultDialog(tr("Game Over"),
                tr("Out of attempts! The number was: %1").arg(m_model.secretNumber()));
            break;
        }
        refreshStatus();
        emit animationRequested();
    } catch (const std::out_of_range &e) {
        qWarning() << "[GameViewModel] Range error:" << e.what();
        emit inputError(tr("Enter a number from 0 to 99!"));
    } catch (const std::logic_error &e) {
        qWarning() << "[GameViewModel] Logic error:" << e.what();
        emit inputError(tr("Game already finished!"));
    } catch (const std::exception &e) {
        qCritical() << "[GameViewModel] Unexpected:" << e.what();
        emit inputError(tr("Internal error"));
    }
}

void GameViewModel::submitGuess(const QString &input)
{
    int value;
    try {
        value = GameModel::parseInput(input);
    } catch (const std::invalid_argument &e) {
        qWarning() << "[GameViewModel] Input error:" << e.what();
        emit inputError(tr("Invalid input!"));
        return;
    }
    submitGuessInt(value);
}

void GameViewModel::updateAttemptsText()
{
    m_attemptsText = tr("Attempts: %1 of %2")
                         .arg(m_model.currentAttempts())
                         .arg(m_model.maxAttempts());
}
