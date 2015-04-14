//
//  Station.cpp
//  2048
//
//  Created by 宋橙 on 14-6-12.
//
//

#include "Station.h"
#include "Box.h"

Station * Station::stationWithPosition(Point _position, int _line, int _column)
{
    Station *station = new Station();
    if (station && station->initWithPosition(_position, _line, _column)) {
        station->autorelease();
        return station;
    }
    CC_SAFE_DELETE(station);
    return NULL;
}

bool Station::initWithPosition(Point _position, int _line, int _column)
{
    position = _position;
    line = _line;
    column = _column;
    box = NULL;
    return true;
}