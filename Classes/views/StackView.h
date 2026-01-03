#ifndef __STACK_VIEW_H__
#define __STACK_VIEW_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"  // 添加这一行
#include "CardView.h"
#include <vector>

class StackView : public cocos2d::Node {
public:
    static StackView* create();
    virtual bool init();
    
    void setCards(const std::vector<CardModel*>& cards);
    void setBottomCard(CardModel* card);
    
    void setDrawCallback(std::function<void()> callback);
    void setCardClickCallback(std::function<void(CardView*)> callback);
    
    void updateBottomCard();
    void updateStack();
    
private:
    std::vector<CardModel*> _stackCards;
    CardModel* _bottomCard;
    
    CardView* _bottomCardView;
    cocos2d::Sprite* _stackBackView;
    cocos2d::ui::Button* _drawButton;
    
    std::function<void()> _drawCallback;
    std::function<void(CardView*)> _cardClickCallback;
    
    void setupUI();
    void onDrawButtonClicked();
};

#endif // __STACK_VIEW_H__