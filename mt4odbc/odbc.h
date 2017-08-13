#pragma once

BOOL DBEnvInit(SQLHENV *phEnv);
BOOL DBEnvDeInit(SQLHENV *phEnv);
BOOL DBConnect(SQLHENV hEnv, SQLHDBC *phDBC, wchar_t *pDsn, wchar_t *pUserName, wchar_t *pPassword);
BOOL DBDisconnect(SQLHENV hEnv, SQLHDBC *phDBC);


wchar_t *DBDiag(SQLSMALLINT iHandleType, SQLHANDLE hHandle);
wchar_t *DBErrMsg();
wchar_t *DBErrCode();

BOOL SQLIsSuccess(SQLRETURN ret);
