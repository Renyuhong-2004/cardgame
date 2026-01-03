// Classes/views/PlayfieldView.cpp
#include "PlayfieldView.h"

USING_NS_CC;

PlayfieldView* PlayfieldView::create() {
    PlayfieldView* view = new PlayfieldView();
    if (view && view->init()) {
        view->autorelease();
        return view;
    }
    delete view;
    return nullptr;
}

bool PlayfieldView::init() {
    if (!Node::init()) {
        return false;
    }
    
    // 设置主牌区大小
    this->setContentSize(Size(1080, 1500));
    
    return true;
}

void PlayfieldView::setCards(const std::vector<CardModel*>& cards) {
    clearCardViews();
    
    for (auto cardModel : cards) {
        addCard(cardModel);
    }
}

void PlayfieldView::addCard(CardModel* card) {
    if (!card) return;
    
    auto cardView = CardView::create(card);
    if (cardView) {
        cardView->onClickCallback = _cardClickCallback;
        this->addChild(cardView);
        _cardViews.push_back(cardView);
    }
}

void PlayfieldView::removeCard(CardModel* card) {
    if (!card) return;
    
    for (auto it = _cardViews.begin(); it != _cardViews.end(); ++it) {
        if ((*it)->getModel() == card) {
            this->removeChild(*it);
            _cardViews.erase(it);
            break;
        }
    }
}

void PlayfieldView::updateCard(CardModel* card) {
    auto cardView = getCardViewForModel(card);
    if (cardView) {
        cardView->updateDisplay();
    }
}

void PlayfieldView::setCardClickCallback(std::function<void(CardView*)> callback) {
    _cardClickCallback = callback;
    for (auto cardView : _cardViews) {
        cardView->onClickCallback = _cardClickCallback;
    }
}

CardView* PlayfieldView::getCardViewForModel(CardModel* model) {
    for (auto cardView : _cardViews) {
        if (cardView->getModel() == model) {
            return cardView;
        }
    }
    return nullptr;
}

void PlayfieldView::clearCardViews() {
    for (auto cardView : _cardViews) {
        this->removeChild(cardView);
    }
    _cardViews.clear();
}