//
//  GameScene.cpp
//  2048
//
//  Created by 宋橙 on 14-6-16.
//
//

#include "GameScene.h"
#include "Station.h"
#include "Box.h"
#include "MyTools.h"
#include "OverScene.h"
#include "HomeScene.h"

USING_NS_CC;

#define userDefault UserDefault::getInstance()
#define DURATION 0.1f
#define WIN_NUMBER 2048

Scene* Game::createScene()
{
    auto scene = Scene::create();
    auto layer = Game::create();
    scene->addChild(layer);
    return scene;
}

bool Game::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    //随机数种子
    srand((int)time(NULL));
    //初始化各项数据
    initData();
    
    return true;
}

//清除上次游戏数据，用于重新开始
void Game::clearData()
{
    userDefault->setIntegerForKey("lastScore", 0);
    for (int line = 0; line < 4; line++) {
        for (int column = 0; column < 4; column++) {
            auto key = String::createWithFormat("tile_%i%i", line, column)->getCString();
            userDefault->setIntegerForKey(key, 0);
        }
    }
    userDefault->flush();
}

#pragma mark - 初始化

//初始化各项数据
void Game::initData()
{
    number = 0;
    firstMove = true;
    isWin = false;
    
    //加载背景
    loadBackground();
    //显示得分
    showScore();
    //加载菜单
    loadMenu();
    //加载站点
    stations = Map<std::string, Node *>(16);
    loadStations();
    //添加数盒
    boxes = Vector<Node *>(16);
    loadBox();
    
}

//加载背景
void Game::loadBackground()
{
    auto background = Sprite::createWithSpriteFrameName("bg_blank.png");
    background->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
    addChild(background);
    
    auto icon = Sprite::createWithSpriteFrameName("bg_icon.png");
    icon->setPosition(130, 825);
    addChild(icon);
    
    auto scoreLast = Sprite::createWithSpriteFrameName("bg_score_last.png");
    scoreLast->setPosition(340, 865);
    addChild(scoreLast);
    
    auto scoreMax = Sprite::createWithSpriteFrameName("bg_score_max.png");
    scoreMax->setPosition(530, 865);
    addChild(scoreMax);
    
    auto table = Sprite::createWithSpriteFrameName("bg_table.png");
    table->setPosition(320, 410);
    addChild(table);
}

//加载菜单
void Game::loadMenu()
{
    //菜单按钮
    auto menuSprite = Sprite::createWithSpriteFrameName("btn_menu.png");
    auto menuSpriteItem = MenuItemSprite::create(menuSprite, menuSprite, CC_CALLBACK_1(Game::menuCallback, this));
    menuSpriteItem->setPosition(Point(20, 285));
    menuSpriteItem->setTag(GameMenuTag);
    
    //排行榜按钮
    auto billboradSprite = Sprite::createWithSpriteFrameName("btn_billboard.png");
    auto billboradSpriteItem = MenuItemSprite::create(billboradSprite, billboradSprite, CC_CALLBACK_1(Game::menuCallback, this));
    billboradSpriteItem->setPosition(Point(210, 285));
    billboradSpriteItem->setTag(GameBillboradTag);
    
    auto menu = Menu::create(menuSpriteItem, billboradSpriteItem, NULL);
    this->addChild(menu);
}

//获胜加载
void Game::loadWin()
{
    _eventDispatcher->removeAllEventListeners();
    
    auto mask = Sprite::createWithSpriteFrameName("bg_mask.png");
    mask->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
    addChild(mask, 2, 100);
    
    auto win = Sprite::createWithSpriteFrameName("bg_win.png");
    win->setScale(1.2f);
    win->setPosition(320, 410);
    addChild(win, 2, 101);
    
    //再试一次按钮
    auto restartSprite = Sprite::createWithSpriteFrameName("btn_restart_win.png");
    restartSprite->setScale(1.2f);
    auto restareSpriteItem = MenuItemSprite::create(restartSprite, restartSprite, CC_CALLBACK_1(Game::menuCallback, this));
    restareSpriteItem->setPosition(Point(-80, -200));
    restareSpriteItem->setTag(GameRestartTag);
    
    //分享按钮
    auto shareSprite = Sprite::createWithSpriteFrameName("btn_share_win.png");
    shareSprite->setScale(1.2f);
    auto shareSpriteItem = MenuItemSprite::create(shareSprite, shareSprite, CC_CALLBACK_1(Game::menuCallback, this));
    shareSpriteItem->setPosition(Point(80, -200));
    shareSpriteItem->setTag(GameShareTag);
    
    auto menu = Menu::create(restareSpriteItem, shareSpriteItem, NULL);
    this->addChild(menu, 2, 102);
}

