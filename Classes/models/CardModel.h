// Classes/models/CardModel.h
#ifndef __CARD_MODEL_H__
#define __CARD_MODEL_H__

#include "cocos2d.h"
#include "../configs/CardConfig.h"

class CardModel {
public:
    CardFaceType face;
    CardSuitType suit;
    bool isFaceUp;
    bool isRemoved;
    bool isSelectable;
    int layerIndex;
    cocos2d::Vec2 position;
    std::string cardId;
    
    CardModel() : face(CFT_NONE), suit(CST_NONE), isFaceUp(true), 
                  isRemoved(false), isSelectable(true), layerIndex(0),
                  position(cocos2d::Vec2::ZERO) {}
    
    CardModel(const CardModel& other) {
        face = other.face;
        suit = other.suit;
        isFaceUp = other.isFaceUp;
        isRemoved = other.isRemoved;
        isSelectable = other.isSelectable;
        layerIndex = other.layerIndex;
        position = other.position;
        cardId = other.cardId;
    }
    
    void reset() {
        isRemoved = false;
        isSelectable = true;
    }
    
    std::string toString() const {
        return cocos2d::StringUtils::format("Card[%s]: Face=%d, Suit=%d, Pos=(%.0f,%.0f)", 
            cardId.c_str(), face, suit, position.x, position.y);
    }
};

#endif // __CARD_MODEL_H__