//
//  project2.cpp
//  project2
//
//  Created by Kate Findlay on 10/6/14.
//  Copyright (c) 2014 Kate Findlay. All rights reserved.
//

#include "proj2.h"

int main(int argc, char * argv[]) {
    MineInfo mineInfo;
    Mine mine;
    getOpts(argc, argv, mineInfo);
    initializeMine(mine, mineInfo);
    findPath(mine, mineInfo);
    //    printMine(mine, mineInfo);
    output(mineInfo);
    if(mineInfo.stats > 0) {
        statsOutput(mineInfo);
    }
}

void fillMine(Mine &mine, MineInfo &mineInfo) {
    mine = vector<vector<Tile>>(mineInfo.size, vector<Tile>(mineInfo.size, Tile()));
    
    for(int i = 0; i < mineInfo.size; i++) {
        for(int j = 0; j < mineInfo.size; j++) {
            mine[i][j].row = i;
            mine[i][j].col = j;
            mine[i][j].rubbleVal = 0;
            mine[i][j].cleared = false;
            mine[i][j].visited = false;
        }
    }
}

void initializeMine(Mine &mine, MineInfo &mineInfo) {
    
    ifstream inFile(mineInfo.mineFile);
    int startRow = 0;
    int startCol = 0;
    string temp;
    
    if(!inFile.is_open()) {
        cerr << "Cannot open file!" << endl;
        exit(1);
    }
    
    string inputFlush;
    inFile >> mineInfo.inputMode;
    inFile >> inputFlush >> mineInfo.size;
    inFile >> inputFlush >> startRow >> startCol;
    
    fillMine(mine, mineInfo);
    
    if(mineInfo.inputMode == 'R') {
        int seed;
        int maxRubble;
        int tntChance;
        inFile >> inputFlush >> seed;
        inFile >> inputFlush >> maxRubble;
        inFile >> inputFlush >> tntChance;
        
        P2random::seed_mt(seed);
        
        for(int i = 0; i < mineInfo.size; i++) {
            for(int j = 0; j < mineInfo.size; j++) {
                int val = P2random::generate_tile(maxRubble, tntChance);
                mine[i][j].row = i;
                mine[i][j].col = j;
                mine[i][j].rubbleVal = val;
                
                if(val == 0) {
                    mine[i][j].cleared = true;
                } else {
                    mine[i][j].cleared = false;
                }
            }
        }
    } else {
        int index = 0;
        int mineSize = mineInfo.size * mineInfo.size;
        
        while(index < mineSize) {
            int row = index/mineInfo.size;
            int col = index % mineInfo.size;
            int val;
            mine[row][col].row = row;
            mine[row][col].col = col;
            inFile >> val;
            mine[row][col].rubbleVal = val;
            if(val == 0) {
                mine[row][col].cleared = true;
            } else {
                mine[row][col].cleared = false;
            }
            index++;
        }
    }
    inFile.close();
    
    mineInfo.start = &mine[startRow][startCol];
    mineInfo.start->visited = true;
    mineInfo.tilesCleared = 0;
    mineInfo.rubbleCleared = 0;
    mineInfo.end = false;
    
}

void findPath(Mine &mine, MineInfo &mineInfo) {
    pqueue->push(mineInfo.start);
    
    while(!mineInfo.end) {
//        pqueue->fix();
        Tile *cur = pqueue->top();
        pqueue->pop();
        tntQueue->push(cur);
        
        while(!tntQueue->empty()) {
            Tile *clear;
            clear = tntQueue->top();
            tntQueue->pop();
            clearCurTile(clear, mine, mineInfo);
        }
        
        mineInfo.end = isEdge(cur, mineInfo.size);
    }
}

void clearCurTile(Tile *cur, Mine &mine, MineInfo &mineInfo) {

    if(cur->rubbleVal == -1) {
        addToTNT(cur, mine, mineInfo);
    } else {
        if(!cur->cleared) {
            addToPath(cur, mine, mineInfo);
            mineInfo.tilesCleared++;
            mineInfo.rubbleCleared += cur->rubbleVal;
        }
    }
    
    
    if(!cur->cleared) {
        clearedTiles.push_back(cur);
        easyOutput->push(cur);
        hardOutput->push(cur);
        cur->cleared = true;
    }
}

void addToTNT(Tile *cur, Mine &mine, MineInfo &mineInfo) {
    Tile *next;
    if(checkNorth(cur)) {
        next = getNorth(cur, mine);
        if(!next->cleared) {
            tntQueue->push(next);
        }
    }
    if(checkEast(cur, mineInfo.size)) {
        next = getEast(cur, mine);
        if(!next->cleared) {
            tntQueue->push(next);
        }
        
    }
    if(checkSouth(cur, mineInfo.size)) {
        next = getSouth(cur, mine);
        if(!next->cleared) {
            tntQueue->push(next);
        }
    }
    if(checkWest(cur)) {
        next = getWest(cur, mine);
        if(!next->cleared) {
            tntQueue->push(next);
        }
    }
}

