#include "stdafx.h"
#include "Statement.h"
#include "odbc.h"

#define NUM_MAX_COLS 20

Statement::Statement() : hStmt(NULL), nCols(0), nMaxCols(NUM_MAX_COLS), pColumns(NULL) {
}

Statement::~Statement(){
	Close();
	if (pColumns != NULL) {
		delete[]pColumns;
	}
}


BOOL Statement::Init(SQLHDBC hDBC) {
	SQLRETURN rRet;
	rRet = SQLAllocHandle(SQL_HANDLE_STMT, hDBC, &hStmt);
	if (!SQLIsSuccess(rRet)) {
		DBDiag(SQL_HANDLE_DBC, hDBC);
		return FALSE;
	}
	pColumns = new Column[NUM_MAX_COLS];
	return TRUE;
}

BOOL Statement::Prepare(wchar_t *sql) {
	SQLRETURN rRet;

	rRet = SQLPrepare(hStmt, (SQLWCHAR*)sql, SQL_NTS);
	if (!SQLIsSuccess(rRet)) {
		DBDiag(SQL_HANDLE_STMT, hStmt);
		return FALSE;
	}
	return TRUE;
}

BOOL Statement::Execute() {
	SQLRETURN rRet;

	rRet = SQLExecute(hStmt);
	if (!SQLIsSuccess(rRet)) {
		DBDiag(SQL_HANDLE_STMT, hStmt);
		return FALSE;
	}
	return TRUE;
}

void Statement::Close() {
	if (hStmt == NULL) return;

	SQLFreeStmt(hStmt, SQL_CLOSE);
}

 Statement* Statement::Query(SQLHDBC hDBC, wchar_t *sql) {
	Statement *pStmt = new Statement();
	if (!pStmt->Init(hDBC)) return NULL;

	if (!pStmt->Prepare(sql)) {
		delete pStmt;
		return NULL;
	}

	if (!pStmt->Execute()) {
		delete pStmt;
		return NULL;
	}
	return pStmt;
}

 BOOL Statement::Execute(SQLHDBC hDBC, wchar_t *sql) {
	 Statement *pStmt = Query(hDBC, sql);
	 if (pStmt == NULL) return FALSE;
	 delete pStmt;
	 return TRUE;
 }

 BOOL Statement::BindCol(int nColNo, int nType, void *pPtr, int nSize, long *pSize) {
	 SQLRETURN rRet;

	 rRet = SQLBindCol(hStmt, nColNo, nType, pPtr, nSize, pSize);
	 if (!SQLIsSuccess(rRet)) {
		 DBDiag(SQL_HANDLE_STMT, hStmt);
		 return FALSE;
	 }
	 return TRUE;

}

BOOL Statement::Fetch() {

	 for (int i = 0; i < nCols; i++) {
		 pColumns[i].nNullCheck = pColumns[i].nSize;
	 }

	 SQLRETURN rRet;
	 rRet = SQLFetch(hStmt);
	 if (rRet == SQL_NO_DATA_FOUND) {
		 return FALSE;
	 }
	 if (!SQLIsSuccess(rRet)) {
		 DBDiag(SQL_HANDLE_STMT, hStmt);
		 return FALSE;
	 }
	 return TRUE;
 }

BOOL Statement::BindInt() {
	return Bind(SQL_C_SLONG, sizeof(int));
}

BOOL Statement::BindLong() {
	return Bind(SQL_C_SBIGINT, sizeof(LONG64));
}

BOOL Statement::BindDouble() {
	return Bind(SQL_C_DOUBLE, sizeof(double));
}

BOOL Statement::BindString(int nMax) {
	return Bind(SQL_C_WCHAR, nMax);
}


BOOL Statement::Bind(SQLSMALLINT nType, int size) {
	if (nCols >= nMaxCols) return FALSE;

	Column *pCol = pColumns + nCols;

	pCol->nType = nType;
	pCol->nSize = size;
	pCol->pBuf = new BYTE[size];

	SQLRETURN rRet;

	rRet = SQLBindCol(hStmt, nCols + 1, pCol->nType, pCol->pBuf, pCol->nSize, &(pCol->nNullCheck));
	if (!SQLIsSuccess(rRet)) {
		DBDiag(SQL_HANDLE_STMT, hStmt);
		delete[] pCol->pBuf;
		return FALSE;
	}

	nCols++;
	return TRUE;
}


Statement::Column::Column() : pBuf(NULL) {
}

Statement::Column::~Column() {
	if (pBuf != NULL) delete[] pBuf;
}

BOOL Statement::Column::Init() {
	return TRUE;
}

int Statement::GetColInt(int iCol) {
	Column *p = pColumns + (iCol - 1);
	if (p->nNullCheck == SQL_NULL_DATA) return 0;
	return *(int*)(p->pBuf);
}

LONG64 Statement::GetColLong(int iCol) {
	Column *p = pColumns + (iCol - 1);
	if (p->nNullCheck == SQL_NULL_DATA) return 0L;
	return *(LONG64*)(p->pBuf);
}

double Statement::GetColDouble(int iCol) {
	Column *p = pColumns + (iCol - 1);
	if (p->nNullCheck == SQL_NULL_DATA) return 0.0;
	return *(double*)(p->pBuf);
}

wchar_t *Statement::GetColString(int iCol) {
	Column *p = pColumns + (iCol - 1);
	if (p->nNullCheck == SQL_NULL_DATA) {
		*(wchar_t*)(p->pBuf) = L'\0';
	}
	return (wchar_t*)(p->pBuf);
}

BOOL Statement::IsNull(int iCol) {
	return pColumns[iCol - 1].nNullCheck == SQL_NULL_DATA;
}
