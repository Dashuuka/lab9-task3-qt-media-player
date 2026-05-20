#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include <QObject>
#include <QRandomGenerator>
#include <stdexcept>

class GameModel : public QObject
{
    Q_OBJECT
public:
    explicit GameModel(QObject *parent = nullptr);

    void startNewGame();
    int maxAttempts() const { return m_maxAttempts; }
    int currentAttempts() const { return m_currentAttempts; }
    int secretNumber() const { return m_secretNumber; }
    bool isGameFinished() const { return m_gameFinished; }
    bool isWon() const { return m_won; }

    enum GuessResult { TooLow, TooHigh, Correct, GameOver };
    Q_ENUM(GuessResult)

    GuessResult makeGuess(int value);
    int calculateScore() const;

    void setMaxAttempts(int n);

    static int parseInput(const QString &text);

    // Reproducible RNG seeding (used by tests)
    void seed(quint32 s);

private:
    int m_secretNumber = 0;
    int m_currentAttempts = 0;
    int m_maxAttempts = 10;
    bool m_gameFinished = false;
    bool m_won = false;
    QRandomGenerator m_rng;
    bool m_seeded = false;
};

#endif
