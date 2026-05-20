#include "gamemodel.h"
#include <QDebug>

GameModel::GameModel(QObject *parent) : QObject(parent)
{
    startNewGame();
}

void GameModel::seed(quint32 s)
{
    m_rng = QRandomGenerator(s);
    m_seeded = true;
    startNewGame();
}

void GameModel::startNewGame()
{
    if (m_seeded) {
        m_secretNumber = m_rng.bounded(0, 100);
    } else {
        m_secretNumber = QRandomGenerator::global()->bounded(0, 100);
    }
    m_currentAttempts = 0;
    m_gameFinished = false;
    m_won = false;
    qDebug() << "[GameModel] New game, secret =" << m_secretNumber;
}

void GameModel::setMaxAttempts(int n)
{
    if (n < 1 || n > 100) {
        qWarning() << "[GameModel] Invalid maxAttempts:" << n << "; keeping" << m_maxAttempts;
        throw std::out_of_range("maxAttempts must be between 1 and 100");
    }
    m_maxAttempts = n;
}

GameModel::GuessResult GameModel::makeGuess(int value)
{
    if (m_gameFinished) {
        qWarning() << "[GameModel] Exception: game already finished";
        throw std::logic_error("Game is already finished");
    }
    if (value < 0 || value > 99) {
        qWarning() << "[GameModel] Exception: value out of range:" << value;
        throw std::out_of_range("Value must be between 0 and 99");
    }

    m_currentAttempts++;
    qDebug() << "[GameModel] Guess #" << m_currentAttempts << ":" << value;

    if (value == m_secretNumber) {
        m_gameFinished = true;
        m_won = true;
        return Correct;
    }
    if (m_currentAttempts >= m_maxAttempts) {
        m_gameFinished = true;
        return GameOver;
    }
    return (value < m_secretNumber) ? TooLow : TooHigh;
}

int GameModel::calculateScore() const
{
    if (!m_gameFinished || !m_won) return 0;
    return (m_maxAttempts - m_currentAttempts + 1) * 10;
}

int GameModel::parseInput(const QString &text)
{
    const QString t = text.trimmed();
    if (t.isEmpty()) {
        qWarning() << "[GameModel] Exception: empty input";
        throw std::invalid_argument("Input is empty");
    }
    bool ok = false;
    int value = t.toInt(&ok);
    if (!ok) {
        qWarning() << "[GameModel] Exception: invalid input:" << text;
        throw std::invalid_argument("Input is not a valid integer");
    }
    return value;
}
