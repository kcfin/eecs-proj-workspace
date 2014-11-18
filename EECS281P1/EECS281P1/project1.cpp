//
//  proj1.cpp
//  project1
//
//  Created by Katelyn Findlay on 9/17/14.
//  Copyright (c) 2014 kcfin. All rights reserved.
//

#include "project1.h"

int main(int argc, char * argv[]) {
    Map map;
    Castle castle;
    deque<Position*> route;
    deque<Position*> printRoute;
    
//    ifstream in("test-9.txt");
//    cin.rdbuf(in.rdbuf()); //redirect std::cin to in.txt!
    
    getOpts(argc, argv, map);
    initializeMap(castle, map);
    findPath(castle, map, route);
    retraceRoute(map, printRoute);
    if(map.outputMode == 'M') {
        printMapOutput(map, castle);
    } else {
        printListOutput(map, printRoute);
    }
}

void initializeMap(Castle &castle, Map &map) {
    cin >> map.inputMode >> map.roomSize >> map.roomNums;
    cin.ignore(1);
    fillMap(castle, map, '.');
    
    string inString;
    int rowNum = 0;
    
    while(getline(cin, inString)){
        
        if(inString[0] == '/' && inString[1] == '/') {
            continue;
        } else if(inString[0] == ' ' || inString[0] == '\0') {
            break;
        }
        
        switch (map.inputMode){
            case 'M': {
                for(int i = 0; i < map.roomSize; i++) {
                    int room = rowNum/map.roomSize;
                    int row = rowNum % map.roomSize;
                    int col = i;
                    char symbol = inString[i];
                    checkValidSymbol(symbol);
                    castle[room][row][col].row = row;
                    castle[room][row][col].col = col;
                    castle[room][row][col].room = room;
                    castle[room][row][col].symbol = symbol;
                    castle[room][row][col].visited = false;
                    if(symbol == 'S') {
                        map.startPos = &castle[room][row][col];
                        map.startPos->visited = true;
                    } else if (symbol == 'R') {
                        map.endPos = &castle[room][row][col];
                    }
                }
                rowNum++;
                break;
            }
            case 'L': {
                int row, col, room;
                char symbol;
                sscanf(inString.c_str(), "(%d, %d, %d, %c)", &row, &col, &room, &symbol);
                checkValidSymbol(symbol);
                checkValidCoordinates(row, col, room, map);
                castle[room][row][col].row = row;
                castle[room][row][col].col = col;
                castle[room][row][col].room = room;
                castle[room][row][col].symbol = symbol;
                castle[room][row][col].visited = false;
                if(symbol == 'S') {
                    map.startPos = &castle[room][row][col];
                    map.startPos->visited = true;
                } else if (symbol == 'R') {
                    map.endPos = &castle[room][row][col];
                }
                break;
            }
        }
    }
}

void fillMap(Castle &castle, Map &map, char symbol) {
    
    castle = vector<vector<vector<Position>>>(map.roomNums, vector<vector<Position>>(map.roomSize, vector<Position>(map.roomSize, Position())));
    
    for(int i = 0; i < map.roomNums; i++) {
        for(int j = 0; j < map.roomSize; j++) {
            for(int k = 0; k < map.roomSize; k++) {
                castle[i][j][k].row = j;
                castle[i][j][k].col = k;
                castle[i][j][k].room = i;
                castle[i][j][k].symbol = symbol;
                castle[i][j][k].visited = false;
            }
        }
    }
}

void checkValidSymbol(char symbol) {
    if(!(symbol == '.' || symbol == '#' || symbol == 'R' || symbol == 'S')
       && !isdigit(symbol)) {
        cerr << "Invalid symbol!" << endl;
        exit(1);
    }
}

void checkValidCoordinates(int row, int col, int room, Map &map) {
    if(row >= map.roomSize || col >= map.roomSize || room >= map.roomNums) {
        cerr << "Invalid coordinates!" << endl;
        exit(1);
    }
}

