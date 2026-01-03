// Classes/views/CardView.cpp
#include "CardView.h"
#include "../services/CardService.h"
#include <string>

USING_NS_CC;

CardView* CardView::create(CardModel* model) {
    CardView* view = new CardView();
    if (view && view->init(model)) {
        view->autorelease();
        return view;
    }
    delete view;
    return nullptr;
}

bool CardView::init(CardModel* model) {
    if (!Sprite::init()) {
        return false;
    }
    
    _model = model;
    _isSelected = false;
    _isHighlighted = false;
    
    // 设置大小
    this->setContentSize(Size(100, 140));
    
    // 更新显示
    updateDisplay();
    
    // 启用触摸
    this->setTouchEnabled(true);
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this](Touch* touch, Event* event) {
        Vec2 locationInNode = this->convertToNodeSpace(touch->getLocation());
        Size s = this->getContentSize();
        Rect rect = Rect(0, 0, s.width, s.height);
        
        if (rect.containsPoint(locationInNode) && _model && !_model->isRemoved) {
            return true;
        }
        return false;
    };
    
    listener->onTouchEnded = [this](Touch* touch, Event* event) {
        if (onClickCallback) {
            onClickCallback(this);
        }
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}

void CardView::updateDisplay() {
    if (!_model) return;
    
    std::string textureName;
    
    if (_model->isRemoved) {
        this->setVisible(false);
        return;
    }
    
    this->setVisible(true);
    
    if (_model->isFaceUp) {
        // 使用数字和花色组合文件名
        // 假设资源命名规则: card_{face}_{suit}.png
        textureName = StringUtils::format("card_%d_%d.png", _model->face, _model->suit);
    } else {
        textureName = "card_back.png";
    }
    
    auto texture = Director::getInstance()->getTextureCache()->addImage(textureName);
    if (texture) {
        this->setTexture(texture);
        this->setTextureRect(Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height));
    }
    
    // 设置位置
    this->setPosition(_model->position);
    this->setLocalZOrder(_model->layerIndex);
    
    // 更新视觉状态
    updateVisualState();
}

void CardView::setSelected(bool selected) {
    _isSelected = selected;
    updateVisualState();
}

void CardView::setHighlighted(bool highlighted) {
    _isHighlighted = highlighted;
    updateVisualState();
}

void CardView::updateVisualState() {
    if (_isSelected) {
        this->setColor(Color3B::YELLOW);
        this->setScale(1.1f);
    } else if (_isHighlighted) {
        this->setColor(Color3B(200, 200, 255));
        this->setScale(1.05f);
    } else {
        this->setColor(Color3B::WHITE);
        this->setScale(1.0f);
    }
}