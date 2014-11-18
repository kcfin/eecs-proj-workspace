//
//  project3.cpp
//  EECS281P3
//
//  Created by Kate Findlay on 10/29/14.
//  Copyright (c) 2014 Kate Findlay. All rights reserved.
//

#include "project3.h"

int main(int argc, char * argv[]) {
    ios_base::sync_with_stdio(false);
    
//    ifstream in("test-5-v.txt");
//    cin.rdbuf(in.rdbuf()); //redirect std::cin to in.txt!
    
    StockInfo stock;
    getOpts(argc, argv, stock);
    getMode();
    runStockExhange(stock);
    printInfo(stock);
}

void getMode() {
    string mode;
    cin >> mode;
    if(mode == "TL") {
        listInput();
    } else if(mode == "PR") {
        randomInput();
    } else {
        exit(1);
    }
}

void randomInput() {
    long long genTimestamp = 0;
    int seed;
    unsigned int numOrders;
    double rate;
    char client, equity;
    string flush;
    
    cin >> flush >> seed >> flush >> numOrders >> flush >> client >> flush >> equity >> flush >> rate;
    
    mt19937 gen(seed);
    uniform_int_distribution<char> clients('a', client);
    uniform_int_distribution<char> equities('A', equity);
    exponential_distribution<> arrivals(rate);
    bernoulli_distribution buy_or_sell;
    uniform_int_distribution<> price(2, 11);
    uniform_int_distribution<> quantity(1, 30);
    
    for(unsigned int i = 0; i < numOrders; i++) {
        
        long long timestamp = genTimestamp;
        genTimestamp = genTimestamp + floor(arrivals(gen) + .5);
        string name = string("C_") + clients(gen);
        bool buy = (buy_or_sell(gen));
        string symbol = string("E_") + equities(gen);
        long long ordPrice = 5 * price(gen);
        long long quant = quantity(gen);
        
        if(traders.count(name) == 0) {
            traders.insert(pair<string, Trader>(name, Trader(name)));
        }
        if(orderBook.count(symbol) == 0) {
            orderBook.insert(pair<string, OrderBook>(symbol, OrderBook(symbol)));
        }
        
        Order order(-1, timestamp, &traders[name], symbol, buy, ordPrice, quant);
        orders.push_back(order);
    }
}

void listInput() {
    
    while(!cin.eof()) {
        string timestamp, name, buy, equity, price, quantity;
        cin >> timestamp >> name >> buy >> equity >> price >> quantity;
        
        if(timestamp == "") {
            return;
        }
        
        for(char c : timestamp) {
            if(!isdigit(c)) {
                exit(1);
            }
        }
        for(char c : name) {
            if(!isalnum(c) && c != '_') {
                exit(1);
            }
        }
        for(char c : equity) {
            if(!isalnum(c) && c != '_' && c != '.') {
                exit(1);
            }
        }
        if (stoll(timestamp) < 0) {
            exit(1);
        } else if (buy != "BUY" && buy != "SELL") {
            exit(1);
        } else if (price[0] != '$') {
            exit(1);
        } else if (quantity[0] != '#') {
            exit(1);
        } else if(equity.length() > 5) {
            exit(1);
        }
        
        price.erase(0, 1);
        quantity.erase(0,1);
        
        for(char c : price) {
            if(!isdigit(c)) {
                exit(1);
            }
        }
        
        for(char c : quantity) {
            if(!isdigit(c)) {
                exit(1);
            }
        }
        
        if(stoll(price) < 1) {
            exit(1);
        } else if (stoll(quantity) < 1) {
            exit(1);
        }
        
        if(traders.count(name) == 0) {
            traders.insert(pair<string, Trader>(name, Trader(name)));
        }
        if(orderBook.count(equity) == 0) {
            orderBook.insert(pair<string, OrderBook>(equity, OrderBook(equity)));
        }
        
        bool isBuy;
        if(buy == "BUY") {
            isBuy = true;
        } else {
            isBuy = false;
        }
        
        Order order(-1, stoll(timestamp), &traders[name], equity, isBuy, stoll(price), stoll(quantity));
        orders.push_back(order);
    }
}

void runStockExhange(StockInfo &stock) {
    long long orig = orders.size();
    
    for(long long i = 0; i < orig; i++) {
        Order *order = &orders[i];
        order->ID = stock.curID++;
        OrderBook &ob = orderBook[order->EQUITY_SYMBOL];
        addOrderToMarket(stock, order, &ob);
        ob.makeTrades(stock);
        if(stock.insider) {
            checkInsiders(stock, &ob);
        }
    }
}

