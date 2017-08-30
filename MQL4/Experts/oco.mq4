//+------------------------------------------------------------------+
//|                                                          oco.mq4 |
//|                                                         rami1942 |
//|                                     https://github.com/rami1942/ |
//+------------------------------------------------------------------+
#property copyright "rami1942"
#property link      "https://github.com/rami1942/"
#property version   "1.00"
#property strict

#include <Odbc.mqh>
#include <ramilib.mqh>
//--- input parameters
input string   dsName="fxc";
input string   userName="";
input string   password="";


/*
create table oco (
   id integer primary key autoincrement,
   
   price1 real,
   price2 real,
   
   ticket1 int,
   ticket2 int,
   
   current_ticket int,
   
   lots real not null,
   buy int not null default 1,
   tp real not null default 0.0,
   sl real not null default 0.0,
   
   next_delta1 real,
   next_delta2 real
   
);

*/

//+------------------------------------------------------------------+
//| Expert initialization function                                   |
//+------------------------------------------------------------------+
int OnInit() {
   if (!OdbcInitEnv()) {
      return (INIT_FAILED);
   }
   // In the case of SQLite3, 
   if (!OdbcConnect(dsName, userName, password)) {
      return (INIT_FAILED);
   }
   

   return(INIT_SUCCEEDED);
}
//+------------------------------------------------------------------+
//| Expert deinitialization function                                 |
//+------------------------------------------------------------------+
void OnDeinit(const int reason) {
//---
   OdbcDisconnect();
   OdbcDeInitEnv(); 
}
//+------------------------------------------------------------------+
//| Expert tick function                                             |
//+------------------------------------------------------------------+
void OnTick() {
//---
   processOCO();

}
//+------------------------------------------------------------------+

void processOCO() {

   ulong sth = OdbcQuery("select price1,price2, ticket1, ticket2, current_ticket, lots, buy, tp, sl, next_delta1, next_delta2, id from oco where valid=1");
   if (sth == 0) {
      Print("Query failed.");
      Print(OdbcErrorCode(), OdbcErrorMsg());
      return;
   }
   OdbcBindColDouble(sth); //  1:price1
   OdbcBindColDouble(sth); //  2:price2
   OdbcBindColInt(sth);    //  3:ticket1
   OdbcBindColInt(sth);    //  4:ticket2
   OdbcBindColInt(sth);    //  5:current_ticket
   OdbcBindColDouble(sth); //  6:lots
   OdbcBindColInt(sth);    //  7:buy
   OdbcBindColDouble(sth); //  8:tp
   OdbcBindColDouble(sth); //  9:sl
   OdbcBindColDouble(sth); // 10:next_delta1
   OdbcBindColDouble(sth); // 11:next_delta2
   OdbcBindColInt(sth);    // 12:id

   while(OdbcFetch(sth)) {
   
      // current_ticket -> price1, price2
      if (!OdbcIsNull(sth, 5)) {
         int ticket = OdbcGetColInt(sth, 5);
         if (IsOrderClosed(ticket)) {
            updateNextPrice(sth);
         }
      }

      // ticket1, ticket2 -> current_ticket
      int pendTicket1 = OdbcGetColInt(sth, 3);
      int pendTicket2 = OdbcGetColInt(sth, 4);
      if (!OdbcIsNull(sth, 3) && !IsOrderPending(pendTicket1)) {
         moveToPosition(sth, pendTicket1, pendTicket2);
      } else if (!OdbcIsNull(sth, 4) && !IsOrderPending(pendTicket2)) {
         moveToPosition(sth, pendTicket2, pendTicket1);
      } else {
         // if price is set and not ordered, order it.
         
         // Even though moveToPosition is executed, OdbcGetColInt(sth,5) (current_ticket) IS NOT NULL because it is in buffer.
         orderOCO(sth, 1, 3);
      }
      
   }   
   OdbcCloseStmt(sth);
}

