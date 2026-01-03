#ifndef JSON_UTILS_H
#define JSON_UTILS_H

#include <string>
#include "cocos2d.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "json/document.h"
#else
#include "third_party/json/document.h"
#endif

// JSON解析工具（无状态、通用）
class JsonUtils {
public:
    // 从文件加载JSON
    static bool loadJsonFromFile(const std::string& filePath, rapidjson::Document& doc) {
        std::string jsonStr = cocos2d::FileUtils::getInstance()->getStringFromFile(filePath);
        if (jsonStr.empty()) return false;
        doc.Parse(jsonStr.c_str());
        return !doc.HasParseError();
    }

    // 获取int值
    static int getInt(const rapidjson::Value& obj, const std::string& key, int defaultValue = 0) {
        if (obj.HasMember(key.c_str()) && obj[key.c_str()].IsInt()) {
            return obj[key.c_str()].GetInt();
        }
        return defaultValue;
    }

    // 获取float值
    static float getFloat(const rapidjson::Value& obj, const std::string& key, float defaultValue = 0.0f) {
        if (obj.HasMember(key.c_str())) {
            if (obj[key.c_str()].IsFloat()) return obj[key.c_str()].GetFloat();
            if (obj[key.c_str()].IsInt()) return static_cast<float>(obj[key.c_str()].GetInt());
        }
        return defaultValue;
    }
};

#endif // JSON_UTILS_H