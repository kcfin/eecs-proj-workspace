//
//  trader.h
//  EECS281P3
//
//  Created by Kate Findlay on 10/29/14.
//  Copyright (c) 2014 Kate Findlay. All rights reserved.
//

#ifndef __EECS281P3__trader__
#define __EECS281P3__trader__
#include <stdio.h>
#include <string>
#include <sstream>
#include "stockinfo.h"
using namespace std;

class Trader {
public:
    string name = "";
    long long stocksBought = 0;
    long long stocksSold = 0;
    long long netValue = 0;
    
    Trader() {}
    
    Trader(string inName) : name(inName) {}
    
    void updateValues(bool buy, long long quantity, long long price) {
        if(buy) {
            stocksBought += quantity;
            netValue -= price*quantity;
        } else {
            stocksSold += quantity;
            netValue += price*quantity;
        }
    }
    
    void printTransfers(StockInfo &stock) {
        stock.os << name << " bought " << stocksBought << " and sold " << stocksSold << " for a net transfer of $" << netValue << '\n';
    }
};

#endif /* defined(__EECS281P3__trader__) */