void findPath(Castle &castle, Map &map, deque<Position*> &route) {
    
    route.push_front(map.startPos);
    Position *curPos;
    Position *nextPos;
    
    while(!route.empty()) {
        
        curPos = route.front();
        route.pop_front();
        
        //check north
        if(curPos->row - 1 >= 0) {
            nextPos = &castle[curPos->room][curPos->row - 1][curPos->col];
            addPositionIfValid(nextPos, curPos, route, map);
            if(nextPos->symbol == 'R') {
                return;
            }
        }
        
        //check east
        if(curPos->col + 1 < map.roomSize){
            nextPos = &castle[curPos->room][curPos->row][curPos->col + 1];
            addPositionIfValid(nextPos, curPos, route, map);
            if(nextPos->symbol == 'R') {
                return;
            }
        }
        
        //check south
        if(curPos->row + 1 < map.roomSize) {
            nextPos = &castle[curPos->room][curPos->row + 1][curPos->col];
            addPositionIfValid(nextPos, curPos, route, map);
            if(nextPos->symbol == 'R') {
                return;
            }
        }
        
        //check west
        if(curPos->col - 1 >= 0) {
            nextPos = &castle[curPos->room][curPos->row][curPos->col - 1];
            addPositionIfValid(nextPos, curPos, route, map);
            if(nextPos->symbol == 'R') {
                return;
            }
        }
        
        //check if current position is a portal
        if(isdigit(curPos->symbol)) {
            int newRoom = curPos->symbol - '0';
            if(newRoom < map.roomNums) {
                nextPos = &castle[newRoom][curPos->row][curPos->col];
                addPositionIfValid(nextPos, curPos, route, map);
                if(nextPos->symbol == 'R') {
                    return;
                }
            }
        }
    }
}

void addPositionIfValid(Position *nextPos, Position *prevPos, deque<Position*> &route, Map &map) {
    if(nextPos->symbol != '#' && !nextPos->visited) {
        if(map.routeMode == 's') {
            route.push_front(nextPos);
        } else {
            route.push_back(nextPos);
        }
        nextPos->visited = true;
        nextPos->prev = prevPos;
    }
}

void getOpts(int argc, char * argv[], Map &map) {
    struct option longOpts[] = {
        {"stack", no_argument, 0, 's'},
        {"queue", no_argument, 0, 'q'},
        {"output", required_argument, 0, 'o'}
    };
    
    char opt = 0;
    int index = 0;
    map.outputMode = 'M';
    map.routeMode = 0;
    
    while ((opt = getopt_long(argc, argv, "sqo:h", longOpts, &index)) != -1) {
        switch(opt) {
            case 's':
            case 'q':
                if(map.routeMode != 0) {
                    cerr << "Can only choose one routing scheme!" << '\n';
                    exit(1);
                } else {
                    map.routeMode = opt;
                }
                break;
            case 'o':
                map.outputMode = *optarg;
                break;
            case 'h':
                //print help message
                cout << "help me\n";
                exit(0);
            default:
                cout << "Flag not recognized!\n";
        }
    }
    
    if(map.routeMode == 0) {
        cerr << "Routing scheme not chosen!" << '\n';
        exit(1);
    }
}

//void testMapOut(Map &map, Castle &castle) {
//    ostringstream os;
//    os << map.roomSize << '\n';
//    os << map.roomNums << '\n';
//    
//    for(int i = 0; i < map.roomNums; i++) {
//        os << "//room " << i << '\n';
//        for(int j = 0; j < map.roomSize; j++) {
//            for(int k = 0; k < map.roomSize; k++) {
//                os << castle[i][j][k].symbol;
//            }
//            os << '\n';
//        }
//    }
//    
//    cout << os.str();
//}

void retraceRoute(Map &map, deque<Position*> &printRoute) {
    
    if(map.endPos->prev == nullptr) {
        return;
    }
    
    Position *curPos = map.endPos;
    Position *changedPos;
    
    while(curPos != map.startPos) {
        changedPos = curPos->prev;
        
        if(curPos->row - changedPos->row == -1) {
            changedPos->symbol = 'n';
        } else if (curPos->row - changedPos->row) {
            changedPos->symbol = 's';
        } else if(curPos->col - changedPos->col == -1) {
            changedPos->symbol = 'w';
        } else if(curPos->col - changedPos->col) {
            changedPos->symbol = 'e';
        } else {
            changedPos->symbol = 'p';
        }
        if(map.outputMode == 'L') {
            printRoute.push_front(changedPos);
        }
        curPos = changedPos;
    }
    
}

void printMapOutput(Map &map, Castle &castle) {
    ostringstream os;
    os << map.roomSize << '\n';
    os << map.roomNums << '\n';
    
    for(int i = 0; i < map.roomNums; i++) {
        os << "//room " << i << '\n';
        for(int j = 0; j < map.roomSize; j++) {
            for(int k = 0; k < map.roomSize; k++) {
                os << castle[i][j][k].symbol;
            }
            os << '\n';
        }
    }
    
    cout << os.str();
}

void printListOutput(Map &map, deque<Position*> &printRoute) {
    ostringstream os;
    os << map.roomSize << '\n';
    os << map.roomNums << '\n';
    os << "//path taken" << '\n';
    
    while(!printRoute.empty()) {
        Position *printPos = printRoute.front();
        printRoute.pop_front();
        os << "(" << printPos->row << "," << printPos->col << "," << printPos->room << "," << printPos->symbol << ")" << '\n';
    }
    
    cout << os.str();
    
}
