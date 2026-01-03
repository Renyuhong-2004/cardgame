#include "HelloWorldScene.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <cmath>
#include <algorithm>
#include "CardEnums.h"

USING_NS_CC;
using namespace cocos2d::ui;

HelloWorldScene::HelloWorldScene() :
    _gameModel(nullptr),
    _gameController(nullptr),
    _cardManager(nullptr),
    _score(0),
    _moves(0),
    _mainCardArea(nullptr),
    _bottomCardArea(nullptr),
    _backupCardArea(nullptr),
    _scoreLabel(nullptr),
    _movesLabel(nullptr),
    _undoButton(nullptr) {
}

HelloWorldScene::~HelloWorldScene() {
    cleanupGameObjects();
}

Scene* HelloWorldScene::createScene() {
    return HelloWorldScene::create();
}

bool HelloWorldScene::init() {
    if (!Scene::init()) {
        return false;
    }

    setupResolution();
    loadGame();

    return true;
}

void HelloWorldScene::setupResolution() {
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();

    if (!glview) {
        glview = GLViewImpl::createWithRect("CardGame",
            Rect(0, 0, 1080, 1600), 2.0f);
        director->setOpenGLView(glview);
    }

    glview->setDesignResolutionSize(1080, 2080, ResolutionPolicy::FIXED_WIDTH);
    director->setDisplayStats(true);
    director->setAnimationInterval(1.0f / 60);
}

void HelloWorldScene::loadGame() {
    CCLOG("Loading game...");

    // 预加载资源
    Director::getInstance()->getTextureCache()->addImage(getCardBackPath());
    Director::getInstance()->getTextureCache()->addImage("cards/back/card_front_bg.png");

    std::vector<CardFaceType> faces = { CFT_ACE, CFT_EIGHT, CFT_NINE, CFT_TEN, CFT_KING, CFT_JACK, CFT_QUEEN };
    std::vector<std::string> colors = { "red", "black" };
    for (auto face : faces) {
        for (auto color : colors) {
            std::string bigNumPath = "cards/numbers_big/" + color + "/" + faceToString(face) + ".png";
            Director::getInstance()->getTextureCache()->addImage(bigNumPath);
            std::string smallNumPath = "cards/numbers_small/" + color + "/" + faceToString(face) + ".png";
            Director::getInstance()->getTextureCache()->addImage(smallNumPath);
        }
    }

    std::vector<CardSuitType> suits = { CST_SPADES, CST_HEARTS, CST_DIAMONDS, CST_CLUBS };
    for (auto suit : suits) {
        Director::getInstance()->getTextureCache()->addImage(getSuitPath(suit));
    }

    // 初始化核心组件
    _gameModel = new GameModel();
    _gameController = new GameController(_gameModel);
    _cardManager = new CardManager(_gameModel);

    ConfigService configService;
    auto levelConfig = configService.loadLevelConfig("level1.json");

    // 初始化主牌堆
    for (int i = 0; i < levelConfig.playfieldCards.size(); i++) {
        auto card = _cardManager->createCard(levelConfig.playfieldCards[i]);
        if (i < 3) {
            card->isFaceUp = true;
            _gameModel->mainCards.push_back(card);
        }
        else {
            card->isFaceUp = false;
            _gameModel->coveredCards.push_back(card);
        }
    }

    // 初始化备用牌堆
    for (const auto& stackCard : levelConfig.stackCards) {
        auto card = _cardManager->createCard(stackCard);
        card->isFaceUp = false;
        _gameModel->backupCards.push_back(card);
    }

    // 初始化底牌
    _moveHistory.clear();
    if (!_gameModel->backupCards.empty()) {
        _gameModel->bottomCard = _gameModel->backupCards.back();
        _gameModel->backupCards.pop_back();
        _gameModel->bottomCard->isFaceUp = true;
    }

    createGameUI();
    updateCardDisplay();
}

