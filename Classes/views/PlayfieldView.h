// Classes/views/PlayfieldView.h
#ifndef __PLAYFIELD_VIEW_H__
#define __PLAYFIELD_VIEW_H__

#include "cocos2d.h"
#include "CardView.h"
#include <vector>

class PlayfieldView : public cocos2d::Node {
public:
    static PlayfieldView* create();
    virtual bool init();
    
    void setCards(const std::vector<CardModel*>& cards);
    void addCard(CardModel* card);
    void removeCard(CardModel* card);
    void updateCard(CardModel* card);
    
    void setCardClickCallback(std::function<void(CardView*)> callback);
    
    CardView* getCardViewForModel(CardModel* model);
    
private:
    std::vector<CardView*> _cardViews;
    std::function<void(CardView*)> _cardClickCallback;
    
    void clearCardViews();
};

#endif // __PLAYFIELD_VIEW_H__