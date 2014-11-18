//
//  orderbook.h
//  EECS281P3
//
//  Created by kcfin on 11/4/14.
//  Copyright (c) 2014 Kate Findlay. All rights reserved.
//

#ifndef EECS281P3_orderbook_h
#define EECS281P3_orderbook_h

#include <stdio.h>
#include <string>
#include <queue>
#include <sstream>
#include <vector>
#include "order.h"
#include "stockinfo.h"
using namespace std;

class OrderBook {
private:
    struct buyOrders {
        bool operator() (const Order *ord1, const Order *ord2) const {
            if(ord1->PRICE == ord2->PRICE) {
                return ord1->ID > ord2->ID;
            } else {
                return ord1->PRICE < ord2->PRICE;
            }
        }
    };
    
    struct sellOrders {
        bool operator() (const Order *ord1, const Order *ord2) const {
            if(ord1->PRICE == ord2->PRICE) {
                return ord1->ID > ord2->ID;
            } else {
                return ord1->PRICE > ord2->PRICE;
            }
        }
    };
    
public:
    
    priority_queue<Order*, vector<Order*>, buyOrders> buys;
    priority_queue<Order*, vector<Order*>, sellOrders> sells;
    priority_queue<long long> medianLess;
    priority_queue<long long, vector<long long>, greater<long long>> medianGreater;
    long long matchPrice = 0;
    long long quantity = 0;
    string symbol = "";
    
    OrderBook() {}
    
    OrderBook(string inSym) : symbol(inSym) {}
    
    void makeTrades(StockInfo &stock) {
        while(!buys.empty() && !sells.empty()) {
            Order *sell = sells.top();
            Order *buy = buys.top();
            
            if(buy->PRICE >= sell->PRICE) {
                
                //find matchprice
                if(buy->ID < sell->ID) {
                    matchPrice = buy->PRICE;
                } else {
                    matchPrice = sell->PRICE;
                }
                
                //adjust quantities
                if(sell->QUANTITY < buy->QUANTITY) {
                    buy->QUANTITY = buy->QUANTITY - sell->QUANTITY;
                    quantity = sell->QUANTITY;
                    sells.pop();
                } else if(sell->QUANTITY > buy->QUANTITY) {
                    sell->QUANTITY = sell->QUANTITY - buy->QUANTITY;
                    quantity = buy->QUANTITY;
                    buys.pop();
                } else {
                    sells.pop();
                    buys.pop();
                    quantity = buy->QUANTITY;
                }
                
                addMedian(matchPrice);
                buy->trader->updateValues(true, quantity, matchPrice);
                sell->trader->updateValues(false, quantity, matchPrice);
                
                if(stock.verbose) {
                    printTrade(stock, buy->trader->name, sell->trader->name);
                }
                if(stock.summary) {
                    getSummaryValues(stock);
                }
                
            } else {
                return;
            }
        }
    }
    
    void getSummaryValues(StockInfo &stock) {
        stock.totalCommission += 2*((matchPrice * quantity)/100);
        stock.moneyTransferred += matchPrice * quantity;
        stock.numSharesTraded += quantity;
        stock.numTrades++;
    }
    
    void addMedian(long long matchPrice) {
        
        if(medianLess.size() == 0 || matchPrice < medianLess.top()) {
            medianLess.push(matchPrice);
        } else {
            medianGreater.push(matchPrice);
        }
        
        if(medianGreater.size() + 1 < medianLess.size()) {
            medianGreater.push(medianLess.top());
            medianLess.pop();
        }
        
        if(medianLess.size() < medianGreater.size()) {
            medianLess.push(medianGreater.top());
            medianGreater.pop();
        }
    }
    
    long long getMedian() {
        if(medianGreater.size() < medianLess.size()) {
            return medianLess.top();
        } else {
            return (medianLess.top() + medianGreater.top())/2;
        }
    }
    
    void printMedian(StockInfo &stock) {
        if(medianLess.size() > 0) {
            stock.os << "Median match price of " << symbol << " at time " << stock.curTime << " is $" << getMedian() << '\n';
        }
    }
    
    void printTrade(StockInfo &stock, string buyName, string sellName) {
        stock.os << buyName << " purchased " << quantity << " shares of " << symbol << " from " << sellName << " for $" << matchPrice << "/share\n";
    }
};


#endif