void HelloWorldScene::createGameUI() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    // 背景
    auto background = LayerColor::create(Color4B(50, 100, 50, 255));
    this->addChild(background, 0);

    // 标题
    auto title = Label::createWithTTF("Card Elimination Game", "fonts/arial.ttf", 60);
    title->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 80));
    title->setColor(Color3B::YELLOW);
    this->addChild(title, 1);

    // 主牌区
    _mainCardArea = LayerColor::create(Color4B(40, 80, 40, 255), 1080, 1500);
    _mainCardArea->setPosition(Vec2(0, 580));
    this->addChild(_mainCardArea, 1);

    // 堆牌区容器
    auto stackArea = LayerColor::create(Color4B(30, 70, 30, 255), 1080, 580);
    stackArea->setPosition(Vec2(0, 0));
    this->addChild(stackArea, 1);

    // 底牌区域
    _bottomCardArea = LayerColor::create(Color4B(60, 120, 60, 255), 200, 280);
    _bottomCardArea->setPosition(Vec2(visibleSize.width / 2 - 100, 150));
    stackArea->addChild(_bottomCardArea, 2);

    // 备用牌区域
    _backupCardArea = LayerColor::create(Color4B(60, 120, 60, 255), 200, 280);
    _backupCardArea->setPosition(Vec2(visibleSize.width / 2 - 350, 150));
    stackArea->addChild(_backupCardArea, 2);

    // 分数
    _scoreLabel = Label::createWithTTF("Score: 0", "fonts/arial.ttf", 40);
    _scoreLabel->setPosition(Vec2(visibleSize.width - 120, visibleSize.height - 80));
    _scoreLabel->setColor(Color3B::WHITE);
    this->addChild(_scoreLabel, 2);

    // 步数
    _movesLabel = Label::createWithTTF("Moves: 0", "fonts/arial.ttf", 40);
    _movesLabel->setPosition(Vec2(visibleSize.width - 120, visibleSize.height - 140));
    _movesLabel->setColor(Color3B::WHITE);
    this->addChild(_movesLabel, 2);

    // 回退按钮（Draw按钮上方）
    _undoButton = Button::create();
    _undoButton->setTitleText("Undo");
    _undoButton->setTitleFontSize(30);
    _undoButton->setTitleColor(Color3B::WHITE);
    _undoButton->setColor(Color3B(100, 100, 200));
    _undoButton->setScale9Enabled(true);
    _undoButton->setContentSize(Size(180, 80));
    _undoButton->setPosition(Vec2(visibleSize.width / 2 + 200, 300));
    _undoButton->setEnabled(false);

    _undoButton->addTouchEventListener([this](Ref* sender, Widget::TouchEventType type) {
        if (type == Widget::TouchEventType::ENDED) {
            this->undoLastMove();
        }
        });
    stackArea->addChild(_undoButton, 2);

    // 抽备用牌按钮
    auto drawBtn = Button::create();
    drawBtn->setTitleText("Draw Card");
    drawBtn->setTitleFontSize(30);
    drawBtn->setTitleColor(Color3B::WHITE);
    drawBtn->setColor(Color3B(0, 100, 200));
    drawBtn->setScale9Enabled(true);
    drawBtn->setContentSize(Size(180, 80));
    drawBtn->setPosition(Vec2(visibleSize.width / 2 + 200, 200));

    drawBtn->addTouchEventListener([this](Ref* sender, Widget::TouchEventType type) {
        if (type == Widget::TouchEventType::ENDED) {
            this->drawNewCard();
        }
        });
    stackArea->addChild(drawBtn, 2);

    // 重新开始按钮
    auto restartBtn = Button::create();
    restartBtn->setTitleText("Restart");
    restartBtn->setTitleFontSize(30);
    restartBtn->setTitleColor(Color3B::WHITE);
    restartBtn->setColor(Color3B(200, 0, 100));
    restartBtn->setScale9Enabled(true);
    restartBtn->setContentSize(Size(180, 80));
    restartBtn->setPosition(Vec2(visibleSize.width / 2 + 200, 100));

    restartBtn->addTouchEventListener([this](Ref* sender, Widget::TouchEventType type) {
        if (type == Widget::TouchEventType::ENDED) {
            this->restartGame();
        }
        });
    stackArea->addChild(restartBtn, 2);

    // 游戏说明
    auto instruction = Label::createWithTTF("Match cards ±1 of bottom card!", "fonts/arial.ttf", 30);
    instruction->setPosition(Vec2(visibleSize.width / 2, 50));
    instruction->setColor(Color3B::WHITE);
    stackArea->addChild(instruction, 2);
}

