// Classes/views/CardView.h
#ifndef __CARD_VIEW_H__
#define __CARD_VIEW_H__

#include "cocos2d.h"
#include "../models/CardModel.h"

class CardView : public cocos2d::Sprite {
public:
    static CardView* create(CardModel* model);
    virtual bool init(CardModel* model);
    
    void updateDisplay();
    void setSelected(bool selected);
    void setHighlighted(bool highlighted);
    
    CardModel* getModel() { return _model; }
    void setModel(CardModel* model) { _model = model; }
    
    std::function<void(CardView*)> onClickCallback;
    
private:
    CardModel* _model;
    bool _isSelected;
    bool _isHighlighted;
    
    void onTouched();
    void updateVisualState();
};

#endif // __CARD_VIEW_H__