// Classes/views/StackView.cpp
#include "StackView.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace ui;

StackView* StackView::create() {
    StackView* view = new StackView();
    if (view && view->init()) {
        view->autorelease();
        return view;
    }
    delete view;
    return nullptr;
}

bool StackView::init() {
    if (!Node::init()) {
        return false;
    }
    
    // 设置堆牌区大小
    this->setContentSize(Size(1080, 580));
    
    setupUI();
    
    return true;
}

void StackView::setupUI() {
    // 创建堆牌背景
    _stackBackView = Sprite::create("stack_back.png");
    if (!_stackBackView) {
        _stackBackView = Sprite::create();
        auto drawNode = DrawNode::create();
        drawNode->drawRect(Vec2(0, 0), Vec2(100, 140), Color4F(0.2f, 0.2f, 0.8f, 0.5f));
        _stackBackView->addChild(drawNode);
    }
    _stackBackView->setPosition(Vec2(200, 290));
    this->addChild(_stackBackView);
    
    // 创建抽牌按钮
    _drawButton = Button::create("draw_button.png", "draw_button_pressed.png");
    if (!_drawButton) {
        _drawButton = Button::create();
        _drawButton->setTitleText("抽牌");
        _drawButton->setTitleFontSize(24);
    }
    _drawButton->setPosition(Vec2(200, 290));
    _drawButton->addClickEventListener([this](Ref* sender) {
        onDrawButtonClicked();
    });
    this->addChild(_drawButton);
    
    // 创建底牌位置
    _bottomCardView = nullptr;
}

void StackView::setCards(const std::vector<CardModel*>& cards) {
    _stackCards = cards;
    updateStack();
}

void StackView::setBottomCard(CardModel* card) {
    _bottomCard = card;
    updateBottomCard();
}

void StackView::updateBottomCard() {
    if (_bottomCardView) {
        this->removeChild(_bottomCardView);
        _bottomCardView = nullptr;
    }
    
    if (_bottomCard) {
        _bottomCardView = CardView::create(_bottomCard);
        if (_bottomCardView) {
            _bottomCardView->setPosition(Vec2(540, 290));
            _bottomCardView->onClickCallback = _cardClickCallback;
            this->addChild(_bottomCardView);
        }
    }
}

void StackView::updateStack() {
    // 更新抽牌按钮状态
    _drawButton->setEnabled(!_stackCards.empty());
    
    // 更新堆牌显示数量
    if (_stackBackView) {
        std::string countText = StringUtils::format("剩余: %zd", _stackCards.size());
        // 这里可以添加文本显示
    }
}

void StackView::setDrawCallback(std::function<void()> callback) {
    _drawCallback = callback;
}

void StackView::setCardClickCallback(std::function<void(CardView*)> callback) {
    _cardClickCallback = callback;
    if (_bottomCardView) {
        _bottomCardView->onClickCallback = _cardClickCallback;
    }
}

void StackView::onDrawButtonClicked() {
    if (_drawCallback) {
        _drawCallback();
    }
}