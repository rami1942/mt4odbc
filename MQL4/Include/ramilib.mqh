//+------------------------------------------------------------------+
//|                                                      ramilib.mqh |
//|                                                         rami1942 |
//|                                     https://github.com/rami1942/ |
//+------------------------------------------------------------------+
#property copyright "rami1942"
#property link      "https://github.com/rami1942/"
#property strict

// Some useful? libraries

#include <stderror.mqh>
#include <stdlib.mqh>

#define WAIT_TIME 5

extern int       slippage=1;

color MarkColor[6] = {Red, Blue, Red, Blue, Red, Blue};


// Send a order to a price.
// *stop/*limit is automatically selected.
// 
// if stopOnly is TRUE, BUYLIMIT/SELLLIMIT order is ignored.
//
// returns ticket no or -1 if failed.
int processOrder(double openPrice, double lots, bool buy, bool stopOnly,
                  int magic, double targetPips, double stopLoss, string comment) {   
   int errCode;
   
   if (buy) {
      if (Ask <= openPrice) {
         return doOrderSend(OP_BUYSTOP, lots, openPrice, stopLoss, openPrice + targetPips, comment, magic, errCode);
      } else {
         if (!stopOnly) {
            return doOrderSend(OP_BUYLIMIT, lots, openPrice, stopLoss, openPrice + targetPips, comment, magic, errCode);
         }
      }
   } else {
      if (Bid <= openPrice) {
         if (!stopOnly) {
            return doOrderSend(OP_SELLLIMIT, lots, openPrice, stopLoss, openPrice - targetPips, comment, magic, errCode);
         }
      } else {
         return doOrderSend(OP_SELLSTOP, lots, openPrice, stopLoss, openPrice - targetPips, comment, magic, errCode);
      }
   }
   return -1;
}

// Send a order.
// This is primitive version.
//
// returns ticket no or -1 if failed.
int doOrderSend(int type, double lots, double openPrice, double stoploss, double closePrice, string comment, int magic, int &errCode) {
   openPrice = NormalizeDouble(openPrice, Digits);
   stoploss = NormalizeDouble(stoploss, Digits);
   closePrice = NormalizeDouble(closePrice, Digits);

   uint starttime = GetTickCount();

   while(true) {
      if(GetTickCount() - starttime > WAIT_TIME * 1000) {
         Print("OrderSend timeout. Check the experts log.");
         return(-1);
      }

      if(IsTradeAllowed() == true) {
         RefreshRates();
         int ticket = OrderSend(Symbol(), type, lots, openPrice, slippage, stoploss, closePrice, comment, magic, 0, MarkColor[type]);
         if( ticket > 0) {
            return(ticket);
         }

         errCode = GetLastError();
         Print("[OrderSendError] : ", errCode, " ", ErrorDescription(errCode));
         Print("price=",openPrice,": stop=",stoploss,": close=",closePrice);
         if(errCode == ERR_INVALID_PRICE || errCode == ERR_INVALID_STOPS) {
            return(-1);
         }
      }
      Sleep(100);
   }
   return(-1);
}

class OrderManager {
   int poolMagics[];
   int poolTickets[];

public:
   
   void update();
   int getTicketByMagic(int magic);
   bool isPositionOrdered(int magic);
   int getTicket(int magic);
};



// Initialize poolMagics and poolTickets.
void OrderManager::update() {
   int n = 0;
   for (int i = 0; i < OrdersTotal(); i++) {
      if (!OrderSelect(i, SELECT_BY_POS, MODE_TRADES)) continue;
      if (OrderSymbol() != Symbol()) continue;
      n++;
   }
   
   ArrayResize(poolMagics, n);
   ArrayResize(poolTickets, n);
   
   n = 0;
   for (int i = 0; i < OrdersTotal(); i++) {
      if (!OrderSelect(i, SELECT_BY_POS, MODE_TRADES)) continue;
      if (OrderSymbol() != Symbol()) continue;
      
      poolTickets[n] = OrderTicket();
      poolMagics[n] = OrderMagicNumber();
      
      n++;
   }
}

// Retrieve magic no to ticket no.
int OrderManager::getTicketByMagic(int magic) {
   for (int i = 0; i < ArraySize(poolMagics); i++) {
      if (poolMagics[i] == magic) return(poolTickets[i]);
   }
   return(-1);
}


// Is the position is ordered?
bool OrderManager::isPositionOrdered(int magic) {
   int ticket = getTicket(magic);   
   return (ticket != -1);
}


// Retrieve magic no to ticket no.
int OrderManager::getTicket(int magic) {
   for (int i = 0; i < ArraySize(poolMagics); i++) {
      if (poolMagics[i] == magic) return(poolTickets[i]);
   }
   return(-1);
}