// 新增：通用MoveTo动画函数（所有卡牌移动统一用这个）
void HelloWorldScene::runCardMoveAnimation(Button* cardBtn, Vec2 targetPos, float duration) {
    cardBtn->runAction(Sequence::create(
        MoveTo::create(duration, targetPos),  // 核心MoveTo动画
        ScaleTo::create(0.1f, 0.9f),
        CallFunc::create([cardBtn]() {
            cardBtn->removeFromParent();
            }),
        nullptr
    ));
}

void HelloWorldScene::updateCardDisplay() {
    auto visibleSize = Director::getInstance()->getVisibleSize();

    // 绘制主牌堆
    _mainCardArea->removeAllChildren();
    int mainCardIndex = 0;
    const int cardsPerRow = 4;
    const float cardWidth = 200;
    const float cardHeight = 280;
    const float paddingX = (1080 - cardsPerRow * cardWidth) / (cardsPerRow + 1);
    const float paddingY = 80;

    for (auto card : _gameModel->mainCards) {
        if (card->isRemoved) continue;

        auto cardBtn = createCardButton(card);
        float x = paddingX + (mainCardIndex % cardsPerRow) * (cardWidth + paddingX);
        float y = _mainCardArea->getContentSize().height - paddingY - (mainCardIndex / cardsPerRow) * (cardHeight + paddingY);
        cardBtn->setPosition(Vec2(x, y));
        _mainCardArea->addChild(cardBtn, 2);
        mainCardIndex++;
    }

    // 绘制底牌
    _bottomCardArea->removeAllChildren();
    if (_gameModel->bottomCard) {
        auto bottomBtn = createCardButton(_gameModel->bottomCard);
        bottomBtn->setScale(0.9f);
        bottomBtn->setPosition(_bottomCardArea->getContentSize() / 2);
        _bottomCardArea->addChild(bottomBtn, 2);
    }

    // 绘制备用牌堆
    _backupCardArea->removeAllChildren();
    if (!_gameModel->backupCards.empty()) {
        auto backupBg = Sprite::create(getCardBackPath());
        backupBg->setScale(0.9f);
        backupBg->setPosition(_backupCardArea->getContentSize() / 2);
        _backupCardArea->addChild(backupBg, 1);

        auto countLabel = Label::createWithTTF("x" + std::to_string(_gameModel->backupCards.size()), "fonts/arial.ttf", 40);
        countLabel->setPosition(_backupCardArea->getContentSize() / 2);
        countLabel->setColor(Color3B::WHITE);
        _backupCardArea->addChild(countLabel, 2);
    }
    else {
        auto emptyLabel = Label::createWithTTF("Empty", "fonts/arial.ttf", 40);
        emptyLabel->setPosition(_backupCardArea->getContentSize() / 2);
        emptyLabel->setColor(Color3B::GRAY);
        _backupCardArea->addChild(emptyLabel, 1);
    }
}

