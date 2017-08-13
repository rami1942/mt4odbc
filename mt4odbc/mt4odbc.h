#ifdef MT4ODBC_EXPORTS
#define MT4ODBC_API __declspec(dllexport)
#else
#define MT4ODBC_API __declspec(dllimport)
#endif

MT4ODBC_API int __stdcall OdbcInitEnv();
MT4ODBC_API int __stdcall OdbcDeInitEnv();
MT4ODBC_API int __stdcall OdbcConnect(wchar_t *pDsn, wchar_t *pUserName, wchar_t *pPassword);
MT4ODBC_API int __stdcall OdbcDisconnect();

MT4ODBC_API int __stdcall OdbcExecute(wchar_t *pSql);

MT4ODBC_API ULONG64 __stdcall OdbcQuery(wchar_t *pSql);
MT4ODBC_API void __stdcall OdbcCloseStmt(ULONG64 sth);

MT4ODBC_API wchar_t* __stdcall OdbcErrorMsg();
MT4ODBC_API wchar_t* __stdcall OdbcErrorCode();

MT4ODBC_API int __stdcall OdbcBindColInt(ULONG64 ihStmt);
MT4ODBC_API int __stdcall OdbcBindColLong(ULONG64 ihStmt);
MT4ODBC_API int __stdcall OdbcBindColDouble(ULONG64 ihStmt);
MT4ODBC_API int __stdcall OdbcBindColString(ULONG64 ihStmt, int nMax);

MT4ODBC_API int __stdcall OdbcGetColInt(ULONG64 ihStmt, int iCol);
MT4ODBC_API LONG64 __stdcall OdbcGetColLong(ULONG64 ihStmt, int iCol);
MT4ODBC_API double __stdcall OdbcGetColDouble(ULONG64 ihStmt, int iCol);
MT4ODBC_API wchar_t* __stdcall OdbcGetColString(ULONG64 ihStmt, int iCol);

MT4ODBC_API int __stdcall OdbcIsNull(ULONG64 ihStmt, int iCol);


MT4ODBC_API int __stdcall OdbcFetch(ULONG64 ihStmt);

MT4ODBC_API wchar_t* __stdcall OdbcTest(wchar_t *p);