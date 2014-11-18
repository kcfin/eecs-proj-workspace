//
//  project1.h
//  project1
//
//  Created by Katelyn Findlay on 9/21/14.
//  Copyright (c) 2014 eecs281. All rights reserved.
//

#ifndef __project1__project1__
#define __project1__project1__

#include <iostream>
#include <vector>
#include <fstream>
#include <deque>
#include <sstream>
#include <getopt.h>
#include <ctype.h>

using namespace std;

struct Position {
    int room;
    int row;
    int col;
    char symbol;
    bool visited;
    Position *prev;
};

struct Map {
    char inputMode;
    char routeMode;
    char outputMode;
    int roomSize;
    int roomNums;
    Position *startPos;
    Position *endPos;
};

typedef vector<vector<vector<Position>>> Castle;

void initializeMap(Castle &castle, Map &map);
void fillMap(Castle &castle, Map &map, char symbol);
void checkValidSymbol(char symbol);
void checkValidCoordinates(int row, int col, int room, Map &map);
void findPath(Castle &castle, Map &map, deque<Position*> &route);
void addPositionIfValid(Position *nextPos, Position *prevPos, deque<Position*> &route, Map &map);
void getOpts(int argc, char * argv[], Map &map);
//void testMapOut(Map &map, Castle &castle);
void retraceRoute(Map &map, deque<Position*> &printRoute);
void printMapOutput(Map &map, Castle &castle);
void printListOutput(Map &map, deque<Position*> &route);

#endif /* defined(__project1__project1__) */
