#pragma once
class Statement{

	SQLHSTMT hStmt;

	class Column {
	public:
		SQLSMALLINT nType;
		SQLLEN nSize;

		BYTE *pBuf;
		SQLLEN nNullCheck;

		Column();
		~Column();
		BOOL Init();

	};

	int nCols;
	int nMaxCols;
	Column *pColumns;

protected:
	BOOL BindCol(int nColNo, int nType, void *pPtr, int nSize, long *pSize);
	BOOL Bind(SQLSMALLINT nType, int nDataSize);

public:
	Statement();
	~Statement();


	BOOL Init(SQLHDBC hDBC);
	BOOL Prepare(wchar_t *sql);
	BOOL Execute();
	void Close();

	BOOL Fetch();

	BOOL BindInt();
	BOOL BindLong();
	BOOL BindDouble();
	BOOL BindString(int nMax);

	int GetColInt(int iCol);
	LONG64 GetColLong(int iCol);
	double GetColDouble(int iCol);
	wchar_t *GetColString(int iCol);

	BOOL IsNull(int iCol);

	static Statement* Query(SQLHDBC hDBC, wchar_t *sql);
	static BOOL Execute(SQLHDBC hDBC, wchar_t *sql);

};

