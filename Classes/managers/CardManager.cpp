#include "CardManager.h"
#include <sstream>
#include <random>
#include "cocos2d.h"

USING_NS_CC;

CardManager::CardManager(GameModel* model) : _model(model), _nextCardId(1) {
    if (!_model) CCLOGERROR("CardManager: null GameModel!");
}

CardManager::~CardManager() {
    CCLOG("CardManager destroyed");
}

CardModel* CardManager::createCard(const CardConfig& config) {
    if (!_model) return nullptr;
    
    auto card = new CardModel();
    card->face = config.face;
    card->suit = config.suit;
    card->position = config.position;
    card->isFaceUp = config.isFaceUp;
    card->isRemoved = false;
    card->cardId = generateCardId();
    card->layerIndex = static_cast<int>(_model->mainCards.size() + _model->coveredCards.size());
    
    return card;
}

CardModel* CardManager::createRandomCard() {
    if (!_model) return nullptr;
    
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> faceDist(0, CFT_NUM_CARD_FACE_TYPES - 1);
    std::uniform_int_distribution<> suitDist(0, CST_NUM_CARD_SUIT_TYPES - 1);
    
    CardConfig config;
    config.face = static_cast<CardFaceType>(faceDist(gen));
    config.suit = static_cast<CardSuitType>(suitDist(gen));
    config.isFaceUp = false;
    config.position = cocos2d::Vec2::ZERO;
    
    return createCard(config);
}

void CardManager::removeCard(CardModel* card) {
    if (!card || !_model) return;
    card->isRemoved = true;
    _model->moveToRemoved(card);
}

void CardManager::flipCard(CardModel* card) {
    if (card) card->isFaceUp = !card->isFaceUp;
}

std::vector<CardModel*> CardManager::getAvailableCards() {
    return _model ? _model->getAvailableCards() : std::vector<CardModel*>();
}

std::vector<CardModel*> CardManager::getCoveredCards() {
    std::vector<CardModel*> covered;
    if (!_model) return covered;
    
    for (auto card : _model->coveredCards) {
        if (!card->isFaceUp && !card->isRemoved) covered.push_back(card);
    }
    return covered;
}

std::string CardManager::generateCardId() {
    std::stringstream ss;
    ss << "card_" << _nextCardId++;
    return ss.str();
}