void addToPath(Tile *cur, Mine &mine, MineInfo &mineInfo) {
    Tile *next;
    if(checkNorth(cur)) {
        next = getNorth(cur, mine);
        if(!next->visited) {
            next->visited = true;
            pqueue->push(next);
        }
        
    }
    if(checkEast(cur, mineInfo.size)) {
        next = getEast(cur, mine);
        if(!next->visited) {
            next->visited = true;
            pqueue->push(next);
        }
        
    }
    if(checkSouth(cur, mineInfo.size)) {
        next = getSouth(cur, mine);
        if(!next->visited) {
            next->visited = true;
            pqueue->push(next);
        }
    }
    if(checkWest(cur)) {
        next = getWest(cur, mine);
        if(!next->visited) {
            next->visited = true;
            pqueue->push(next);
        }
    }
}

void getOpts(int argc, char * argv[], MineInfo &mineInfo) {
    struct option longOpts[] = {
        {"stats", optional_argument, 0, 's'},
        {"container", required_argument, 0, 'c'}
    };
    
    char opt = 0;
    int index = 0;
    mineInfo.stats = 0;
    easyOutput = &easyBinary;
    hardOutput = &hardBinary;
    
    while ((opt = getopt_long(argc, argv, "c:s:h", longOpts, &index)) != -1) {
        switch(opt) {
            case 's':
                mineInfo.stats = atoi(optarg);
                break;
            case 'c':
                if(!strcmp(optarg, "POOR_MAN")) {
                    pqueue = &poorman;
                    tntQueue = &poormanTNT;
                } else if (!strcmp(optarg, "SORTED")) {
                    pqueue = &sorted;
                    tntQueue = &sortedTNT;
                } else if (!strcmp(optarg, "BINARY")) {
                    pqueue = &binary;
                    tntQueue = &binaryTNT;
                } else if (!strcmp(optarg, "PAIRING")) {
//                    pqueue = &pairing;
//                    tntQueue = &pairingTNT;
                }
                break;
            case 'h':
                //print help message
                cout << "help me\n";
                exit(0);
            default:
                cout << "Flag not recognized!\n";
                exit(1);
        }
    }
    
    if (optind < argc) {
        mineInfo.mineFile = argv[optind++];
    }
}

void printMine(Mine &mine, MineInfo mineInfo) {
    ostringstream os;
    os << mineInfo.inputMode << '\n';
    os << mineInfo.size << '\n';
    os << "Mine: " << '\n';
    
    for(int i = 0; i < mineInfo.size; i++) {
        for(int j = 0; j < mineInfo.size; j++) {
            int rubbleVal = mine[i][j].rubbleVal;
            if(mine[i][j].cleared == true) {
                rubbleVal = 0;
            }
            os << rubbleVal << " ";
        }
        os << '\n';
    }
    
    os << "Tiles Cleared: " << mineInfo.tilesCleared << '\n';
    
    cout << os.str();
}

void output(MineInfo &mineInfo) {
    ostringstream os;
    os << "Cleared " << mineInfo.tilesCleared <<
    " tiles containing " << mineInfo.rubbleCleared <<
    " rubble and escaped." <<  '\n';
    
    cout << os.str();
    
}

void statsOutput(MineInfo &mineInfo) {
    ostringstream os;
    
    //    if((int)clearedTiles.size() < mineInfo.stats) {
    //        mineInfo.stats = (int)clearedTiles.size();
    //    }
    
    mineInfo.stats = (int)clearedTiles.size();
    
    os << "First tiles cleared:\n";
    for(int i = 0; i < mineInfo.stats; i++) {
        
        if(clearedTiles[i]->rubbleVal == -1) {
            os << "TNT";
        } else {
            os << clearedTiles[i]->rubbleVal;
        }
        
        os << " at [" << clearedTiles[i]->row << "," << clearedTiles[i]->col << "]\n";
    }
    
    os << "Last tiles cleared:\n";
    for(int i = mineInfo.stats-1; i >= 0; i--) {
        
        if(clearedTiles[i]->rubbleVal == -1) {
            os << "TNT";
        } else {
            os << clearedTiles[i]->rubbleVal;
        }
        
        os << " at [" << clearedTiles[i]->row << "," << clearedTiles[i]->col << "]\n";
    }
    
    os << "Easiest tiles cleared:\n";
    while(!easyOutput->empty()) {
        Tile *temp = easyOutput->top();
        easyOutput->pop();
        
        if(temp->rubbleVal == -1) {
            os << "TNT";
        } else {
            os << temp->rubbleVal;
        }
        
        os << " at [" << temp->row << "," << temp->col << "]\n";
    }
    
    os << "Hardest tiles cleared:\n";
    while(!hardOutput->empty()) {
        Tile *temp = hardOutput->top();
        hardOutput->pop();
        
        if(temp->rubbleVal == -1) {
            os << "TNT";
        } else {
            os << temp->rubbleVal;
        }
        
        os << " at [" << temp->row << "," << temp->col << "]\n";
    }
    
    cout << os.str();
}
