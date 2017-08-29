//+------------------------------------------------------------------+
//|                                                       export.mq4 |
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
input string   dsName="usdjpy";
input string   userName="";
input string   password="";

/*
create table usdjpy (
   time text primary key,
   open real not null,
   high real not null,
   low real not null,
   close real not null
);
*/

//+------------------------------------------------------------------+
//| Script program start function                                    |
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

void OnDeinit(const int reason) {
//---
   OdbcDisconnect();
   OdbcDeInitEnv(); 
}

void OnStart() {
//---
   string symbol = "USDJPY";
   int timeframe = PERIOD_H1;
   for (int i = 0; i < 10; i++) {
      datetime dt = iTime(symbol, timeframe, i);
      double open = iOpen(symbol, timeframe, i);
      double close = iClose(symbol, timeframe, i);
      double high = iHigh(symbol, timeframe, i);
      double low = iLow(symbol, timeframe, i);
      Print(dt, ",", open, ",", close, ",", high, ",", low);
      
      string sql = StringConcatenate("insert into usdjpy (time, open, high, low, close) values ('",
                     dt, "',", open, ",", high, ",", low, ",", close, ")");
      if (!OdbcExecute(sql)) {
         Print("Insert failed.");
         Print(OdbcErrorCode(), OdbcErrorMsg());
      }
   }
   
   ulong sth = OdbcQuery("select count(*), max(open), min(open) from usdjpy");
   if (sth == 0) {
      Print("Query failed.");
      Print(OdbcErrorCode(), OdbcErrorMsg());
      return;     
   }
   OdbcBindColInt(sth); // count
   OdbcBindColDouble(sth); // count
   OdbcBindColDouble(sth); // count
   
   while (OdbcFetch(sth)) {
      if (OdbcIsNull(sth, 1)) { // No meaning, just a sample.
         continue;
      }
      
      int cnt = OdbcGetColInt(sth, 1);
      double max = OdbcGetColDouble(sth, 2);
      double min = OdbcGetColDouble(sth, 3);
      
      Print("Count=", cnt, " max=", max, " min=", min);
   }
   OdbcCloseStmt(sth);
   
}
//+------------------------------------------------------------------+
