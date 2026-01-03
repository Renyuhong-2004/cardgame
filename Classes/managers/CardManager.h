#pragma once
#include "models/GameModel.h"
#include "services/ConfigService.h"

class CardManager {
public:
    CardManager(GameModel* model);
    ~CardManager();

    CardModel* createCard(const CardConfig& config);
    CardModel* createRandomCard();
    void removeCard(CardModel* card);
    void flipCard(CardModel* card);
    std::vector<CardModel*> getAvailableCards();
    std::vector<CardModel*> getCoveredCards();

private:
    GameModel* _model;
    int _nextCardId;
    std::string generateCardId();
};