Button* HelloWorldScene::createCardButton(CardModel* card)
{
    auto cardBtn = Button::create();
    cardBtn->setScale9Enabled(false);
    cardBtn->setContentSize(Size(200, 280));

    if (card->isFaceUp)
    {
        cardBtn->loadTextures("cards/back/card_front_bg.png",
            "cards/back/card_front_bg.png",
            "",
            Widget::TextureResType::LOCAL);
        cardBtn->setCapInsets(Rect(12, 12, 176, 256));

        /* 中央大数字 */
        auto bigNum = Sprite::create(getBigNumberPath(card->face, card->suit));
        if (bigNum)
        {
            bigNum->setPosition(Vec2(100, 140));
            bigNum->setScale(1.0f);
            cardBtn->addChild(bigNum, 2);
        }
        else
        {
            auto lb = Label::createWithTTF(faceToString(card->face), "fonts/arial.ttf", 60);
            lb->setColor(getNumberColor(card->suit) == "red" ? Color3B::RED : Color3B::BLACK);
            lb->setPosition(Vec2(100, 140));
            cardBtn->addChild(lb, 2);
        }

        /* 左上角小数字 */
        auto smallNum = Sprite::create(getSmallNumberPath(card->face, card->suit));
        if (smallNum)
        {
            smallNum->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
            smallNum->setPosition(Vec2(12, 280 - 12));
            cardBtn->addChild(smallNum, 2);
        }
        else
        {
            auto lb = Label::createWithTTF(faceToString(card->face), "fonts/arial.ttf", 22);
            lb->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
            lb->setPosition(Vec2(12, 280 - 12));
            lb->setColor(getNumberColor(card->suit) == "red" ? Color3B::RED : Color3B::BLACK);
            cardBtn->addChild(lb, 2);
        }

        /* 花色 */
        auto suit = Sprite::create(getSuitPath(card->suit));
        if (suit)
        {
            suit->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
            suit->setPosition(Vec2(12, 280 - 12 - 30 - 15));
            suit->setScale(1.0f);
            cardBtn->addChild(suit, 2);
        }
    }
    else
    {
        cardBtn->loadTextures(getCardBackPath(), getCardBackPath(), "");
    }

    /* 点击事件 */
    if (card->isFaceUp &&
        std::find(_gameModel->mainCards.begin(),
            _gameModel->mainCards.end(), card) != _gameModel->mainCards.end())
    {
        cardBtn->addTouchEventListener([this, card](Ref* sender, Widget::TouchEventType type) {
            if (type == Widget::TouchEventType::ENDED)
                this->handleCardClick(card, sender);
            });
    }
    return cardBtn;
}

std::string HelloWorldScene::getCardDisplayName(CardModel* card) {
    if (!card) return "";

    std::string faceStr, suitStr;
    switch (card->face) {
    case CFT_ACE: faceStr = "A"; break;
    case CFT_TWO: faceStr = "2"; break;
    case CFT_THREE: faceStr = "3"; break;
    case CFT_FOUR: faceStr = "4"; break;
    case CFT_FIVE: faceStr = "5"; break;
    case CFT_SIX: faceStr = "6"; break;
    case CFT_SEVEN: faceStr = "7"; break;
    case CFT_EIGHT: faceStr = "8"; break;
    case CFT_NINE: faceStr = "9"; break;
    case CFT_TEN: faceStr = "10"; break;
    case CFT_JACK: faceStr = "J"; break;
    case CFT_QUEEN: faceStr = "Q"; break;
    case CFT_KING: faceStr = "K"; break;
    default: faceStr = "?";
    }
    switch (card->suit) {
    case CST_SPADES: suitStr = "S"; break;
    case CST_HEARTS: suitStr = "H"; break;
    case CST_DIAMONDS: suitStr = "D"; break;
    case CST_CLUBS: suitStr = "C"; break;
    default: suitStr = "?";
    }
    return faceStr + "\n" + suitStr;
}

