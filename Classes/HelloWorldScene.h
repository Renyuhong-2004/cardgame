#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "managers/CardManager.h"
#include "models/GameModel.h"
#include "services/ConfigService.h"

class GameController;

// 新增：记录每一步操作的类型和数据
enum class MoveType {
    MAIN_CARD_MATCH,  // 主牌匹配到底牌堆
    BACKUP_CARD_DRAW  // 备用牌抽到底牌堆
};

struct MoveRecord {
    MoveType type;          // 操作类型
    CardModel* card;        // 涉及的卡牌
    CardModel* prevBottom;  // 操作前的底牌
};

class HelloWorldScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(HelloWorldScene);

    void onEnter() override;
    void onExit() override;

private:
    // 游戏核心
    GameModel* _gameModel;
    GameController* _gameController;
    CardManager* _cardManager;
    int _score;
    int _moves;

    // UI组件
    cocos2d::Layer* _mainCardArea;    // 主牌区域
    cocos2d::Layer* _bottomCardArea;  // 底牌区域
    cocos2d::Layer* _backupCardArea;  // 备用牌区域
    cocos2d::Label* _scoreLabel;
    cocos2d::Label* _movesLabel;
    cocos2d::ui::Button* _undoButton; // 回退按钮

    // 新增：精准的操作历史记录（替代原bottomCardHistory）
    std::vector<MoveRecord> _moveHistory;

    // 构造/析构
    HelloWorldScene();
    ~HelloWorldScene();

    // 初始化
    void setupResolution();
    void loadGame();
    void createGameUI();
    void updateCardDisplay();

    // 游戏逻辑
    std::string getCardDisplayName(CardModel* card);
    bool checkMatch(CardModel* mainCard);
    void handleCardClick(CardModel* card, cocos2d::Ref* sender);
    void drawNewCard();
    void restartGame();
    void updateScoreAndMoves();
    void cleanupGameObjects();
    void undoLastMove();  // 新增回退功能

    // UI辅助
    cocos2d::ui::Button* createCardButton(CardModel* card);
    // 新增：通用MoveTo动画函数
    void runCardMoveAnimation(cocos2d::ui::Button* cardBtn, cocos2d::Vec2 targetPos, float duration = 0.3f);
};

#endif // __HELLOWORLD_SCENE_H__