#include "CardService.h"

bool CardService::isValidMatch(CardFaceType face1, CardFaceType face2) {
    int value1 = getCardValue(face1);
    int value2 = getCardValue(face2);
    
    return isAdjacent(value1, value2);
}

int CardService::getCardValue(CardFaceType face) {
    switch (face) {
        case CFT_ACE: return 1;
        case CFT_TWO: return 2;
        case CFT_THREE: return 3;
        case CFT_FOUR: return 4;
        case CFT_FIVE: return 5;
        case CFT_SIX: return 6;
        case CFT_SEVEN: return 7;
        case CFT_EIGHT: return 8;
        case CFT_NINE: return 9;
        case CFT_TEN: return 10;
        case CFT_JACK: return 11;
        case CFT_QUEEN: return 12;
        case CFT_KING: return 13;
        default: return 0;
    }
}

bool CardService::isAdjacent(int value1, int value2) {
    // 处理A和K的特殊情况（循环）
    if (value1 == 1 && value2 == 13) return true;
    if (value1 == 13 && value2 == 1) return true;
    
    return abs(value1 - value2) == 1;
}

bool CardService::checkRule(CardFaceType face1, CardSuitType suit1,
                           CardFaceType face2, CardSuitType suit2,
                           RuleType rule) {
    switch (rule) {
        case RULE_ADJACENT_NUMBERS:
            return isValidMatch(face1, face2);
            
        case RULE_SAME_SUIT:
            return suit1 == suit2;
            
        case RULE_ALTERNATE_COLORS:
            // 梅花和黑桃是黑色(0,3)，方块和红桃是红色(1,2)
            bool isBlack1 = (suit1 == CST_CLUBS || suit1 == CST_SPADES);
            bool isBlack2 = (suit2 == CST_CLUBS || suit2 == CST_SPADES);
            return isBlack1 != isBlack2;
            
        case RULE_CUSTOM:
            // 自定义规则可以在这里扩展
            return false;
            
        default:
            return false;
    }
}