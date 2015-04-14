//
//  MyTools.cpp
//  2048
//
//  Created by 宋橙 on 14-6-19.
//
//

#include "MyTools.h"
#include "SimpleAudioEngine.h"

#define userDefault UserDefault::getInstance()
#define audioEngine CocosDenshion::SimpleAudioEngine::getInstance()

void MyTools::preloadEffect()
{
    audioEngine->preloadEffect("move.wav");
    audioEngine->preloadEffect("merge.wav");
}

void MyTools::playEffect(const std::string &effectName, bool force)
{
    //如果非强制播放，要去获取声音开启状态，未开启就不播放
    if (!force) {
        auto soundOff = userDefault->getBoolForKey("soundOff");
        if (soundOff) return;
    }
    //播放音效
    audioEngine->playEffect(effectName.c_str());
}