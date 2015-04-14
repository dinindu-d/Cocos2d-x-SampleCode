//
//  MyTools.h
//  2048
//
//  Created by 宋橙 on 14-6-19.
//
//

#ifndef ___048__MyTools__
#define ___048__MyTools__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;

class MyTools
{
public:
    static void preloadEffect();
    static void playEffect(const std::string &effectName, bool force = false);
    
};


#endif /* defined(___048__MyTools__) */
