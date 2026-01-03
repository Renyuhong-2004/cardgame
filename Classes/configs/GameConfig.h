#ifndef LEVEL_CONFIG_LOADER_H
#define LEVEL_CONFIG_LOADER_H

#include "configs/models/LevelConfig.h"
#include "utils/JsonUtils.h"
// 移除不必要的 cocos2d.h 引用（避免依赖）

// 关卡配置加载器（仅负责加载解析，无业务逻辑）
class LevelConfigLoader {
public:
    // 从文件加载关卡配置
    static bool loadFromFile(const std::string& filePath, LevelConfig& outConfig) {
        rapidjson::Document doc;
        if (!JsonUtils::loadJsonFromFile(filePath, doc)) {
            // 改用原生C++输出，彻底解决日志宏兼容问题
            OutputDebugStringA(("Load level config failed: " + filePath + "\n").c_str());
            return false;
        }

        // 解析主牌区
        parseCardArray(doc, "Playfield", outConfig.playfieldCards);
        // 解析备用牌堆
        parseCardArray(doc, "Stack", outConfig.stackCards);

        return true;
    }

private:
    // 解析卡牌数组
    static void parseCardArray(const rapidjson::Document& doc, const std::string& key, 
                              std::vector<LevelCardConfig>& outCards) {
        outCards.clear();
        if (!doc.HasMember(key.c_str()) || !doc[key.c_str()].IsArray()) return;

        const auto& cardArray = doc[key.c_str()];
        for (rapidjson::SizeType i = 0; i < cardArray.Size(); ++i) {
            const auto& cardJson = cardArray[i];
            LevelCardConfig cardConfig;
            cardConfig.faceType = static_cast<CardFaceType>(JsonUtils::getInt(cardJson, "CardFace"));
            cardConfig.suitType = static_cast<CardSuitType>(JsonUtils::getInt(cardJson, "CardSuit"));
            cardConfig.position.x = JsonUtils::getFloat(cardJson["Position"], "x");
            cardConfig.position.y = JsonUtils::getFloat(cardJson["Position"], "y");
            outCards.push_back(cardConfig);
        }
    }
};

#endif // LEVEL_CONFIG_LOADER_H