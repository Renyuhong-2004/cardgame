#include "ConfigService.h"
#include "cocos2d.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"

USING_NS_CC;

LevelConfig ConfigService::loadLevelConfig(const std::string& filename) {
    LevelConfig config;
    
    // 尝试加载JSON文件，失败则用默认数据
    std::string path = FileUtils::getInstance()->fullPathForFilename(filename);
    if (FileUtils::getInstance()->isFileExist(path)) {
        std::string content = FileUtils::getInstance()->getStringFromFile(path);
        rapidjson::Document doc;
        if (!content.empty() && !doc.Parse<0>(content.c_str()).HasParseError()) {
            // 解析主牌堆
            if (doc.HasMember("playfieldCards") && doc["playfieldCards"].IsArray()) {
                const rapidjson::Value& cards = doc["playfieldCards"];
                for (rapidjson::SizeType i = 0; i < cards.Size(); ++i) {
                    const rapidjson::Value& cardJson = cards[i];
                    CardConfig cardConfig;
                    
                    if (cardJson.HasMember("face")) cardConfig.face = (CardFaceType)cardJson["face"].GetInt();
                    if (cardJson.HasMember("suit")) cardConfig.suit = (CardSuitType)cardJson["suit"].GetInt();
                    if (cardJson.HasMember("x") && cardJson.HasMember("y")) {
                        cardConfig.position = Vec2(cardJson["x"].GetFloat(), cardJson["y"].GetFloat());
                    }
                    if (cardJson.HasMember("isFaceUp")) cardConfig.isFaceUp = cardJson["isFaceUp"].GetBool();
                    
                    config.playfieldCards.push_back(cardConfig);
                }
            }
            
            // 解析备用牌堆
            if (doc.HasMember("stackCards") && doc["stackCards"].IsArray()) {
                const rapidjson::Value& stacks = doc["stackCards"];
                for (rapidjson::SizeType i = 0; i < stacks.Size(); ++i) {
                    const rapidjson::Value& stackJson = stacks[i];
                    CardConfig stackConfig;
                    
                    if (stackJson.HasMember("face")) stackConfig.face = (CardFaceType)stackJson["face"].GetInt();
                    if (stackJson.HasMember("suit")) stackConfig.suit = (CardSuitType)stackJson["suit"].GetInt();
                    
                    config.stackCards.push_back(stackConfig);
                }
            }
            
            CCLOG("Loaded config from: %s", filename.c_str());
            return config;
        }
    }
    
    // 默认测试数据
    CCLOG("Using default config for: %s", filename.c_str());
    // 主牌堆（6张）
    CardConfig card1{CFT_EIGHT, CST_SPADES, Vec2(100, 1000), false};
    CardConfig card2{CFT_NINE, CST_HEARTS, Vec2(200, 1000), false};
    CardConfig card3{CFT_SEVEN, CST_DIAMONDS, Vec2(300, 1000), false};
    CardConfig card4{CFT_TEN, CST_CLUBS, Vec2(400, 1000), false};
    CardConfig card5{CFT_JACK, CST_SPADES, Vec2(500, 1000), false};
    CardConfig card6{CFT_QUEEN, CST_HEARTS, Vec2(600, 1000), false};
    config.playfieldCards = {card1, card2, card3, card4, card5, card6};
    
    // 备用牌堆（5张）
    CardConfig stack1{CFT_EIGHT, CST_CLUBS, Vec2::ZERO, false};
    CardConfig stack2{CFT_KING, CST_DIAMONDS, Vec2::ZERO, false};
    CardConfig stack3{CFT_ACE, CST_SPADES, Vec2::ZERO, false};
    CardConfig stack4{CFT_TWO, CST_HEARTS, Vec2::ZERO, false};
    CardConfig stack5{CFT_THREE, CST_CLUBS, Vec2::ZERO, false};
    config.stackCards = {stack1, stack2, stack3, stack4, stack5};
    
    return config;
}

bool ConfigService::saveLevelConfig(const LevelConfig& config, const std::string& filename) {
    rapidjson::Document doc;
    doc.SetObject();
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
    
    // 保存主牌堆
    rapidjson::Value playfieldCards(rapidjson::kArrayType);
    for (const auto& card : config.playfieldCards) {
        rapidjson::Value cardObj(rapidjson::kObjectType);
        cardObj.AddMember("face", card.face, allocator);
        cardObj.AddMember("suit", card.suit, allocator);
        cardObj.AddMember("x", card.position.x, allocator);
        cardObj.AddMember("y", card.position.y, allocator);
        cardObj.AddMember("isFaceUp", card.isFaceUp, allocator);
        playfieldCards.PushBack(cardObj, allocator);
    }
    doc.AddMember("playfieldCards", playfieldCards, allocator);
    
    // 保存备用牌堆
    rapidjson::Value stackCards(rapidjson::kArrayType);
    for (const auto& stack : config.stackCards) {
        rapidjson::Value stackObj(rapidjson::kObjectType);
        stackObj.AddMember("face", stack.face, allocator);
        stackObj.AddMember("suit", stack.suit, allocator);
        stackCards.PushBack(stackObj, allocator);
    }
    doc.AddMember("stackCards", stackCards, allocator);
    
    // 写入文件
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);
    
    std::string writePath = FileUtils::getInstance()->getWritablePath() + filename;
    bool result = FileUtils::getInstance()->writeStringToFile(buffer.GetString(), writePath);
    CCLOG("Saved config to: %s (result: %d)", writePath.c_str(), result);
    
    return result;
}

CardConfig ConfigService::parseCardConfig(const std::string& jsonStr) {
    CardConfig config;
    rapidjson::Document doc;
    if (!jsonStr.empty() && !doc.Parse<0>(jsonStr.c_str()).HasParseError()) {
        if (doc.HasMember("face")) config.face = (CardFaceType)doc["face"].GetInt();
        if (doc.HasMember("suit")) config.suit = (CardSuitType)doc["suit"].GetInt();
        if (doc.HasMember("x") && doc.HasMember("y")) {
            config.position = Vec2(doc["x"].GetFloat(), doc["y"].GetFloat());
        }
        if (doc.HasMember("isFaceUp")) config.isFaceUp = doc["isFaceUp"].GetBool();
    }
    return config;
}

std::string ConfigService::serializeCardConfig(const CardConfig& config) {
    rapidjson::Document doc;
    doc.SetObject();
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
    
    doc.AddMember("face", config.face, allocator);
    doc.AddMember("suit", config.suit, allocator);
    doc.AddMember("x", config.position.x, allocator);
    doc.AddMember("y", config.position.y, allocator);
    doc.AddMember("isFaceUp", config.isFaceUp, allocator);
    
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);
    
    return buffer.GetString();
}