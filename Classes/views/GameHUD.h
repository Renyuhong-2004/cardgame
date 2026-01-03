// Classes/views/GameHUD.h
#ifndef __GAME_HUD_H__
#define __GAME_HUD_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class GameHUD : public cocos2d::Node {
public:
    static GameHUD* create();
    virtual bool init();
    
    void updateScore(int score);
    void updateMoves(int moves);
    void updateLevel(int level);
    
    void setUndoCallback(std::function<void()> callback);
    void setRestartCallback(std::function<void()> callback);
    void setMenuCallback(std::function<void()> callback);
    
    void showMessage(const std::string& message);
    void hideMessage();
    
private:
    cocos2d::ui::Text* _scoreText;
    cocos2d::ui::Text* _movesText;
    cocos2d::ui::Text* _levelText;
    cocos2d::ui::Text* _messageText;
    
    cocos2d::ui::Button* _undoButton;
    cocos2d::ui::Button* _restartButton;
    cocos2d::ui::Button* _menuButton;
    
    std::function<void()> _undoCallback;
    std::function<void()> _restartCallback;
    std::function<void()> _menuCallback;
    
    void setupUI();
    void onUndoClicked();
    void onRestartClicked();
    void onMenuClicked();
};

#endif // __GAME_HUD_H__