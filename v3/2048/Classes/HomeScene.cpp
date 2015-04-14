//
//  HomeScene.cpp
//  2048
//
//  Created by 宋橙 on 14-6-16.
//
//

#include "HomeScene.h"
#include "GameScene.h"
#include "InstructorScene.h"
#include "MyTools.h"

USING_NS_CC;

Scene* Home::createScene()
{
    auto scene = Scene::create();
    auto layer = Home::create();
    scene->addChild(layer);
    return scene;
}

bool Home::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    initData();
    
    return true;
}


#pragma mark - 初始化

//初始化各项数据
void Home::initData()
{
    //加载背景
    loadBackground();
    //加载声音菜单
    loadSoundMenu();
    //加载主菜单
    loadMainMenu();
    
}

//加载背景
void Home::loadBackground()
{
    auto background = Sprite::createWithSpriteFrameName("bg_blank.png");
    background->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
    addChild(background);
    
    auto menu = Sprite::createWithSpriteFrameName("bg_menu.png");
    menu->setPosition(320, 875);
    addChild(menu);
}


#pragma mark - 菜单

//加载声音菜单
void Home::loadSoundMenu()
{
    //声音按钮
    auto soundOnSprite = Sprite::createWithSpriteFrameName("btn_sound_on.png");
    auto soundOnSpriteItem = MenuItemSprite::create(soundOnSprite, soundOnSprite);
    
    auto soundOffSprite = Sprite::createWithSpriteFrameName("btn_sound_off.png");
    auto soundOffSpriteItem = MenuItemSprite::create(soundOffSprite, soundOffSprite);
    
    auto toggleItem = MenuItemToggle::createWithCallback(CC_CALLBACK_1(Home::soundMenuCallback, this), soundOnSpriteItem, soundOffSpriteItem, NULL);
    toggleItem->setPosition(Point(240, 400));
    if (UserDefault::getInstance()->getBoolForKey("soundOff")) {
        toggleItem->setSelectedIndex(1);
    }
    auto soundMenu = Menu::create(toggleItem, NULL);
    this->addChild(soundMenu);
}

//声音菜单回调函数
void Home::soundMenuCallback(Ref *sender)
{
    auto index = ((MenuItemToggle *)sender)->getSelectedIndex();
//    log("index = %d", index);
    
    auto userDefault = UserDefault::getInstance();
    switch (index) {
        case 0:
//            log("开启声音...");
            MyTools::playEffect("move.wav", true);
            userDefault->setBoolForKey("soundOff", false);
            break;
        case 1:
//            log("关闭声音...");
            userDefault->setBoolForKey("soundOff", true);
            break;
        default:
            break;
    }
    userDefault->flush(); //提交
}

//加载主菜单
void Home::loadMainMenu()
{
    //继续按钮
    auto continueSprite = Sprite::createWithSpriteFrameName("btn_continue.png");
    auto continueSpriteItem = MenuItemSprite::create(continueSprite, continueSprite, CC_CALLBACK_1(Home::mainMenuCallback, this));
    continueSpriteItem->setPosition(Point(0, 200));
    continueSpriteItem->setTag(HomeContinueTag);
    
    //重新开始按钮
    auto restartSprite = Sprite::createWithSpriteFrameName("btn_restart.png");
    auto restartSpriteItem = MenuItemSprite::create(restartSprite, restartSprite, CC_CALLBACK_1(Home::mainMenuCallback, this));
    restartSpriteItem->setPosition(Point(0, 40));
    restartSpriteItem->setTag(HomeRestartTag);
    
    //评论按钮
    auto commentSprite = Sprite::createWithSpriteFrameName("btn_comment.png");
    auto commentSpriteItem = MenuItemSprite::create(commentSprite, commentSprite, CC_CALLBACK_1(Home::mainMenuCallback, this));
    commentSpriteItem->setPosition(Point(0, -120));
    commentSpriteItem->setTag(HomeCommentTag);
    
    //指导书按钮
    auto instructorSprite = Sprite::createWithSpriteFrameName("btn_instructor.png");
    auto instructorSpriteItem = MenuItemSprite::create(instructorSprite, instructorSprite, CC_CALLBACK_1(Home::mainMenuCallback, this));
    instructorSpriteItem->setPosition(Point(0, -280));
    instructorSpriteItem->setTag(HomeInstructorTag);
    
    auto mainMenu = Menu::create(continueSpriteItem, restartSpriteItem, commentSpriteItem, instructorSpriteItem, NULL);
    this->addChild(mainMenu);
}

//主菜单回调函数
void Home::mainMenuCallback(Ref *sender)
{
    MyTools::playEffect("move.wav");
    
    auto menuItemSprite = ((MenuItemSprite *)sender);
    auto tag = menuItemSprite->getTag();
//    log("tag = %d", tag);
    switch (tag) {
        case HomeContinueTag:
            Director::getInstance()->replaceScene(TransitionFade::create(0.5f, Game::createScene()));
            break;
        case HomeRestartTag:
            Game::clearData();
            Director::getInstance()->replaceScene(TransitionFade::create(0.5f, Game::createScene()));
            break;
        case HomeCommentTag:
            Director::getInstance()->pushScene(TransitionFade::create(0.5f, Instructor::createScene()));
            break;
        case HomeInstructorTag:
            Director::getInstance()->pushScene(TransitionFade::create(0.5f, Instructor::createScene()));
            break;
        default:
            break;
    }
}






