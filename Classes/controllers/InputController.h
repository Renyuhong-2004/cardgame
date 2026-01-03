#ifndef CARD_CONTROLLER_H
#define CARD_CONTROLLER_H

#include "models/GameModel.h"
#include "services/GameModelGenerator.h"

// 卡牌业务控制器（处理卡牌核心逻辑）
class CardController {
public:
    CardController(GameModel* gameModel) : _gameModel(gameModel) {}

    // 检查卡牌是否可匹配（数字差1）
    bool isCardMatch(CardModel* card1, CardModel* card2) {
        if (!card1 || !card2) return false;
        return abs(static_cast<int>(card1->getNumber()) - static_cast<int>(card2->getNumber())) == 1;
    }

    // 消除主牌区指定列的顶层牌
    bool removeMainCardTop(int colIndex) {
        auto& mainCards = _gameModel->getMainCards();
        if (colIndex < 0 ||static_cast<size_t>(colIndex) >= mainCards.size() || mainCards[colIndex].empty()) return false;

        CardModel* topCard = mainCards[colIndex].back();
        if (!isCardMatch(_gameModel->getBottomCard(), topCard)) return false;

        // 移除牌并更新底牌
        mainCards[colIndex].pop_back();
        _gameModel->setBottomCard(topCard);

        // 翻开下一层牌
        if (!mainCards[colIndex].empty()) {
            mainCards[colIndex].back()->setIsOpen(true);
        }

        // 检查胜利
        checkWin();
        return true;
    }

    // 从备用牌抽取新底牌
    bool drawReserveCard() {
        auto& reserveCards = _gameModel->getReserveCards();
        if (reserveCards.empty()) return false;

        CardModel* newBottom = reserveCards.back();
        reserveCards.pop_back();
        newBottom->setIsOpen(true);
        _gameModel->setBottomCard(newBottom);

        // 检查游戏结束
        checkGameOver();
        return true;
    }

    // 检查胜利
    void checkWin() {
        auto& mainCards = _gameModel->getMainCards();
        bool allEmpty = true;
        for (auto& col : mainCards) {
            if (!col.empty()) {
                allEmpty = false;
                break;
            }
        }
        _gameModel->setIsWin(allEmpty);
        _gameModel->setIsGameOver(allEmpty);
    }

    // 检查游戏结束（失败）
    void checkGameOver() {
        auto& reserveCards = _gameModel->getReserveCards();
        if (!reserveCards.empty()) return;

        // 备用牌耗尽且无匹配牌
        bool hasMatch = false;
        auto& mainCards = _gameModel->getMainCards();
        for (auto& col : mainCards) {
            if (col.empty()) continue;
            if (isCardMatch(_gameModel->getBottomCard(), col.back())) {
                hasMatch = true;
                break;
            }
        }

        _gameModel->setIsGameOver(!hasMatch);
    }

private:
    GameModel* _gameModel;
};

#endif // CARD_CONTROLLER_H