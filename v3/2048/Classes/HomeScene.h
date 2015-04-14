//
//  HomeScene.h
//  2048
//
//  Created by 宋橙 on 14-6-16.
//
//

#ifndef ___048__HomeScene__
#define ___048__HomeScene__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;

enum HomeTagList
{
    HomeContinueTag = 1,
    HomeRestartTag,
    HomeCommentTag,
    HomeInstructorTag,
};

class Home : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
private:
    virtual bool init();
    CREATE_FUNC(Home);
    
#pragma mark - 初始化
    void initData();
    void loadBackground();

#pragma mark - 菜单
    void loadSoundMenu();
    void loadMainMenu();
    void mainMenuCallback(Ref *sender);
    void soundMenuCallback(Ref *sender);
    
};

#endif /* defined(___048__HomeScene__) */