#pragma mark - 得分

//显示得分
void Game::showScore()
{
    //最近得分
    lastScore = userDefault->getIntegerForKey("lastScore");
    lastScoreLabel = Label::createWithSystemFont(String::createWithFormat("%d", lastScore)->getCString(), "HelveticaNeue-Bold", 50);
    lastScoreLabel->setPosition(Point(340, 850));
    lastScoreLabel->setColor(Color3B(255, 255, 255));
    addChild(lastScoreLabel);
    
    //最大得分
    maxScore = userDefault->getIntegerForKey("maxScore", lastScore);
    auto maxScoreLabel = Label::createWithSystemFont(String::createWithFormat("%d", maxScore)->getCString(), "HelveticaNeue-Bold", 50);
    maxScoreLabel->setPosition(Point(530, 850));
    maxScoreLabel->setColor(Color3B(255, 255, 255));
    addChild(maxScoreLabel);
}


#pragma mark - 站点

//加载站点
void Game::loadStations()
{
    auto stationPoints = FileUtils::getInstance()->getValueVectorFromFile("stationPoints.plist");
    for (int i = 0; i < stationPoints.size(); i++) {
        auto columns = stationPoints[i].asValueVector();
        for (int j = 0; j < columns.size(); j++) {
            auto point = columns[j].asValueMap();
            auto key = String::createWithFormat("%i%i", i, j)->getCString();
            auto position = Point(point["x"].asFloat(), point["y"].asFloat());
            stations.insert(key, Station::stationWithPosition(position, i, j));
        }
    }
}

//获取空站点
Vector<Node *> Game::getEmptyStations()
{
    auto emptyStations = Vector<Node *>(16);
    for (auto iter = stations.begin(); iter != stations.end(); iter++) {
        auto station = (Station *)(iter->second);
        if (station->box == NULL){
            emptyStations.pushBack(station);
        }
    }
    return emptyStations;
}


//通过行列号获取站点
Station * Game::getStation(int line, int column)
{
    auto key = String::createWithFormat("%i%i", line, column)->getCString();
    auto station = (Station *)stations.at(key);
    return station;
}

//获取下一个站点
Station * Game::getNextStation(Station *station, WhereGo whereGo)
{
    auto line = station->line;
    auto column = station->column;
    switch (whereGo) {
        case GO_UP:
            line -= 1;
            if (line < 0) return NULL;
            break;
        case GO_DOWN:
            line += 1;
            if (line > 3) return NULL;
            break;
        case GO_LEFT:
            column -= 1;
            if (column < 0) return NULL;
            break;
        case GO_RIGHT:
            column += 1;
            if (column > 3) return NULL;
            break;
        default:
            return NULL;
            break;
    }
    return getStation(line, column);
}


#pragma mark - 数盒

//随机添加一个数盒
void Game::addBox()
{
    //获取空站点
    auto emptyStations = getEmptyStations();
    if (emptyStations.size() == 0) {
        gameOver();
        return;
    }
    
    //随机选择一个空站点
    int index = rand() % emptyStations.size();
    auto station = (Station *)emptyStations.at(index);
    
    //随机一个数盒
    int number = pow(2, rand()%2 + 1);
    auto box = Box::boxWithStation(station, number);
    
    station->box = box;
    boxes.pushBack(box);
    
    box->setScale(0.0f);
    addChild(box);
    box->runAction(ScaleTo::create(DURATION, 1.0f));
    
    //记录数据
    recordData();
    
    //当就剩一个可用站点时添加数盒后的判断
    if (!isWin && emptyStations.size() == 1) {
        gameOver();
    }
}

//加载记录的数盒
void Game::loadBox()
{
    int count = 0;
    
    //继续
    for (auto iter = stations.begin(); iter != stations.end(); iter++) {
        auto station = (Station *)(iter->second);
        auto key = String::createWithFormat("tile_%i%i", station->line, station->column)->getCString();
        auto number = userDefault->getIntegerForKey(key);
        if (number) {
            count ++;
//            log("读取：tile_%d%d = %d", station->line, station->column, number);
            auto box = Box::boxWithStation(station, number);
            station->box = box;
            boxes.pushBack(box);
            addChild(box);
        }
    }
    
    //重新开始
    if (count < 2) {
        clearData();
        addBox();
        addBox();
    }
}


