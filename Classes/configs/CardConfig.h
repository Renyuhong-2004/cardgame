// Classes/configs/CardConfig.h
#ifndef __CARD_CONFIG_H__
#define __CARD_CONFIG_H__

#include "cocos2d.h"

// 花色类型
enum CardSuitType {
    CST_NONE = -1,
    CST_CLUBS,      // 梅花
    CST_DIAMONDS,   // 方块
    CST_HEARTS,     // 红桃
    CST_SPADES,     // 黑桃
    CST_NUM_CARD_SUIT_TYPES
};

// 正面类型
enum CardFaceType {
    CFT_NONE = -1,
    CFT_ACE,
    CFT_TWO,
    CFT_THREE,
    CFT_FOUR,
    CFT_FIVE,
    CFT_SIX,
    CFT_SEVEN,
    CFT_EIGHT,
    CFT_NINE,
    CFT_TEN,
    CFT_JACK,
    CFT_QUEEN,
    CFT_KING,
    CFT_NUM_CARD_FACE_TYPES
};

struct CardConfig {
    CardFaceType face;
    CardSuitType suit;
    cocos2d::Vec2 position;
    bool isFaceUp;
    
    CardConfig() : face(CFT_NONE), suit(CST_NONE), position(cocos2d::Vec2::ZERO), isFaceUp(true) {}
};

#endif // __CARD_CONFIG_H__