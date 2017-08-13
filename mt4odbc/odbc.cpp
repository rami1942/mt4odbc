#include "stdafx.h"
#include "odbc.h"
#include <stdio.h>

BOOL SQLIsSuccess(SQLRETURN ret) { return ((ret == SQL_SUCCESS) || (ret == SQL_SUCCESS_WITH_INFO)); }

BOOL DBEnvInit(SQLHENV *phEnv) {
	SQLRETURN rRet;
	rRet = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, phEnv);
	if (!SQLIsSuccess(rRet)) {
		return FALSE;
	}
	rRet = SQLSetEnvAttr(*phEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);
	if (!SQLIsSuccess(rRet)) {
		SQLFreeHandle(SQL_HANDLE_ENV, (SQLHANDLE)*phEnv);
		*phEnv = NULL;
		return FALSE;
	}
	return TRUE;
}

BOOL DBEnvDeInit(SQLHENV *phEnv) {
	if (*phEnv == NULL) return TRUE;
	SQLFreeHandle(SQL_HANDLE_ENV, *phEnv);
	*phEnv = NULL;
	return TRUE;
}

BOOL DBConnect(SQLHENV hEnv, SQLHDBC *phDBC, wchar_t *pDsn, wchar_t *pUserName, wchar_t *pPassword) {
	SQLRETURN rRet;
	rRet = SQLAllocHandle(SQL_HANDLE_DBC, hEnv, phDBC);
	if (!SQLIsSuccess(rRet)) {
		return FALSE;
	}
	rRet = SQLConnect(*phDBC, (SQLWCHAR*)pDsn, SQL_NTS, (SQLWCHAR*)pUserName, SQL_NTS, (SQLWCHAR*)pPassword, SQL_NTS);
	if (!SQLIsSuccess(rRet)) {
		DBDiag(SQL_HANDLE_DBC, *phDBC);
		SQLFreeHandle(SQL_HANDLE_DBC, *phDBC);
		return FALSE;
	}

	return TRUE;
}

BOOL DBDisconnect(SQLHENV hEnv, SQLHDBC *phDBC) {
	if (*phDBC == NULL) return TRUE;

	SQLRETURN rRet;
	rRet = SQLDisconnect(*phDBC);
	if (!SQLIsSuccess(rRet)) {
		return FALSE;
	}

	SQLFreeHandle(SQL_HANDLE_DBC, *phDBC);
	*phDBC = NULL;
	return TRUE;
}


#define DIAG_BUFSIZE 8192
static wchar_t szDiagBuf[DIAG_BUFSIZE];
static SQLWCHAR szState[32];

wchar_t *DBErrMsg() {
	return szDiagBuf;
}

wchar_t *DBErrCode() {
	return szState;
}

wchar_t *DBDiag(SQLSMALLINT iHandleType, SQLHANDLE hHandle) {
	SQLRETURN rRet;

	SQLINTEGER nNativeErrorPtr;
	SQLSMALLINT nLen;

	rRet = SQLGetDiagRec(iHandleType, hHandle, 1, szState, 
				&nNativeErrorPtr, szDiagBuf, DIAG_BUFSIZE, &nLen);
	if (SQLIsSuccess(rRet)) {
		return szDiagBuf;
	}
	return NULL;
}
