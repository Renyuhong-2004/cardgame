#pragma once
#include <string>
#include <vector>
#include "CardEnums.h"
#include "cocos2d.h"

// 卡牌配置结构体
struct CardConfig {
    CardFaceType face = CFT_ACE;
    CardSuitType suit = CST_SPADES;
    cocos2d::Vec2 position = cocos2d::Vec2::ZERO;
    bool isFaceUp = false;
};

// 关卡配置结构体
struct LevelConfig {
    std::vector<CardConfig> playfieldCards; // 主牌堆配置
    std::vector<CardConfig> stackCards;     // 备用牌堆配置
};

class ConfigService {
public:
    LevelConfig loadLevelConfig(const std::string& filename);
    bool saveLevelConfig(const LevelConfig& config, const std::string& filename);
    CardConfig parseCardConfig(const std::string& jsonStr);
    std::string serializeCardConfig(const CardConfig& config);
};