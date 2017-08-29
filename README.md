# mt4odbc
Yet another MT4-ODBC bridge, and some scripts.

Most of DBMS provides its ODBC driver, you can access any DBMS from your MQL programs. You can write more complex EAs, useful scripts, etc.

Notice these codes are writtern for my personal use and not intended to develop fully products. 
I will maintain and fix bugs if I found or I need, but may not be fixed if I think I don't need. 
Yes you can use these codes by your own risk and rewrite for yourself.

# WIPWIPWIP

# Environment

* Windows Server 2016
* Windows Server 2012 R2
* Visual Studio 2017 (if you compile dll/exe)
* Metatrader 4 at FOREX.com version 4.0.0 build 1090 (http://jp.forex.com/jp/)
* SQLite3 ODBC Driver 32bit, 0.9995.00.00 (http://www.ch-werner.de/sqliteodbc/)

You need Microsoft Visual C++ 2017 Redistributable x86 (https://go.microsoft.com/fwlink/?LinkId=746571)  
MT4 is 32bit application, so you need 32bit ODBC/redistributable. NOT x64 even though OS is 64bit.

# Install

* Copy .\MQL4 to your MT4 data folder. You can open MT4 data folder by File -> Open Datafolder menu.
* Install ODBC driver
* Set up ODBC datasource 
* Include odbc.mqh from your MQL scripts.

# Sample
See \MQL4\Scripts\export.mq4

## Use
 #include <Odbc.mqh> 

## Init, Connect/ DeInit

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
		OdbcDisconnect();
		OdbcDeInitEnv(); 
	}

## Insert/ Update / Delete

      string sql = StringConcatenate("insert into usdjpy (time, open, high, low, close) values ('", 
                     dt, "',", open, ",", high, ",", low, ",", close, ")"); // Prepare/placefolder is not supported. 
      if (!OdbcExecute(sql)) { // result is true/false. if you want last insert ID, use select last_insert_rowid() / last_insert_id() (Depends on DBMS) 
         Print("Insert failed."); 
         Print(OdbcErrorCode(), OdbcErrorMsg()); // You can get error reason 
	  } 

## Query

	ulong sth = OdbcQuery("select * from foo"); // You can open more than one cursor 
	if (sth == 0) { 
		ERROR 
	} 
    OdbcBindColInt(sth); // First column as int 
    OdbcBindColDouble(sth); // Second column as double 
    OdbcBindColLong(sth); // 3rd column as long 
    OdbcBindColString(sth); // 4th column as string 

	while(OdbcFetch(sth)) { 
		if (OdbcIsNull(sth, 4)) { // Null check. Column no is base 1. Not base 0
			string str = OdbcGetColString(sth, 4); // if the column is null, return "" or 0 
		} 
	} 
	OdbCloseStmt(sth); // Free buffer. Don't forget without you want memory leak. 

# Restrictions

* Working 2 or more EAs are not tested.
* date/time is not supported. Use string or int(unix time or something).


