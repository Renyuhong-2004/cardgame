#ifndef __GAME_CONTROLLER_H__
#define __GAME_CONTROLLER_H__

#include "../models/GameModel.h"
#include "../managers/CardManager.h"
#include "../managers/GameManager.h"
#include "../managers/UndoManager.h"
#include <functional>

// 前向声明
class CardManager;
class GameManager;
class UndoManager;

class GameController {
public:
    GameController(GameModel* model);
    ~GameController();
    
    void setCardManager(CardManager* manager) { _cardManager = manager; }
    void setGameManager(GameManager* manager) { _gameManager = manager; }
    void setUndoManager(UndoManager* manager) { _undoManager = manager; }
    
    CardManager* getCardManager() { return _cardManager; }
    GameManager* getGameManager() { return _gameManager; }
    UndoManager* getUndoManager() { return _undoManager; }
    
    void onCardClicked(CardModel* card);
    void onDrawFromStack();
    void onUndo();
    void onRestart();
    
    bool checkMatch(CardModel* card1, CardModel* card2);
    
private:
    GameModel* _model;
    CardManager* _cardManager;
    GameManager* _gameManager;
    UndoManager* _undoManager;
};

#endif // __GAME_CONTROLLER_H__