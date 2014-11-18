//
//  stockinfo.h
//  EECS281P3
//
//  Created by Kate Findlay on 11/9/14.
//  Copyright (c) 2014 Kate Findlay. All rights reserved.
//

#ifndef EECS281P3_stockinfo_h
#define EECS281P3_stockinfo_h
#include <sstream>
using namespace std;

struct StockInfo {
    bool summary = false;
    bool verbose = false;
    bool median = false;
    bool transfers = false;
    bool insider = false;
    bool ttt = false;
    long long curTime = -1;
    long long curID = 0;
    long long totalCommission = 0;
    long long moneyTransferred = 0;
    long long numTrades = 0;
    long long numSharesTraded = 0;
    ostringstream os;
};

#endif