bool HelloWorldScene::checkMatch(CardModel* mainCard) {
    if (!_gameModel->bottomCard || !mainCard) return false;

    auto getCardValue = [](CardFaceType face) -> int {
        switch (face) {
        case CFT_ACE: return 1;
        case CFT_JACK: return 11;
        case CFT_QUEEN: return 12;
        case CFT_KING: return 13;
        default: return (int)face + 1;
        }
        };

    int mainVal = getCardValue(mainCard->face);
    int bottomVal = getCardValue(_gameModel->bottomCard->face);
    return abs(mainVal - bottomVal) == 1;
}

void HelloWorldScene::handleCardClick(CardModel* card, Ref* sender) {
    if (!card) return;
    auto visibleSize = Director::getInstance()->getVisibleSize();

    if (checkMatch(card)) {
        // 1. 记录操作历史（主牌匹配类型）
        MoveRecord record;
        record.type = MoveType::MAIN_CARD_MATCH;
        record.card = card;
        record.prevBottom = _gameModel->bottomCard;
        _moveHistory.push_back(record);

        // 2. 执行主牌匹配逻辑
        _cardManager->removeCard(card);
        auto oldBottom = _gameModel->bottomCard;
        _gameModel->bottomCard = card;

        _score += 20;
        _moves += 1;
        _undoButton->setEnabled(!_moveHistory.empty());

        if (!_gameModel->coveredCards.empty()) {
            auto nextCard = _gameModel->coveredCards.front();
            _gameModel->coveredCards.erase(_gameModel->coveredCards.begin());
            nextCard->isFaceUp = true;
            _gameModel->mainCards.push_back(nextCard);
        }

        // 3. 主牌移动到底牌堆的MoveTo动画
        auto button = dynamic_cast<Button*>(sender);
        if (button) {
            auto targetPos = _mainCardArea->convertToNodeSpace(
                _bottomCardArea->convertToWorldSpace(_bottomCardArea->getContentSize() / 2));

            auto movingCard = createCardButton(card);
            movingCard->setPosition(button->getPosition());
            _mainCardArea->addChild(movingCard, 10);

            // 调用通用MoveTo动画
            runCardMoveAnimation(movingCard, targetPos);
        }

        auto msg = Label::createWithTTF("Match Success! +20", "fonts/arial.ttf", 40);
        msg->setPosition(Vec2(visibleSize.width / 2, 300));
        msg->setColor(Color3B::GREEN);
        this->addChild(msg, 10);
        msg->runAction(Sequence::create(FadeOut::create(1.5f), RemoveSelf::create(), nullptr));
    }
    else {
        _moves += 1;
        auto msg = Label::createWithTTF("Not Match!", "fonts/arial.ttf", 40);
        msg->setPosition(Vec2(visibleSize.width / 2, 300));
        msg->setColor(Color3B::RED);
        this->addChild(msg, 10);
        msg->runAction(Sequence::create(FadeOut::create(1.0f), RemoveSelf::create(), nullptr));
    }

    auto button = dynamic_cast<Button*>(sender);
    if (button) {
        button->runAction(Sequence::create(ScaleTo::create(0.1f, 1.2f), ScaleTo::create(0.1f, 1.0f), nullptr));
    }

    updateScoreAndMoves();
    updateCardDisplay();
}

