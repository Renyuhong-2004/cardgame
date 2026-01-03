#pragma once
#include <vector>
#include <algorithm>
#include "CardEnums.h"
#include "cocos2d.h"

// 卡牌模型
class CardModel {
public:
    CardFaceType face;
    CardSuitType suit;
    cocos2d::Vec2 position;
    bool isFaceUp;
    bool isRemoved;
    std::string cardId;
    int layerIndex;

    CardModel() : 
        face(CFT_ACE), 
        suit(CST_SPADES), 
        position(cocos2d::Vec2::ZERO),
        isFaceUp(false), 
        isRemoved(false),
        layerIndex(0) {}
};

// 游戏数据模型
class GameModel {
public:
    std::vector<CardModel*> mainCards;      // 主牌堆（可点击的翻开牌）
    std::vector<CardModel*> coveredCards;   // 主牌堆的覆盖牌
    CardModel* bottomCard = nullptr;        // 当前底牌
    std::vector<CardModel*> backupCards;    // 备用牌堆
    std::vector<CardModel*> removedCards;   // 已消除牌

    // 获取可用牌（未消除的主牌）
    std::vector<CardModel*> getAvailableCards() {
        std::vector<CardModel*> available;
        for (auto card : mainCards) {
            if (!card->isRemoved) available.push_back(card);
        }
        return available;
    }

    // 移至已消除列表
    void moveToRemoved(CardModel* card) {
        if (card) removedCards.push_back(card);
    }
};

// 游戏控制器（空实现，可扩展）
class GameController {
public:
    GameController(GameModel* model) : _model(model) {}
    ~GameController() {}

private:
    GameModel* _model;
};