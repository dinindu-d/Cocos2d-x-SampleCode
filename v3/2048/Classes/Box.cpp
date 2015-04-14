//
//  Box.cpp
//  2048
//
//  Created by 宋橙 on 14-6-12.
//
//

#include "Box.h"
#include "Station.h"

Box * Box::boxWithStation(Station *_station, int _number)
{
    Box *box = new Box();
    auto frameName = String::createWithFormat("tile_%i.png", _number)->getCString();
//    log("%s", frameName);
    if (box && box->initWithSpriteFrameName(frameName)) {
        box->boxInit(_station, _number);
        box->autorelease();
        return box;
    }
    CC_SAFE_DELETE(box);
    return NULL;
}

void Box::boxInit(Station *_station, int _number)
{
    station = _station;
    number = _number;
    this->setPosition(station->position);
    hasCombined = false;
}






