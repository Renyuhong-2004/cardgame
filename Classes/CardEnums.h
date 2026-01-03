#ifndef CARD_ENUMS_H
#define CARD_ENUMS_H

#include <string>

// 卡牌花色枚举
enum CardSuitType {
    CST_SPADES,    // 黑桃（黑数字）
    CST_HEARTS,    // 红桃（红数字）
    CST_DIAMONDS,  // 方块（红数字）
    CST_CLUBS,     // 梅花（黑数字）
    CST_NUM_CARD_SUIT_TYPES
};

// 卡牌面值枚举
enum CardFaceType {
    CFT_ACE,      // A
    CFT_TWO,      // 2
    CFT_THREE,    // 3
    CFT_FOUR,     // 4
    CFT_FIVE,     // 5
    CFT_SIX,      // 6
    CFT_SEVEN,    // 7
    CFT_EIGHT,    // 8
    CFT_NINE,     // 9
    CFT_TEN,      // 10
    CFT_JACK,     // J
    CFT_QUEEN,    // Q
    CFT_KING,     // K
    CFT_NUM_CARD_FACE_TYPES
};

// 花色转字符串（严格匹配suits目录下的文件名：S/H/D/C.png）
inline std::string suitToString(CardSuitType suit) {
    switch (suit) {
        case CST_SPADES: return "S";
        case CST_HEARTS: return "H";
        case CST_DIAMONDS: return "D";
        case CST_CLUBS: return "C";
        default: return "S";
    }
}

// 面值转字符串（严格匹配数字目录下的文件名：A.png/8.png）
inline std::string faceToString(CardFaceType face) {
    switch (face) {
        case CFT_ACE: return "A";
        case CFT_TWO: return "2";
        case CFT_THREE: return "3";
        case CFT_FOUR: return "4";
        case CFT_FIVE: return "5";
        case CFT_SIX: return "6";
        case CFT_SEVEN: return "7";
        case CFT_EIGHT: return "8";
        case CFT_NINE: return "9";
        case CFT_TEN: return "10";
        case CFT_JACK: return "J";
        case CFT_QUEEN: return "Q";
        case CFT_KING: return "K";
        default: return "A";
    }
}

// 判断花色对应的数字颜色（红/黑）
inline std::string getNumberColor(CardSuitType suit) {
    switch (suit) {
        case CST_HEARTS:
        case CST_DIAMONDS:
            return "red";
        case CST_SPADES:
        case CST_CLUBS:
        default:
            return "black";
    }
}

// 双色大数字路径（严格匹配：cards/numbers_big/red/A.png）
inline std::string getBigNumberPath(CardFaceType face, CardSuitType suit) {
    std::string color = getNumberColor(suit);
    return "cards/numbers_big/" + color + "/" + faceToString(face) + ".png";
}

// 双色小数字路径（严格匹配：cards/numbers_small/black/8.png）
inline std::string getSmallNumberPath(CardFaceType face, CardSuitType suit) {
    std::string color = getNumberColor(suit);
    return "cards/numbers_small/" + color + "/" + faceToString(face) + ".png";
}

// 花色路径（严格匹配：cards/suits/S.png）
inline std::string getSuitPath(CardSuitType suit) {
    return "cards/suits/" + suitToString(suit) + ".png";
}

// 卡牌背面路径（严格匹配：cards/back/card_back.png）
inline std::string getCardBackPath() {
    return "cards/back/card_back.png";
}

#endif // CARD_ENUMS_H