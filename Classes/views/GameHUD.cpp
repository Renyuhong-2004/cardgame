// Classes/views/GameHUD.cpp
#include "GameHUD.h"

USING_NS_CC;
using namespace ui;

GameHUD* GameHUD::create() {
    GameHUD* hud = new GameHUD();
    if (hud && hud->init()) {
        hud->autorelease();
        return hud;
    }
    delete hud;
    return nullptr;
}

bool GameHUD::init() {
    if (!Node::init()) {
        return false;
    }
    
    setupUI();
    
    return true;
}

void GameHUD::setupUI() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    // 分数显示
    _scoreText = Text::create("分数: 0", "fonts/arial.ttf", 24);
    _scoreText->setPosition(Vec2(150, 1550));
    _scoreText->setTextColor(Color4B::WHITE);
    this->addChild(_scoreText);
    
    // 步数显示
    _movesText = Text::create("步数: 0", "fonts/arial.ttf", 24);
    _movesText->setPosition(Vec2(150, 1510));
    _movesText->setTextColor(Color4B::WHITE);
    this->addChild(_movesText);
    
    // 关卡显示
    _levelText = Text::create("关卡: 1", "fonts/arial.ttf", 24);
    _levelText->setPosition(Vec2(150, 1470));
    _levelText->setTextColor(Color4B::WHITE);
    this->addChild(_levelText);
    
    // 消息文本
    _messageText = Text::create("", "fonts/arial.ttf", 32);
    _messageText->setPosition(Vec2(visibleSize.width/2, 1600));
    _messageText->setTextColor(Color4B::YELLOW);
    _messageText->setVisible(false);
    this->addChild(_messageText);
    
    // 回退按钮
    _undoButton = Button::create("undo_button.png", "undo_button_pressed.png");
    if (!_undoButton) {
        _undoButton = Button::create();
        _undoButton->setTitleText("回退");
        _undoButton->setTitleFontSize(20);
    }
    _undoButton->setPosition(Vec2(900, 1550));
    _undoButton->addClickEventListener([this](Ref* sender) {
        onUndoClicked();
    });
    this->addChild(_undoButton);
    
    // 重新开始按钮
    _restartButton = Button::create("restart_button.png", "restart_button_pressed.png");
    if (!_restartButton) {
        _restartButton = Button::create();
        _restartButton->setTitleText("重玩");
        _restartButton->setTitleFontSize(20);
    }
    _restartButton->setPosition(Vec2(900, 1510));
    _restartButton->addClickEventListener([this](Ref* sender) {
        onRestartClicked();
    });
    this->addChild(_restartButton);
    
    // 菜单按钮
    _menuButton = Button::create("menu_button.png", "menu_button_pressed.png");
    if (!_menuButton) {
        _menuButton = Button::create();
        _menuButton->setTitleText("菜单");
        _menuButton->setTitleFontSize(20);
    }
    _menuButton->setPosition(Vec2(900, 1470));
    _menuButton->addClickEventListener([this](Ref* sender) {
        onMenuClicked();
    });
    this->addChild(_menuButton);
}

void GameHUD::updateScore(int score) {
    _scoreText->setString(StringUtils::format("分数: %d", score));
}

void GameHUD::updateMoves(int moves) {
    _movesText->setString(StringUtils::format("步数: %d", moves));
}

void GameHUD::updateLevel(int level) {
    _levelText->setString(StringUtils::format("关卡: %d", level));
}

void GameHUD::showMessage(const std::string& message) {
    _messageText->setString(message);
    _messageText->setVisible(true);
    
    // 3秒后自动隐藏
    this->scheduleOnce([this](float dt) {
        hideMessage();
    }, 3.0f, "hide_message");
}

void GameHUD::hideMessage() {
    _messageText->setVisible(false);
}

void GameHUD::setUndoCallback(std::function<void()> callback) {
    _undoCallback = callback;
}

void GameHUD::setRestartCallback(std::function<void()> callback) {
    _restartCallback = callback;
}

void GameHUD::setMenuCallback(std::function<void()> callback) {
    _menuCallback = callback;
}

void GameHUD::onUndoClicked() {
    if (_undoCallback) {
        _undoCallback();
    }
}

void GameHUD::onRestartClicked() {
    if (_restartCallback) {
        _restartCallback();
    }
}

void GameHUD::onMenuClicked() {
    if (_menuCallback) {
        _menuCallback();
    }
}