#pragma mark - 触摸监听

//进入该场景就添加触摸监听事件
void Game::onEnter()
{
    Layer::onEnter();
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(Game::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(Game::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(Game::onTouchEnded, this);
    listener->setSwallowTouches(true);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

//开始触摸时
bool Game::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    if (number != 0) {
        return false;
    }
    beginPoint = touch->getLocation();
    return true;
}

//触摸移动时
void Game::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{
    if (oneTouch) return;
    auto endPoint = touch->getLocation();
    auto deltaPoint = endPoint - beginPoint;
    
    if (fabs(deltaPoint.x) > 20 || fabs(deltaPoint.y) > 20) {
        oneTouch = true;
        
        WhereGo whereGo;
        if (fabs(deltaPoint.y) > fabs(deltaPoint.x)) {
            if (deltaPoint.y > 0) {
                whereGo = GO_UP;
            } else {
                whereGo = GO_DOWN;
            }
        } else {
            if (deltaPoint.x < 0) {
                whereGo = GO_LEFT;
            } else {
                whereGo = GO_RIGHT;
            }
        }
        moveAll(whereGo);
    }
}

//结束触摸时
void Game::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{
    oneTouch = false;
}


#pragma mark - 移动

//移动所有
void Game::moveAll(WhereGo whereGo)
{
    switch (whereGo) {
        case GO_UP:
//            log("上滑 ^");
            for (int line = 1; line <= 3; line++) {
                for (int column = 0; column <= 3; column++) {
                    moveOne(getStation(line, column), GO_UP);
                }
            }
            break;
        case GO_DOWN:
//            log("下滑 v");
            for (int line = 2; line >= 0; line--) {
                for (int column = 0; column <= 3; column++) {
                    moveOne(getStation(line, column), GO_DOWN);
                }
            }
            break;
        case GO_LEFT:
//            log("左滑 <");
            for (int column = 1; column <= 3; column++) {
                for (int line = 0; line <= 3; line++) {
                    moveOne(getStation(line, column), GO_LEFT);
                }
            }
            break;
        case GO_RIGHT:
//            log("右滑 >");
            for (int column = 2; column >= 0; column--) {
                for (int line = 0; line <= 3; line++) {
                    moveOne(getStation(line, column), GO_RIGHT);
                }
            }
            break;
        default:
            break;
    }
}

//移动一个
void Game::moveOne(Station *station, WhereGo whereGo)
{
//    auto key = String::createWithFormat("%i%i", station->line, station->column)->getCString();
//    log("移动：key=%s", key);
    
    auto box = station->box;
    if (box) {
        auto nextStation = getNextStation(station, whereGo);
        auto nextBox = nextStation->box;
        if (nextBox) {
            //相邻两数盒相等并且没有被合并过
            if (nextBox->number == box->number && !(nextBox->hasCombined)) {
                moveToNext(station, whereGo);
                return;
            }
        } else {
            moveToNext(station, whereGo);
            return;
        }
    }
    countNumber();
}

//移动到下一个站点
void Game::moveToNext(Station *station, WhereGo whereGo)
{
    if (firstMove) {
        MyTools::playEffect("move.wav");
        firstMove = false;
    }
    auto box = station->box;
    station->box = NULL;
    auto nextStation = getNextStation(station, whereGo);
    auto moveAction = MoveTo::create(DURATION/2, nextStation->position);
    auto moveCallback = CallFunc::create(CC_CALLBACK_0(Game::moveCallback, this, box, whereGo));
    auto seq = Sequence::create(moveAction, moveCallback, NULL);
    box->runAction(seq);
}

//移动完毕的回调函数
void Game::moveCallback(Box *box, WhereGo whereGo)
{
    auto station = box->station;
    auto nextStation = getNextStation(station, whereGo);
    auto nextBox = nextStation->box;
    
    if (nextBox) {
//        log("合并相邻的两个%d", box->number);
        combineWithNext(box, whereGo);
        return;
    } else {
        nextStation->box = box;
        box->station = nextStation;
        //还有下一个站点就继续移动
        if (getNextStation(nextStation, whereGo)) {
            moveOne(nextStation, whereGo);
            return;
        }
    }
    countNumber();
}

//记录移动完毕的站点数
void Game::countNumber()
{
    number ++;
//    log("number = %d", number);
    
    if (number == 12) {
        number = 0;
        //没有移动时不添加新数盒
        if (!firstMove) {
            firstMove = true;
            resetCombined();
            addBox();
        }
    }
}


#pragma mark - 合并

//与下一个数盒合并
void Game::combineWithNext(Box *box, WhereGo whereGo)
{
    MyTools::playEffect("merge.wav");
    
    auto station = box->station;
    auto nextStation = getNextStation(station, whereGo);
    auto nextBox = nextStation->box;
    
    auto newNumber = box->number * 2;
    auto newBox = Box::boxWithStation(nextStation, newNumber);
    newBox->hasCombined = true;
    
    boxes.eraseObject(box);
    box->removeFromParent();
    
    boxes.eraseObject(nextBox);
    nextBox->removeFromParent();
    nextStation->box = newBox;
    
    boxes.pushBack(newBox);
    addChild(newBox);
    newBox->runAction(Sequence::create(ScaleTo::create(DURATION, 1.2f), ScaleTo::create(DURATION, 1.0f), NULL));
    
    //更新分数
    lastScore += newNumber;
    lastScoreLabel->setString(String::createWithFormat("%d", lastScore)->getCString());
    
    if (newNumber == WIN_NUMBER) {
        isWin = true;
        //闪烁
        newBox->runAction(RepeatForever::create(Blink::create(0.5f, 1)));
    }
    
    countNumber();
}

//重置数盒合并状态
void Game::resetCombined()
{
//    log("重置合并状态...");
    for (int i = 0; i < boxes.size(); i++) {
        auto box = (Box *)boxes.at(i);
        box->hasCombined = false;
    }
}


#pragma mark - 场景切换

//菜单回调方法
void Game::menuCallback(Ref *sender)
{
    MyTools::playEffect("move.wav");
    
    auto menuItemSprite = ((MenuItemSprite *)sender);
    auto tag = menuItemSprite->getTag();
//    log("tag = %d", tag);
    switch (tag) {
        case GameMenuTag: //菜单：前往主页
            Director::getInstance()->replaceScene(TransitionFade::create(0.5f, Home::createScene()));
            break;
        case GameBillboradTag: //排行榜：该功能暂时没做，和菜单一样
            Director::getInstance()->replaceScene(TransitionFade::create(0.5f, Home::createScene()));
            break;
        case GameRestartTag: //再试一次
            Director::getInstance()->replaceScene(TransitionFade::create(0.5f, Game::createScene()));
            break;
        case GameShareTag: //分享：该功能暂时没做，和再试一次一样
            Director::getInstance()->replaceScene(TransitionFade::create(0.5f, Game::createScene()));
            break;
        default:
            break;
    }
}

//游戏结束方法
void Game::gameOver()
{
    for (int i = 0; i < boxes.size(); i++) {
        auto box = (Box *)boxes.at(i);
        auto station = box->station;
        
        auto upStation = getNextStation(station, GO_UP);
        if (upStation && upStation->box->number == box->number) return;
        
        auto downStation = getNextStation(station, GO_DOWN);
        if (downStation && downStation->box->number == box->number) return;
        
        auto leftStation = getNextStation(station, GO_LEFT);
        if (leftStation && leftStation->box->number == box->number) return;
        
        auto rightStation = getNextStation(station, GO_RIGHT);
        if (rightStation && rightStation->box->number == box->number) return;
    }
    
//    log("游戏结束...数盒个数：%d", (int)boxes.size());
    clearData();
    Over::score = lastScore;
    Director::getInstance()->replaceScene(TransitionFade::create(1.5f, Over::createScene()));
}

//记录数据
void Game::recordData()
{
    //记录得分
    userDefault->setIntegerForKey("lastScore", lastScore);
    if (lastScore > maxScore) {
        maxScore = lastScore;
        userDefault->setIntegerForKey("maxScore", maxScore);
    }
    
    //记录数盒
    for (auto iter = stations.begin(); iter != stations.end(); iter++) {
        auto station = (Station *)(iter->second);
        auto box = station->box;
        auto number = box ? box->number : 0;
        
        auto key = String::createWithFormat("tile_%i%i", station->line, station->column)->getCString();
        userDefault->setIntegerForKey(key, number);
//        log("更新：tile_%d%d = %d", station->line, station->column, number);
    }
    
    userDefault->flush();
    
    //获胜
    if (isWin) {
        clearData();
        loadWin();
    }
}


