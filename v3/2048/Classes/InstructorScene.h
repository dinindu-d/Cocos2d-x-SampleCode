//
//  InstructorScene.h
//  2048
//
//  Created by 宋橙 on 14-6-18.
//
//

#ifndef ___048__InstructorScene__
#define ___048__InstructorScene__

#include <iostream>
#include "cocos2d.h"
#include "CocosGUI.h"

USING_NS_CC;
using namespace ui;

class Instructor : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
private:
    virtual bool init();
    CREATE_FUNC(Instructor);
    
    PageView *pageView;
    Layout *dotLayout;
    
    void initData();
    void loadBackground();
    void loadPageView();
    void loadPageDot();
    void pageViewEvent(Ref *sender, PageViewEventType type);
    void startCallback(Ref *sender);
    void changePageDotFrame();
    
};

#endif /* defined(___048__InstructorScene__) */