void addOrderToMarket(StockInfo &stock, Order *order, OrderBook *ob) {
    
    if(order->TIMESTAMP != stock.curTime) {
        if(order->TIMESTAMP < stock.curTime) {
            exit(1);
        }
        if(stock.median) {
            printMedian(stock);
        }
        stock.curTime = order->TIMESTAMP;
    }
    
    if(order->BUY_OR_SELL) {
        ob->buys.push(order);
    } else {
        ob->sells.push(order);
    }
}

void checkInsiders(StockInfo &stock, OrderBook *ob) {
    string insider = "INSIDER_" + ob->symbol;
    long long profit;
    
    if(traders.count(insider) == 1) {
        if(ob->medianLess.size() > 0) {
            if(!ob->sells.empty()) {
                profit = ob->getMedian() * 0.9;
                if(ob->sells.top()->PRICE < profit) {
                    Order order(stock.curID++, stock.curTime, &traders[insider], ob->symbol, true, profit, ob->sells.top()->QUANTITY);
                    orders.push_back(order);
                    Order *ord = &orders[orders.size()-1];
                    ob->buys.push(ord);
                    ob->makeTrades(stock);
                }
            }
            if(!ob->buys.empty()) {
                profit = ob->getMedian() * 1.1;
                if(ob->buys.top()->PRICE > profit) {
                    Order order(stock.curID++, stock.curTime, &traders[insider], ob->symbol, false, profit, ob->buys.top()->QUANTITY);
                    orders.push_back(order);
                    Order *ord = &orders[orders.size()-1];
                    ob->sells.push(ord);
                    ob->makeTrades(stock);
                }
            }
        }
    }
}

void getOpts(int argc, char * argv[], StockInfo &stock) {
    struct option longOpts[] = {
        {"summary", no_argument, 0, 's'},
        {"verbose", no_argument, 0, 'v'},
        {"median", no_argument, 0, 'm'},
        {"transfers", no_argument, 0, 't'},
        {"insider", required_argument, 0, 'i'},
        {"ttt", required_argument, 0, 'g'},
        {"help", no_argument, 0, 'h'}
    };
    
    char opt = 0;
    int index = 0;
    
    while ((opt = getopt_long(argc, argv, "svmti:g:h", longOpts, &index)) != -1) {
        switch(opt) {
            case 's':
                stock.summary = true;
                break;
            case 'v':
                stock.verbose = true;
                break;
            case 'm':
                stock.median = true;
                break;
            case 't':
                stock.transfers = true;
                break;
            case 'i': {
                stock.insider = true;
                string s = "INSIDER_";
                s += optarg;
                traders.insert(pair<string, Trader>(s, Trader(s)));
                break;
            }
            case 'g': {
                stock.ttt = true;
                ttsSymb.push_back(optarg);
                tts.insert(pair<string, TimeTraveler>(optarg, TimeTraveler(optarg)));
                break;
            }
            case 'h':
                //print help message
                cout << "help me\n";
                exit(0);
            default:
                cout << "Flag not recognized!\n";
                exit(1);
        }
    }
}

void printInfo(StockInfo &stock) {
    
    if(stock.median) { printMedian(stock); }
    stock.os << "---End of Day---\n";
    if(stock.summary) { printSummary(stock); }
    if (stock.transfers) { printTransfers(stock); }
    if (stock.ttt) { printTimeTravelers(stock); }
    cout << stock.os.str();
    
}

void printSummary(StockInfo &stock) {
    
    stock.os << "Commission Earnings: $" << stock.totalCommission << '\n'
    << "Total Amount of Money Transferred: $" << stock.moneyTransferred << '\n'
    << "Number of Completed Trades: " << stock.numTrades << '\n'
    << "Number of Shares Traded: " << stock.numSharesTraded << '\n';
}

void printMedian(StockInfo &stock) {
    auto it = orderBook.begin();
    while(it != orderBook.end()) {
        orderBook[it->first].printMedian(stock);
        it++;
    }
}

void printTransfers(StockInfo &stock) {
    auto it = traders.begin();
    while(it != traders.end()) {
        traders[it->first].printTransfers(stock);
        it++;
    }
}

void printTimeTravelers(StockInfo &stock) {
    for(unsigned long long i = 0; i < orders.size(); i++) {
        Order *ord = &orders[i];
        if(tts.count(ord->EQUITY_SYMBOL)) {
            tts[ord->EQUITY_SYMBOL].checkOrd(ord);
        }
    }
    
    for(unsigned long long i = 0; i < ttsSymb.size(); i++) {
        tts[ttsSymb[i]].printInfo(stock);
    }
}
