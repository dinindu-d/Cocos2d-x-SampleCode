//
//  OverScene.h
//  2048
//
//  Created by 宋橙 on 14-6-16.
//
//

#ifndef ___048__OverScene__
#define ___048__OverScene__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;

enum OverTagList
{
    OverShareTag = 1,
    OverRestartTag,
};

class Over : public cocos2d::Layer
{
public:
    static int score;
    static cocos2d::Scene* createScene();
    
private:
    virtual bool init();
    CREATE_FUNC(Over);
    
    void initData();
    void loadBackground();
    void showScore();
    void loadMenu();
    void menuCallback(Ref *sender);
    
};

#endif /* defined(___048__OverScene__) */






