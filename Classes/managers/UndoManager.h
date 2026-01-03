// Classes/managers/UndoManager.h
#ifndef __UNDO_MANAGER_H__
#define __UNDO_MANAGER_H__

#include "../models/GameModel.h"
#include <stack>

struct GameState {
    struct CardState {
        std::string cardId;
        CardFaceType face;
        CardSuitType suit;
        bool isFaceUp;
        bool isRemoved;
        bool isSelectable;
        int layerIndex;
        cocos2d::Vec2 position;
    };
    
    CardState bottomCardState;
    std::vector<CardState> playfieldState;
    std::vector<CardState> stackState;
    std::vector<CardState> removedState;
    
    int score;
    int moves;
};

class UndoManager {
public:
    UndoManager(GameModel* model);
    
    void saveState();
    bool canUndo() const { return !_undoStack.empty(); }
    void undo();
    void clear();
    
private:
    GameModel* _model;
    std::stack<GameState> _undoStack;
    
    GameState::CardState createCardState(CardModel* card);
    void applyCardState(CardModel* card, const GameState::CardState& state);
};

#endif // __UNDO_MANAGER_H__