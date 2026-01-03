#include "GameController.h"
#include "../services/CardService.h"

GameController::GameController(GameModel* model) : _model(model) {
    _cardManager = nullptr;
    _gameManager = nullptr;
    _undoManager = nullptr;
}

GameController::~GameController() {
    // 注意：控制器不负责删除管理器，由外层管理
}

void GameController::onCardClicked(CardModel* card) {
    if (!_model || !card || card->isRemoved || !card->isFaceUp) {
        return;
    }
    
    if (!_model->currentBottomCard) {
        // 如果没有底牌，直接设置为底牌
        _undoManager->saveState();
        _model->currentBottomCard = card;
        _model->moveToRemoved(card);
        return;
    }
    
    // 检查是否匹配
    if (checkMatch(_model->currentBottomCard, card)) {
        // 保存状态
        _undoManager->saveState();
        
        // 执行消除
        _model->moveToRemoved(card);
        
        // 更新底牌
        auto oldBottom = _model->currentBottomCard;
        _model->currentBottomCard = card;
        
        // 将旧底牌移回主牌区（如果需要）
        if (oldBottom->isRemoved) {
            oldBottom->isRemoved = false;
            _model->addToPlayfield(oldBottom);
        }
        
        // 更新分数和步数
        _model->score += 10;
        _model->moves++;
        
        // 检查游戏胜利
        if (_gameManager) {
            _model->isGameWon = _gameManager->checkWinCondition();
        }
        
        // 检查游戏结束
        if (!_model->hasAvailableMoves()) {
            _model->isGameOver = true;
        }
    }
}

void GameController::onDrawFromStack() {
    if (!_model || _model->stackCards.empty()) {
        return;
    }
    
    // 保存状态
    _undoManager->saveState();
    
    // 从备用牌堆抽取
    auto newCard = _model->stackCards.back();
    _model->stackCards.pop_back();
    
    // 设置新底牌
    newCard->isFaceUp = true;
    
    // 如果有旧底牌，放回主牌区
    if (_model->currentBottomCard) {
        auto oldBottom = _model->currentBottomCard;
        oldBottom->isRemoved = false;
        _model->addToPlayfield(oldBottom);
    }
    
    _model->currentBottomCard = newCard;
    
    // 更新步数
    _model->moves++;
}

void GameController::onUndo() {
    if (_undoManager) {
        _undoManager->undo();
        
        // 更新游戏状态
        if (_gameManager) {
            _model->isGameWon = _gameManager->checkWinCondition();
            _model->isGameOver = !_model->hasAvailableMoves() && _model->stackCards.empty();
        }
    }
}

void GameController::onRestart() {
    // 重新加载当前关卡
    if (_gameManager) {
        _gameManager->restartLevel();
    }
}

bool GameController::checkMatch(CardModel* card1, CardModel* card2) {
    if (!card1 || !card2) return false;
    return CardService::isValidMatch(card1->face, card2->face);
}