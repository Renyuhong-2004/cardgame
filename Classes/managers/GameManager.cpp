#include "CardManager.h"
#include <sstream>
#include <string>

CardManager::CardManager(GameModel* model) : _model(model), _nextCardId(1) {
}

CardModel* CardManager::createCard(const CardConfig& config) {
    CardModel* card = new CardModel();
    card->face = config.face;
    card->suit = config.suit;
    card->position = config.position;
    card->isFaceUp = config.isFaceUp;
    card->cardId = generateCardId();
    
    // 设置层级
    card->layerIndex = static_cast<int>(_model->playfieldCards.size());
    
    return card;
}

CardModel* CardManager::createRandomCard() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> faceDist(0, CFT_NUM_CARD_FACE_TYPES - 1);
    std::uniform_int_distribution<> suitDist(0, CST_NUM_CARD_SUIT_TYPES - 1);
    
    CardConfig config;
    config.face = static_cast<CardFaceType>(faceDist(gen));
    config.suit = static_cast<CardSuitType>(suitDist(gen));
    config.isFaceUp = true;
    config.position = cocos2d::Vec2::ZERO;
    
    return createCard(config);
}

void CardManager::removeCard(CardModel* card) {
    if (!card || !_model) return;
    
    _model->moveToRemoved(card);
}

void CardManager::flipCard(CardModel* card) {
    if (card) {
        card->isFaceUp = !card->isFaceUp;
    }
}

std::vector<CardModel*> CardManager::getAvailableCards() {
    return _model->getAvailableCards();
}

std::vector<CardModel*> CardManager::getCoveredCards() {
    std::vector<CardModel*> covered;
    if (!_model) return covered;
    
    for (auto card : _model->playfieldCards) {
        if (!card->isFaceUp && !card->isRemoved) {
            covered.push_back(card);
        }
    }
    return covered;
}

std::string CardManager::generateCardId() {
    std::stringstream ss;
    ss << "card_" << _nextCardId++;
    return ss.str();
}