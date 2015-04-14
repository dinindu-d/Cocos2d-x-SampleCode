//
//  Station.h
//  2048
//
//  Created by 宋橙 on 14-6-12.
//
//

#ifndef ___048__Station__
#define ___048__Station__

#include <iostream>
#include "cocos2d.h"

class Box;

USING_NS_CC;

class Station : public Node
{
public:
    static Station *stationWithPosition(Point _position, int _line, int _column);
    
private:
    bool initWithPosition(Point _position, int _line, int _column);
    
public:
    Point position;
    int line;
    int column;
    Box *box;
    
};
#endif /* defined(___048__Station__) */