void HelloWorldScene::drawNewCard() {
    auto visibleSize = Director::getInstance()->getVisibleSize();

    if (_gameModel->backupCards.empty()) {
        auto msg = Label::createWithTTF("No Backup Cards!", "fonts/arial.ttf", 40);
        msg->setPosition(Vec2(visibleSize.width / 2, 300));
        msg->setColor(Color3B::RED);
        this->addChild(msg, 10);
        msg->runAction(Sequence::create(FadeOut::create(1.0f), RemoveSelf::create(), nullptr));
        return;
    }

    // 1. 记录操作历史（备用牌抽取类型）
    MoveRecord record;
    record.type = MoveType::BACKUP_CARD_DRAW;
    record.card = _gameModel->backupCards.back();
    record.prevBottom = _gameModel->bottomCard;
    _moveHistory.push_back(record);

    // 2. 执行备用牌抽取逻辑
    auto newBottom = _gameModel->backupCards.back();
    _gameModel->backupCards.pop_back();
    newBottom->isFaceUp = true;
    auto oldBottom = _gameModel->bottomCard;
    _gameModel->bottomCard = newBottom;

    _moves += 1;
    _score = std::max(0, _score - 5);
    _undoButton->setEnabled(!_moveHistory.empty());
    updateScoreAndMoves();

    // 3. 备用牌移动到底牌堆的MoveTo动画
    auto targetPos = _bottomCardArea->convertToNodeSpace(
        _bottomCardArea->convertToWorldSpace(_bottomCardArea->getContentSize() / 2));

    auto movingCard = createCardButton(newBottom);
    movingCard->setPosition(_backupCardArea->convertToNodeSpace(
        _backupCardArea->convertToWorldSpace(_backupCardArea->getContentSize() / 2)));
    this->addChild(movingCard, 10);

    // 调用通用MoveTo动画
    runCardMoveAnimation(movingCard, targetPos);

    auto msg = Label::createWithTTF("Draw Backup Card! -5", "fonts/arial.ttf", 40);
    msg->setPosition(Vec2(visibleSize.width / 2, 300));
    msg->setColor(Color3B::YELLOW);
    this->addChild(msg, 10);
    msg->runAction(Sequence::create(FadeOut::create(1.5f), RemoveSelf::create(), nullptr));

    updateCardDisplay();
}

// 核心：精准回退（主牌回主牌堆，备用牌回备用牌堆）
void HelloWorldScene::undoLastMove() {
    if (_moveHistory.empty()) {
        auto msg = Label::createWithTTF("Cannot undo further!", "fonts/arial.ttf", 40);
        msg->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, 300));
        msg->setColor(Color3B::ORANGE);
        this->addChild(msg, 10);
        msg->runAction(Sequence::create(FadeOut::create(1.0f), RemoveSelf::create(), nullptr));
        return;
    }

    // 1. 获取最后一步操作记录
    MoveRecord lastRecord = _moveHistory.back();
    _moveHistory.pop_back();

    // 2. 恢复底牌
    CardModel* currentBottom = _gameModel->bottomCard;
    _gameModel->bottomCard = lastRecord.prevBottom;

    // 3. 根据操作类型精准回退
    if (lastRecord.type == MoveType::MAIN_CARD_MATCH) {
        // 主牌匹配操作：回退到主牌堆
        currentBottom->isRemoved = false;
        auto it = std::find(_gameModel->removedCards.begin(), _gameModel->removedCards.end(), currentBottom);
        if (it != _gameModel->removedCards.end()) {
            _gameModel->removedCards.erase(it);
        }
        _gameModel->mainCards.push_back(currentBottom);

        // 计算主牌堆目标位置（MoveTo动画）
        int insertIndex = _gameModel->mainCards.size() - 1;
        const int cardsPerRow = 4;
        const float cardWidth = 200;
        const float cardHeight = 280;
        const float paddingX = (1080 - cardsPerRow * cardWidth) / (cardsPerRow + 1);
        const float paddingY = 80;
        Vec2 targetPos = Vec2(
            paddingX + (insertIndex % cardsPerRow) * (cardWidth + paddingX),
            _mainCardArea->getContentSize().height - paddingY - (insertIndex / cardsPerRow) * (cardHeight + paddingY)
        );

        // 主牌从底牌堆回退到主牌堆的MoveTo动画
        auto movingCard = createCardButton(currentBottom);
        movingCard->setPosition(_bottomCardArea->convertToNodeSpace(
            _bottomCardArea->convertToWorldSpace(_bottomCardArea->getContentSize() / 2)));
        this->addChild(movingCard, 10);

        runCardMoveAnimation(movingCard, _mainCardArea->convertToNodeSpace(
            _mainCardArea->convertToWorldSpace(targetPos)));

        // 恢复分数和步数
        _score = std::max(0, _score - 20);
        _moves -= 1;

        auto msg = Label::createWithTTF("Undo main card match", "fonts/arial.ttf", 40);
        msg->setColor(Color3B(0, 255, 255));
        msg->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, 300));
        this->addChild(msg, 10);
        msg->runAction(Sequence::create(FadeOut::create(1.0f), RemoveSelf::create(), nullptr));

    }
    else if (lastRecord.type == MoveType::BACKUP_CARD_DRAW) {
        // 备用牌抽取操作：回退到备用牌堆
        currentBottom->isFaceUp = false;
        _gameModel->backupCards.push_back(currentBottom);

        // 备用牌从底牌堆回退到备用牌堆的MoveTo动画
        auto movingCard = createCardButton(currentBottom);
        movingCard->setPosition(_bottomCardArea->convertToNodeSpace(
            _bottomCardArea->convertToWorldSpace(_bottomCardArea->getContentSize() / 2)));
        this->addChild(movingCard, 10);

        runCardMoveAnimation(movingCard, _backupCardArea->convertToNodeSpace(
            _backupCardArea->convertToWorldSpace(_backupCardArea->getContentSize() / 2)));

        // 恢复分数和步数
        _score += 5;
        _moves -= 1;

        auto msg = Label::createWithTTF("Undo backup card draw", "fonts/arial.ttf", 40);
        msg->setColor(Color3B(0, 255, 255));
        msg->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, 300));
        this->addChild(msg, 10);
        msg->runAction(Sequence::create(FadeOut::create(1.0f), RemoveSelf::create(), nullptr));
    }

    // 4. 更新UI状态
    _undoButton->setEnabled(!_moveHistory.empty());
    updateScoreAndMoves();
    updateCardDisplay();
}

