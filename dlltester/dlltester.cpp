// dlltester.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "mt4odbc.h"
#include <locale.h>
int _tmain()
{
	setlocale(LC_ALL, "ja-JP");

	printf("Start..\n");

	if (!OdbcInitEnv()) {
		printf("InitEnv failed.\n");
		return 1;
	}

	if (!OdbcConnect(L"fxc", L"", L"")) {
		printf("Connect failed. code=%ls msg=%ls\n", OdbcErrorCode(), OdbcErrorMsg());
		return 1;
	}

	if (!OdbcExecute(L"insert into test(kn, kv) values ('abc', '123')")) {
		printf("Exec failed. code=%ls msg=%ls\n", OdbcErrorCode(),OdbcErrorMsg());
		return 1;
	}

	ULONG64 lSth = OdbcQuery(L"select kn, kv from test");
	if (lSth == 0) {
		printf("Select failed. code=%ls msg=%ls\n", OdbcErrorCode(), OdbcErrorMsg());
		return 1;
	}
	OdbcBindColString(lSth, 128);
	OdbcBindColLong(lSth);

	while (OdbcFetch(lSth)) {
		wchar_t *str = OdbcGetColString(lSth, 1);
		if (OdbcIsNull(lSth, 1)) {
			printf("kn= NULL(%ls) kv = %I64d\n", str, OdbcGetColLong(lSth, 2));
		} else {
			wprintf(L"kn=%ls kv =%I64d\n", str, OdbcGetColLong(lSth, 2));
		}
	}
	OdbcCloseStmt(lSth);

	if (!OdbcDisconnect()) {
		printf("Disconnect failed.\n");
		return 1;
	}

	if (!OdbcDeInitEnv()) {
		printf("DeInitEnv failed.\n");
		return 1;
	}

	printf("Success!\n");
	return 0;
}

