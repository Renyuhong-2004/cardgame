// Classes/configs/LevelConfig.h
#ifndef __LEVEL_CONFIG_H__
#define __LEVEL_CONFIG_H__

#include "CardConfig.h"
#include <vector>

struct LevelConfig {
    std::vector<CardConfig> playfieldCards;
    std::vector<CardConfig> stackCards;
    
    LevelConfig() {}
};

#endif // __LEVEL_CONFIG_H__