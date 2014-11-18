//
//  timetraveler.h
//  EECS281P3
//
//  Created by Kate Findlay on 11/9/14.
//  Copyright (c) 2014 Kate Findlay. All rights reserved.
//

#ifndef EECS281P3_timetraveler_h
#define EECS281P3_timetraveler_h
#include <sstream>
#include <limits.h>
#include "order.h"
using namespace std;


struct TimeTraveler {
    
    TimeTraveler () {}
    
    TimeTraveler(string name) : equity(name) {}
    
    string equity;
    long long profit = 0;
    Order *buy = nullptr;
    Order *potentialBuy = nullptr;
    Order *sell = nullptr;
    
    void checkOrd(Order *order) {
        if(!order->BUY_OR_SELL) {
            if(potentialBuy == nullptr || (order->PRICE < potentialBuy->PRICE)) {
                potentialBuy = order;
            }
        } else if(potentialBuy != nullptr) {
            if(profit == 0 || (order->PRICE - potentialBuy->PRICE > profit)) {
                sell = order;
                profit = sell->PRICE - potentialBuy->PRICE;
                buy = potentialBuy;
            }
        }
    }
    
    void printInfo(StockInfo &stock) {
        long long buyTime, sellTime;
        
        if(buy == nullptr) {
            buyTime = -1;
            sellTime = -1;
        } else {
            buyTime = buy->TIMESTAMP;
            sellTime = sell->TIMESTAMP;
        }
        
        stock.os << "Time travelers would buy " << equity << " at time: " << buyTime << " and sell it at time: " << sellTime << '\n';
    }
};

#endif
