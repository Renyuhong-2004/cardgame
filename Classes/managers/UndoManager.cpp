// Classes/managers/UndoManager.cpp
#include "UndoManager.h"

UndoManager::UndoManager(GameModel* model) : _model(model) {
}

void UndoManager::saveState() {
    GameState state;
    
    // 保存游戏状态
    state.score = _model->score;
    state.moves = _model->moves;
    
    // 保存底牌状态
    if (_model->currentBottomCard) {
        state.bottomCardState = createCardState(_model->currentBottomCard);
    }
    
    // 保存主牌区状态
    for (auto card : _model->playfieldCards) {
        state.playfieldState.push_back(createCardState(card));
    }
    
    // 保存备用牌堆状态
    for (auto card : _model->stackCards) {
        state.stackState.push_back(createCardState(card));
    }
    
    // 保存已移除卡牌状态
    for (auto card : _model->removedCards) {
        state.removedState.push_back(createCardState(card));
    }
    
    _undoStack.push(state);
}

void UndoManager::undo() {
    if (_undoStack.empty()) {
        return;
    }
    
    GameState state = _undoStack.top();
    _undoStack.pop();
    
    // 恢复游戏状态
    _model->score = state.score;
    _model->moves = state.moves;
    
    // 清空当前状态
    _model->clear();
    
    // 恢复底牌
    if (!state.bottomCardState.cardId.empty()) {
        auto card = new CardModel();
        applyCardState(card, state.bottomCardState);
        _model->currentBottomCard = card;
    }
    
    // 恢复主牌区
    for (const auto& cardState : state.playfieldState) {
        auto card = new CardModel();
        applyCardState(card, cardState);
        _model->addToPlayfield(card);
    }
    
    // 恢复备用牌堆
    for (const auto& cardState : state.stackState) {
        auto card = new CardModel();
        applyCardState(card, cardState);
        _model->addToStack(card);
    }
    
    // 恢复已移除卡牌
    for (const auto& cardState : state.removedState) {
        auto card = new CardModel();
        applyCardState(card, cardState);
        _model->removedCards.push_back(card);
    }
}

void UndoManager::clear() {
    while (!_undoStack.empty()) {
        _undoStack.pop();
    }
}

GameState::CardState UndoManager::createCardState(CardModel* card) {
    GameState::CardState state;
    
    if (!card) return state;
    
    state.cardId = card->cardId;
    state.face = card->face;
    state.suit = card->suit;
    state.isFaceUp = card->isFaceUp;
    state.isRemoved = card->isRemoved;
    state.isSelectable = card->isSelectable;
    state.layerIndex = card->layerIndex;
    state.position = card->position;
    
    return state;
}

void UndoManager::applyCardState(CardModel* card, const GameState::CardState& state) {
    if (!card) return;
    
    card->cardId = state.cardId;
    card->face = state.face;
    card->suit = state.suit;
    card->isFaceUp = state.isFaceUp;
    card->isRemoved = state.isRemoved;
    card->isSelectable = state.isSelectable;
    card->layerIndex = state.layerIndex;
    card->position = state.position;
}