void HelloWorldScene::restartGame() {
    auto visibleSize = Director::getInstance()->getVisibleSize();

    _score = 0;
    _moves = 0;
    updateScoreAndMoves();

    for (auto card : _gameModel->mainCards) _cardManager->removeCard(card);
    for (auto card : _gameModel->coveredCards) _cardManager->removeCard(card);
    if (_gameModel->bottomCard) _cardManager->removeCard(_gameModel->bottomCard);
    for (auto card : _gameModel->backupCards) _cardManager->removeCard(card);

    _gameModel->mainCards.clear();
    _gameModel->coveredCards.clear();
    _gameModel->bottomCard = nullptr;
    _gameModel->backupCards.clear();
    _gameModel->removedCards.clear();
    _moveHistory.clear(); // 清空操作历史

    loadGame();

    auto msg = Label::createWithTTF("Game Restarted!", "fonts/arial.ttf", 50);
    msg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    msg->setColor(Color3B::YELLOW);
    this->addChild(msg, 10);
    msg->runAction(Sequence::create(FadeOut::create(2.0f), RemoveSelf::create(), nullptr));
}

void HelloWorldScene::updateScoreAndMoves() {
    _scoreLabel->setString("Score: " + std::to_string(_score));
    _movesLabel->setString("Moves: " + std::to_string(_moves));
}

void HelloWorldScene::cleanupGameObjects() {
    CCLOG("Cleaning up game objects...");

    _moveHistory.clear();

    if (_cardManager) {
        delete _cardManager;
        _cardManager = nullptr;
    }
    if (_gameController) {
        delete _gameController;
        _gameController = nullptr;
    }
    if (_gameModel) {
        delete _gameModel;
        _gameModel = nullptr;
    }
}

void HelloWorldScene::onEnter() {
    Scene::onEnter();
    CCLOG("HelloWorldScene onEnter!");
}

void HelloWorldScene::onExit() {
    Scene::onExit();
    CCLOG("HelloWorldScene onExit!");
}