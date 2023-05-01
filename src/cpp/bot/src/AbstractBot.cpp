#include "AbstractBot.h"

Result AbstractBot::miniMaxSearch(bool isBlackTurn, int depth, float alpha, float beta) {
    Result miniMaxResult;

    if (this->isGameOver() || depth == 0) {
        miniMaxResult.payoff = this->getUtility();
        return miniMaxResult;
    }

    vector<string> validMoves = this->getValidMoves(isBlackTurn);
    if (validMoves.size() == 0) {
        miniMaxResult.payoff = this->getUtility();
        return miniMaxResult;
    }

    miniMaxResult.payoff = isBlackTurn ? beta : alpha;
    for (string move : validMoves) {
        AbstractBot* currentBotCopy = this->clone();
        currentBotCopy->executeMove(move);

        float childAlpha = isBlackTurn ? alpha : miniMaxResult.payoff;
        float childBeta = isBlackTurn ? miniMaxResult.payoff : beta;
        Result resultForMove = currentBotCopy->miniMaxSearch(!isBlackTurn, depth - 1, childAlpha, childBeta);
        delete currentBotCopy;

        if ((isBlackTurn && (resultForMove.payoff < miniMaxResult.payoff)) ||
            (!isBlackTurn && (resultForMove.payoff > miniMaxResult.payoff))) {
            miniMaxResult.payoff = resultForMove.payoff;
            miniMaxResult.strategy = resultForMove.strategy;
            miniMaxResult.strategy.insert(miniMaxResult.strategy.begin(), move);
            if ((isBlackTurn && miniMaxResult.payoff <= alpha) || (!isBlackTurn && miniMaxResult.payoff >= beta)) {
                break;
            }
        }
    }

    return miniMaxResult;
}

Result AbstractBot::iterativeDeepeningSearch(bool isBlackTurn) {
    Result result = miniMaxSearch(isBlackTurn, 5, -0.12, 10.12);
    return result;
}