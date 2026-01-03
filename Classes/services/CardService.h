// Classes/services/CardService.h
#ifndef __CARD_SERVICE_H__
#define __CARD_SERVICE_H__

#include "../configs/CardConfig.h"

class CardService {
public:
    static bool isValidMatch(CardFaceType face1, CardFaceType face2);
    static int getCardValue(CardFaceType face);
    static bool isAdjacent(int value1, int value2);
    
    // 扩展：支持不同规则
    enum RuleType {
        RULE_ADJACENT_NUMBERS,    // 相邻数字
        RULE_SAME_SUIT,           // 同花色
        RULE_ALTERNATE_COLORS,    // 交替颜色
        RULE_CUSTOM               // 自定义规则
    };
    
    static bool checkRule(CardFaceType face1, CardSuitType suit1, 
                         CardFaceType face2, CardSuitType suit2, 
                         RuleType rule);
    
private:
    CardService() {}
    ~CardService() {}
};

#endif // __CARD_SERVICE_H__