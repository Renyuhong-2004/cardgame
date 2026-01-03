#ifndef __GAME_VIEW_H__
#define __GAME_VIEW_H__

#include "cocos2d.h"
#include "PlayfieldView.h"
#include "StackView.h"
#include "GameHUD.h"
#include "../models/GameModel.h"

// 前向声明，避免循环依赖
class GameController;

class GameView : public cocos2d::Node {
public:
    static GameView* create(GameModel* model, GameController* controller);
    virtual bool init(GameModel* model, GameController* controller);
    
    void refresh();
    void highlightAvailableCards();
    
    void showWinMessage();
    void showGameOverMessage();
    
private:
    GameModel* _model;
    GameController* _controller;
    
    PlayfieldView* _playfieldView;
    StackView* _stackView;
    GameHUD* _hud;
    
    cocos2d::LayerColor* _playfieldBg;
    cocos2d::LayerColor* _stackBg;
    
    void setupUI();
    void onCardClicked(CardView* cardView);
    void onDrawFromStack();
    void onUndo();
    void onRestart();
    void onMenu();
};

#endif // __GAME_VIEW_H__