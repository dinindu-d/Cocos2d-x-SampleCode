//
//  GameScene.h
//  2048
//
//  Created by 宋橙 on 14-6-16.
//
//

#ifndef ___048__GameScene__
#define ___048__GameScene__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;

class Station;
class Box;

enum WhereGo
{
    GO_UP,
    GO_DOWN,
    GO_LEFT,
    GO_RIGHT,
};

enum GameTagList
{
    GameMenuTag = 1,
    GameBillboradTag,
    GameRestartTag,
    GameShareTag,
};

class Game : public cocos2d::Layer
{
public:
    static void clearData();
    static cocos2d::Scene* createScene();
    
private:
    virtual bool init();
    CREATE_FUNC(Game);
    
#pragma mark - 初始化
    Vector<Node *> boxes;
    Map<std::string, Node *> stations;
    bool isWin;
    
    void initData();
    void loadBackground();
    void loadMenu();
    void loadWin();
    
#pragma mark - 得分
    int lastScore;
    int maxScore;
    Label *lastScoreLabel;
    
    void showScore();
    
#pragma mark - 站点
    void loadStations();
    Vector<Node *> getEmptyStations();
    Station * getStation(int line, int column);
    Station * getNextStation(Station *station, WhereGo whereGo);
    
#pragma mark - 数盒
    void addBox();
    void loadBox();
    
#pragma mark - 触摸监听
    Point beginPoint; //触摸起点
    bool oneTouch; //是否同一次触摸事件
    
    void onEnter();
    bool onTouchBegan(Touch *touch, Event *event);
    void onTouchMoved(Touch *touch, Event *event);
    void onTouchEnded(Touch *touch, Event *event);

#pragma mark - 移动
    int number; //站点完成移动计数
    bool firstMove; //是否第一次移动
    
    void moveAll(WhereGo whereGo);
    void moveOne(Station *station, WhereGo whereGo);
    void moveToNext(Station *station, WhereGo whereGo);
    void moveCallback(Box *box, WhereGo whereGo);
    void countNumber();
    
#pragma mark - 合并
    void combineWithNext(Box *box, WhereGo whereGo);
    void resetCombined();
    
#pragma mark - 切换场景
    void menuCallback(Ref *sender);
    void gameOver();
    void recordData();
    
};

#endif /* defined(___048__GameScene__) */





