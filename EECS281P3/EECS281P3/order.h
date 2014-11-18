//
//  order.h
//  EECS281P3
//
//  Created by Kate Findlay on 10/29/14.
//  Copyright (c) 2014 Kate Findlay. All rights reserved.
//

#ifndef EECS281P3_order_h
#define EECS281P3_order_h
#include <stdio.h>
#include <string>
using namespace std;
#include "trader.h"

class Order {
    
public:
    long long ID = 0;
    long long TIMESTAMP = 0;
    Trader *trader;
    string EQUITY_SYMBOL;
    bool BUY_OR_SELL = false;
    long long PRICE = 0;
    long long QUANTITY = 0;
    
    Order(long long inID, long long inTime, Trader* inTrader, string inEquity, bool inBuy, long long inPrice, long long inQuantity) {
        ID = inID;
        TIMESTAMP = inTime;
        trader = inTrader;
        EQUITY_SYMBOL = inEquity;
        BUY_OR_SELL = inBuy;
        PRICE = inPrice;
        QUANTITY = inQuantity;
    }
};


#endif
