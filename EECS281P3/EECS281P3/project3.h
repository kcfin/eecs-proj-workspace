//
//  project3.h
//  EECS281P3
//
//  Created by Kate Findlay on 10/29/14.
//  Copyright (c) 2014 Kate Findlay. All rights reserved.
//

#ifndef __EECS281P3__project3__
#define __EECS281P3__project3__
#include <stdio.h>
#include <map>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <getopt.h>
#include <random>
#include "stockinfo.h"
#include "trader.h"
#include "orderbook.h"
#include "order.h"
#include "timetraveler.h"
using namespace std;

vector<Order> orders;
vector<string> ttsSymb;
map<string, Trader> traders;
map<string, OrderBook> orderBook;
unordered_map<string, TimeTraveler> tts;

// input
void getOpts(int argc, char * argv[], StockInfo &stock);
void getMode();
void listInput();
void randomInput();

// run the stock market
void runStockExhange(StockInfo &stock);
void addOrderToMarket(StockInfo &stock, Order *order, OrderBook *ob);
void checkInsiders(StockInfo &stock, OrderBook *ob);
void timeTravelerInfo(Order *order);

// print options
void printInfo(StockInfo &stock);
void printMedian(StockInfo &stock);
void printTimeTravelers(StockInfo &stock);
void printSummary(StockInfo &stock);
void printTransfers(StockInfo &stock);

#endif /* defined(__EECS281P3__project3__) */
