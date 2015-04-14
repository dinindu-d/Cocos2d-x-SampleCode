//
//  Box.h
//  2048
//
//  Created by 宋橙 on 14-6-12.
//
//

#ifndef ___048__Box__
#define ___048__Box__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;
class Station;

class Box : public Sprite
{
public:
    static Box * boxWithStation(Station *_station, int _number);
    
private:
    void boxInit(Station *_station, int _number);
    
public:
    int number;
    Station *station;
    bool hasCombined;
    
};

#endif /* defined(___048__Box__) */
