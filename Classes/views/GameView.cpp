#include "GameView.h"
#include "../controllers/GameController.h"  // 在这里包含

USING_NS_CC;

GameView* GameView::create(GameModel* model, GameController* controller) {
    GameView* view = new GameView();
    if (view && view->init(model, controller)) {
        view->autorelease();
        return view;
    }
    delete view;
    return nullptr;
}

bool GameView::init(GameModel* model, GameController* controller) {
    if (!Node::init()) {
        return false;
    }
    
    _model = model;
    _controller = controller;
    
    setupUI();
    refresh();
    
    return true;
}

void GameView::setupUI() {
    // 创建主牌区背景
    _playfieldBg = LayerColor::create(Color4B(50, 100, 50, 255), 1080, 1500);
    _playfieldBg->setPosition(Vec2(0, 580));
    this->addChild(_playfieldBg);
    
    // 创建备用牌区背景
    _stackBg = LayerColor::create(Color4B(100, 50, 50, 255), 1080, 580);
    _stackBg->setPosition(Vec2(0, 0));
    this->addChild(_stackBg);
    
    // 创建主牌区视图
    _playfieldView = PlayfieldView::create();
    _playfieldView->setPosition(Vec2(0, 580));
    _playfieldView->setCardClickCallback([this](CardView* cardView) {
        onCardClicked(cardView);
    });
    this->addChild(_playfieldView);
    
    // 创建堆牌区视图
    _stackView = StackView::create();
    _stackView->setPosition(Vec2(0, 0));
    _stackView->setDrawCallback([this]() {
        onDrawFromStack();
    });
    _stackView->setCardClickCallback([this](CardView* cardView) {
        onCardClicked(cardView);
    });
    this->addChild(_stackView);
    
    // 创建HUD
    _hud = GameHUD::create();
    _hud->setUndoCallback([this]() {
        onUndo();
    });
    _hud->setRestartCallback([this]() {
        onRestart();
    });
    _hud->setMenuCallback([this]() {
        onMenu();
    });
    this->addChild(_hud);
}

void GameView::refresh() {
    if (!_model) return;
    
    // 更新主牌区
    _playfieldView->setCards(_model->playfieldCards);
    
    // 更新堆牌区
    _stackView->setCards(_model->stackCards);
    _stackView->setBottomCard(_model->currentBottomCard);
    
    // 更新HUD
    _hud->updateScore(_model->score);
    _hud->updateMoves(_model->moves);
    _hud->updateLevel(_model->level);
    
    // 高亮可用卡牌
    highlightAvailableCards();
    
    // 检查游戏状态
    if (_model->isGameWon) {
        showWinMessage();
    } else if (_model->isGameOver) {
        showGameOverMessage();
    }
}

void GameView::highlightAvailableCards() {
    if (!_model) return;
    
    auto availableCards = _model->getAvailableCards();
    
    // 首先重置所有卡牌高亮
    for (auto cardModel : _model->playfieldCards) {
        auto cardView = _playfieldView->getCardViewForModel(cardModel);
        if (cardView) {
            cardView->setHighlighted(false);
        }
    }
    
    // 高亮可用卡牌
    for (auto cardModel : availableCards) {
        auto cardView = _playfieldView->getCardViewForModel(cardModel);
        if (cardView) {
            cardView->setHighlighted(true);
        }
    }
}

void GameView::onCardClicked(CardView* cardView) {
    if (!_controller || !cardView) return;
    
    _controller->onCardClicked(cardView->getModel());
    refresh();
}

void GameView::onDrawFromStack() {
    if (!_controller) return;
    
    _controller->onDrawFromStack();
    refresh();
}

void GameView::onUndo() {
    if (!_controller) return;
    
    _controller->onUndo();
    refresh();
}

void GameView::onRestart() {
    if (!_controller) return;
    
    _controller->onRestart();
    refresh();
}

void GameView::onMenu() {
    // 返回主菜单
    Director::getInstance()->popScene();
}

void GameView::showWinMessage() {
    _hud->showMessage("恭喜！你赢了！");
}

void GameView::showGameOverMessage() {
    _hud->showMessage("游戏结束！");
}