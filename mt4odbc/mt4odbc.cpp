#include "stdafx.h"
#include "mt4odbc.h"
#include "odbc.h"
#include "Statement.h"

static SQLHENV hEnv = NULL;
static SQLHDBC hDBC = NULL;


MT4ODBC_API int __stdcall OdbcInitEnv() {
	if (DBEnvInit(&hEnv)) {
		return 1;
	}
	return 0;
}

MT4ODBC_API int __stdcall OdbcDeInitEnv() {
	if (DBEnvDeInit(&hEnv)) {
		return 1;
	}
	return 0;
}

MT4ODBC_API wchar_t* __stdcall OdbcErrorMsg() {
	return DBErrMsg();
}
MT4ODBC_API wchar_t* __stdcall OdbcErrorCode() {
	return DBErrCode();
}


MT4ODBC_API int __stdcall OdbcConnect(wchar_t *pDsn, wchar_t *pUserName, wchar_t *pPassword) {
	if (hDBC != NULL) {
		DBDisconnect(hEnv, &hDBC);
	}

	if (DBConnect(hEnv, &hDBC, pDsn, pUserName, pPassword)) {
		return 1;
	} else {
		return 0;
	}
}

MT4ODBC_API int __stdcall OdbcDisconnect() {
	DBDisconnect(hEnv, &hDBC);
	return 1;
}

MT4ODBC_API ULONG64 __stdcall OdbcQuery(wchar_t *pSql) {
	Statement *pStmt = Statement::Query(hDBC, pSql);
	if (pStmt == NULL) {
		return 0;
	}
	return (ULONG64)pStmt;
}

MT4ODBC_API int __stdcall OdbcExecute(wchar_t *pSql) {
	return Statement::Execute(hDBC, pSql);
}

MT4ODBC_API void __stdcall OdbcCloseStmt(ULONG64 sth) {
	Statement *pStmt = (Statement*)sth;
	delete pStmt;
}

MT4ODBC_API int __stdcall OdbcFetch(ULONG64 ihStmt) {
	Statement *pStmt = (Statement*)ihStmt;
	return pStmt->Fetch();
}

MT4ODBC_API int __stdcall OdbcBindColInt(ULONG64 ihStmt) {
	Statement *pStmt = (Statement*)ihStmt;
	return pStmt->BindInt();
}

MT4ODBC_API int __stdcall OdbcBindColLong(ULONG64 ihStmt) {
	Statement *pStmt = (Statement*)ihStmt;
	return pStmt->BindLong();
}

MT4ODBC_API int __stdcall OdbcBindColDouble(ULONG64 ihStmt) {
	Statement *pStmt = (Statement*)ihStmt;
	return pStmt->BindDouble();
}

MT4ODBC_API int __stdcall OdbcBindColString(ULONG64 ihStmt, int nMax) {
	Statement *pStmt = (Statement*)ihStmt;
	return pStmt->BindString(nMax);
}

MT4ODBC_API wchar_t* __stdcall OdbcTest(wchar_t *p) {

	int *pN = (int*)((char*)p - sizeof(int));

	static wchar_t buf[4096];
	wsprintf(buf, L"HELLO %d", *pN);
	lstrcpy(p, L"TEST");
	return buf;
}

MT4ODBC_API int __stdcall OdbcGetColInt(ULONG64 ihStmt, int iCol) {
	Statement *pStmt = (Statement*)ihStmt;
	return pStmt->GetColInt(iCol);
}

MT4ODBC_API LONG64 __stdcall OdbcGetColLong(ULONG64 ihStmt, int iCol) {
	Statement *pStmt = (Statement*)ihStmt;
	return pStmt->GetColLong(iCol);
}

MT4ODBC_API double __stdcall OdbcGetColDouble(ULONG64 ihStmt, int iCol) {
	Statement *pStmt = (Statement*)ihStmt;
	return pStmt->GetColDouble(iCol);
}

MT4ODBC_API wchar_t* __stdcall OdbcGetColString(ULONG64 ihStmt, int iCol) {
	Statement *pStmt = (Statement*)ihStmt;
	return pStmt->GetColString(iCol);
}

MT4ODBC_API int __stdcall OdbcIsNull(ULONG64 ihStmt, int iCol) {
	Statement *pStmt = (Statement*)ihStmt;
	return pStmt->IsNull(iCol);
}
