//
//  InstructorScene.cpp
//  2048
//
//  Created by 宋橙 on 14-6-18.
//
//

#include "InstructorScene.h"
#include "MyTools.h"

#define VISIBLE_WIDTH Director::getInstance()->getVisibleSize().width
#define VISIBLE_HEIGHT Director::getInstance()->getVisibleSize().height

Scene* Instructor::createScene()
{
    auto scene = Scene::create();
    auto layer = Instructor::create();
    scene->addChild(layer);
    return scene;
}

bool Instructor::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    initData();
    
    return true;
}

//初始化各项数据
void Instructor::initData()
{
    //加载背景
    loadBackground();
    //加载分页视图
    loadPageView();
    //加载页码点
    loadPageDot();
    //设置页码状态
    changePageDotFrame();
}

//加载背景
void Instructor::loadBackground()
{
    auto background = Sprite::createWithSpriteFrameName("bg_blank.png");
    background->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
    addChild(background);
}

//加载分页视图
void Instructor::loadPageView()
{
    pageView = PageView::create();
    pageView->setSize(Size(VISIBLE_WIDTH, VISIBLE_HEIGHT));
    for (int i = 0; i < 3; i++) {
        auto pageLayout = Layout::create();
        pageLayout->setSize(Size(pageView->getSize().width, pageView->getSize().height));
        
        auto frameName = StringUtils::format("instructor_%d.png", i+1);
        auto pageSprite = Sprite::createWithSpriteFrameName(frameName);
        pageSprite->setAnchorPoint(Point::ANCHOR_TOP_LEFT);
        pageSprite->setPosition(Point(10, pageLayout->getSize().height-10));
        pageLayout->addChild(pageSprite);
        
        if (i == 2) {
            auto startSprite = Sprite::createWithSpriteFrameName("btn_start.png");
            auto startSpriteItem = MenuItemSprite::create(startSprite, startSprite, CC_CALLBACK_1(Instructor::startCallback, this));
            startSpriteItem->setPosition(Point(0, -280));
            
            auto startMenu = Menu::create(startSpriteItem, NULL);
            pageLayout->addChild(startMenu);
        }
        
        pageView->addPage(pageLayout);
    }
    
    pageView->addEventListenerPageView(this, pagevieweventselector(Instructor::pageViewEvent));
    addChild(pageView);
}

//加载页码点
void Instructor::loadPageDot()
{
    dotLayout = Layout::create();
    dotLayout->setSize(Size(VISIBLE_WIDTH, VISIBLE_HEIGHT));
    for (int i = 0; i < 3; i++) {
        auto pageDot = Sprite::createWithSpriteFrameName("dot_normal.png");
        pageDot->setTag(i+1);
        auto dotWidth = pageDot->getContentSize().width;
        auto x = VISIBLE_WIDTH/2 + 2.5*dotWidth*(i-1);
        pageDot->setPosition(x, 50);
        dotLayout->addChild(pageDot);
    }
    this->addChild(dotLayout);
}

//设置页码状态
void Instructor::changePageDotFrame()
{
    auto page = (int)pageView->getCurPageIndex() + 1;
    auto cache = SpriteFrameCache::getInstance();
    for (int i = 1; i <= 3; i++) {
        auto pageDot = (Sprite *)dotLayout->getChildByTag(i);
        auto frameName = i==page ? "dot_selected.png" : "dot_normal.png";
        auto frame = cache->getSpriteFrameByName(frameName);
        pageDot->setDisplayFrame(frame);
    }
}

//分页视图事件监听回调函数
void Instructor::pageViewEvent(cocos2d::Ref *sender, PageViewEventType type)
{
    switch (type) {
        case cocos2d::ui::PAGEVIEW_EVENT_TURNING:
        {
            changePageDotFrame();
        }
            break;
        default:
            break;
    }
}

//开始菜单回调函数
void Instructor::startCallback(Ref *sender)
{
    MyTools::playEffect("move.wav");
    Director::getInstance()->popScene();
}