void orderOCO(ulong sth, int col1, int col2) {

   if (!OdbcIsNull(sth, 5)) {
      return;
   }

   double price1 = OdbcGetColDouble(sth, 1);
   double price2 = OdbcGetColDouble(sth, 2);

   double lots = OdbcGetColDouble(sth, 6);
   int buy = OdbcGetColInt(sth, 7);
   double tp = OdbcGetColDouble(sth, 8);
   double sl = OdbcGetColDouble(sth, 9);
   int id = OdbcGetColInt(sth, 12);

   string comment = StringConcatenate("oco ID=", id);
   
   if (!OdbcIsNull(sth, 1) && OdbcIsNull(sth, 3)) {
      int ticket = processOrder(price1, lots, 1, FALSE, 0, tp, sl, comment);
      if (ticket == -1) {
         Print("Can't order ", price1, " ", ErrorDescription(GetLastError()));
         string sql = StringConcatenate("update oco set ticket1=-1,price1=NULL where id=", id);
         if (!OdbcExecute(sql)) {
            Print("update ticket1 failed.", OdbcErrorCode(), " ", OdbcErrorMsg());
         }
      } else {
         string sql = StringConcatenate("update oco set ticket1=", ticket, ",price1=NULL where id=", id);
         if (!OdbcExecute(sql)) {
            Print("update ticket1 failed.", OdbcErrorCode(), " ", OdbcErrorMsg());
         }
      }
   }
   
   if (!OdbcIsNull(sth, 2) && OdbcIsNull(sth, 4)) {
      int ticket = processOrder(price2, lots, 1, FALSE, 0, tp, sl, comment);
      if (ticket == -1) {
         Print("Can't order ", price2, " ", ErrorDescription(GetLastError()));
         string sql = StringConcatenate("update oco set ticket2=-1, price2=NULL where id=", id);
         if (!OdbcExecute(sql)) {
            Print("update ticket1 failed.", OdbcErrorCode(), " ", OdbcErrorMsg());
         }
      } else {
         string sql = StringConcatenate("update oco set ticket2=",ticket, ",price2=NULL where id=", id);
         if (!OdbcExecute(sql)) {
            Print("update ticket1 failed.", OdbcErrorCode(), " ", OdbcErrorMsg());
         }
      }
   }  
   
}

void moveToPosition(ulong sth, int ticket, int oco) {
   Print("Delete oco position ", oco);
   if (IsOrderPending(oco) && !OrderDelete(oco)) {
      Print("Can't delete ticket ", oco, " ", ErrorDescription(GetLastError()));
   }

   Print("Set pending ticket ", ticket, " to current");
   int id = OdbcGetColInt(sth, 12);
   string sql = StringConcatenate("update oco set current_ticket=", ticket, ", ticket1=NULL, ticket2=NULL where id=", id);
   if (!OdbcExecute(sql)) {
      Print("Update failed.", OdbcErrorCode(), " ", OdbcErrorMsg());
   }
}

// update price1 and price2
void updateNextPrice(ulong sth) {

   double openPrice = OrderOpenPrice();
   double lots = OdbcGetColDouble(sth, 6);
   int buy = OdbcGetColInt(sth, 7);
   double tp = OdbcGetColDouble(sth, 8);
   double sl = OdbcGetColDouble(sth, 9);
   double next_delta1 = OdbcGetColDouble(sth, 10);
   double next_delta2 = OdbcGetColDouble(sth, 11);
   int id = OdbcGetColInt(sth, 12);

   // update price1 from delta1
   if (!OdbcIsNull(sth, 10)) {
      double newPrice = OrderClosePrice() + next_delta1;
      double tpPrice = newPrice + tp;
      Print(id, ":Request Pending order1 ", newPrice, " Lots=", lots, " TP=", tpPrice);
      string sql = StringConcatenate("update oco set current_ticket=NULL, price1=", newPrice, " where id=", id);
      if (!OdbcExecute(sql)) {
         Print("Update failed.", OdbcErrorCode(), " ", OdbcErrorMsg());
      }
   }

   // update price2 from delta2
   if (!OdbcIsNull(sth, 11)) {
      double newPrice = OrderClosePrice() + next_delta2;
      double tpPrice = newPrice + tp;
      Print(id, ":Request Pending order2 ", newPrice, " Lots=", lots, " TP=", tpPrice);
      string sql = StringConcatenate("update oco set current_ticket=NULL, price2=", newPrice, " where id=", id);
      if (!OdbcExecute(sql)) {
         Print("Update failed.", OdbcErrorCode(), " ", OdbcErrorMsg());
      }
   }
}

int IsOrderPending(int ticket) {
   if (!OrderSelect(ticket, SELECT_BY_TICKET)) {
      return 0; // invalid ticket
   }
   if (OrderCloseTime() != 0) {
      return 0; // closed
   }
   int t = OrderType();
   if (t != OP_BUY && t != OP_SELL) {
      return 1;
   }
   return 0;
}

int IsOrderClosed(int ticket) {
   if (!OrderSelect(ticket, SELECT_BY_TICKET)) {
      return 0;
   }
   if (OrderCloseTime() != 0) {
      return 1;
   }
   return 0;
}
