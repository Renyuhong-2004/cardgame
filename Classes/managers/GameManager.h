// Classes/managers/GameManager.h
#ifndef __GAME_MANAGER_H__
#define __GAME_MANAGER_H__

#include "../models/GameModel.h"
#include "../configs/LevelConfig.h"

class GameManager {
public:
    GameManager(GameModel* model);
    
    bool checkWinCondition();
    bool checkLoseCondition();
    
    void loadLevel(const LevelConfig& config);
    void restartLevel();
    void nextLevel();
    
    void saveGame();
    bool loadGame();
    
private:
    GameModel* _model;
    LevelConfig _currentLevelConfig;
};

#endif // __GAME_MANAGER_H__