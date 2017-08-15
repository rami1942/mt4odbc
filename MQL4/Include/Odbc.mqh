//+------------------------------------------------------------------+
//|                                                         Odbc.mqh |
//|                                                         rami1942 |
//|                                     https://github.com/rami1942/ |
//+------------------------------------------------------------------+
#property copyright "rami1942"
#property link      "https://github.com/rami1942/"
#property strict


#import "mt4odbc.dll"
   int OdbcInitEnv();
   int OdbcDeInitEnv();
   int OdbcConnect(string dsName, string userName, string password);
   int OdbcDisconnect();

   int OdbcExecute(string sql);
   ulong OdbcQuery(string sql);

   void OdbcCloseStmt(ulong sth);
   string OdbcErrorMsg();
   string OdbcErrorCode();

   int OdbcBindColLong(ulong ihStmt);
   int OdbcBindColInt(ulong ihStmt);
   int OdbcBindColDouble(ulong ihStmt);
   int OdbcBindColString(ulong ihStmt, int nSize);

   int OdbcGetColInt(ulong ihStmt, int iCol);
   long OdbcGetColLong(ulong ihStmt, int iCol);
   double OdbcGetColDouble(ulong ihStmt, int iCol);
   string OdbcGetColString(ulong ihStmt, int iCol);
   int OdbcIsNull(ulong ihStmt, int iCol);

   int OdbcFetch(ulong ihStmt);
#import
