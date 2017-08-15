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
See \MQL4\oco.mq4

# Restrictions

* Working 2 or more EAs are not tested.
* date/time is not supported. Use string or int(unix time or something).


