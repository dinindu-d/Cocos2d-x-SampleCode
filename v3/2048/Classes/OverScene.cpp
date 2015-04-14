//
//  OverScene.cpp
//  2048
//
//  Created by 宋橙 on 14-6-16.
//
//

#include "OverScene.h"
#include "MyTools.h"
#include "GameScene.h"

USING_NS_CC;

int Over::score = 0;

Scene* Over::createScene()
{
    auto scene = Scene::create();
    auto layer = Over::create();
    scene->addChild(layer);
    return scene;
}

bool Over::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    initData();
    
    return true;
}

//初始化各项数据
void Over::initData()
{
    //加载背景
    loadBackground();
    //显示得分
    showScore();
    //加载菜单
    loadMenu();
}

//加载背景
void Over::loadBackground()
{
    auto background = Sprite::createWithSpriteFrameName("bg_blank.png");
    background->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
    addChild(background);
    
    auto over = Sprite::createWithSpriteFrameName("bg_over.png");
    over->setPosition(320, 855);
    addChild(over);
    
    auto scoreOver = Sprite::createWithSpriteFrameName("bg_score_over.png");
    scoreOver->setPosition(320, 665);
    addChild(scoreOver);
}

//显示得分
void Over::showScore()
{
//    auto score = UserDefault::getInstance()->getIntegerForKey("lastScore");
    auto scoreLabel = Label::createWithSystemFont(String::createWithFormat("%d", score)->getCString(), "HelveticaNeue-Bold", 50);
    scoreLabel->setPosition(Point(320, 650));
    scoreLabel->setColor(Color3B(255, 255, 255));
    addChild(scoreLabel);
}

//加载菜单
void Over::loadMenu()
{
    //分享按钮
    auto shareSprite = Sprite::createWithSpriteFrameName("btn_share.png");
    auto shareSpriteItem = MenuItemSprite::create(shareSprite, shareSprite, CC_CALLBACK_1(Over::menuCallback, this));
    shareSpriteItem->setPosition(Point(0, -20));
    shareSpriteItem->setTag(OverShareTag);
    
    //重新开始按钮
    auto restartSprite = Sprite::createWithSpriteFrameName("btn_restart.png");
    auto restartSpriteItem = MenuItemSprite::create(restartSprite, restartSprite, CC_CALLBACK_1(Over::menuCallback, this));
    restartSpriteItem->setPosition(Point(0, -180));
    restartSpriteItem->setTag(OverRestartTag);
    
    auto menu = Menu::create(shareSpriteItem, restartSpriteItem, NULL);
    this->addChild(menu);
}

//菜单回调函数
void Over::menuCallback(Ref *sender)
{
    MyTools::playEffect("move.wav");
    
    auto menuItemSprite = ((MenuItemSprite *)sender);
    auto tag = menuItemSprite->getTag();
//    log("tag = %d", tag);
    switch (tag) {
        case OverShareTag: //分享：该功能暂时没做，和重新开始一样
            Game::clearData();
            Director::getInstance()->replaceScene(TransitionFade::create(0.5f, Game::createScene()));
            break;
        case OverRestartTag: //重新开始
            Game::clearData();
            Director::getInstance()->replaceScene(TransitionFade::create(0.5f, Game::createScene()));
            break;
        default:
            break;
    }
}





