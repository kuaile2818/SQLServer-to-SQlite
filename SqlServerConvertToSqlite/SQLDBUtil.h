#pragma once
typedef int QueryCallback(void *para, int n_column, char **column_value, char **column_name);

typedef enum _SQLITE_DATATYPE
{
	SQLITE_DATATYPE_INTEGER = SQLITE_INTEGER,
	SQLITE_DATATYPE_FLOAT = SQLITE_FLOAT,
	SQLITE_DATATYPE_TEXT = SQLITE_TEXT,
	SQLITE_DATATYPE_BLOB = SQLITE_BLOB,
	SQLITE_DATATYPE_NULL = SQLITE_NULL,
}SQLITE_DATATYPE;

typedef enum _SQLITE_DB
{
	SQLITE_LOCAL,
	SQLITE_INFO
}SQLITE_DB;

class SQLDBUtil;

class SQLiteDataReader
{
public:
	SQLiteDataReader(sqlite3_stmt *pStmt);
	~SQLiteDataReader();
public:
	// ��ȡһ������   
	BOOL Read();
	// �ر�Reader����ȡ���������   
	void Close();
	// �ܵ�����   
	int ColumnCount(void);
	// ��ȡĳ�е�����    
	LPCTSTR GetName(int nCol);
	// ��ȡĳ�е���������   
	SQLITE_DATATYPE GetDataType(int nCol);
	// ��ȡĳ�е�ֵ(�ַ���)   
#ifdef  UNICODE 
	wchar_t* GetStringValue(int nCol);
#else
	char* GetStringValue(int nCol);
#endif

	// ��ȡĳ�е�ֵ(����)   
	int GetIntValue(int nCol);
	// ��ȡĳ�е�ֵ(������)   
	long GetInt64Value(int nCol);
	// ��ȡĳ�е�ֵ(������)   
	double GetFloatValue(int nCol);
	// ��ȡĳ�е�ֵ(����������)   
	const BYTE* GetBlobValue(int nCol, int &nLen);
private:
	sqlite3_stmt *m_pStmt;
};

class SQLiteCommand
{
public:
	SQLiteCommand(SQLDBUtil* pSqlite);
	SQLiteCommand(SQLDBUtil* pSqlite, LPCTSTR lpSql);
	~SQLiteCommand();
public:
	// ��������   
	BOOL SetCommandText(LPCTSTR lpSql);
	// �󶨲�����indexΪҪ�󶨲�������ţ���1��ʼ��  
	BOOL BindParam(int index, LPCTSTR szValue);
	BOOL BindParam(int index, const int nValue);
	BOOL BindParam(int index, const double dValue);
	BOOL BindParam(int index, const unsigned char* blobValue, int nLen);
	// ִ������   
	BOOL Excute();
	// �����������ʹ��ʱ����øýӿ������   
	void Clear();
private:
	SQLDBUtil *m_pSqlite;
	sqlite3_stmt *m_pStmt;
};

class SQLDBUtil
{
public:
	SQLDBUtil(LPCTSTR sqliteName, LPCTSTR sqlitePWD);
	~SQLDBUtil(void);
public:
	BOOL Init(SQLITE_DB type);
	// �����ݿ�   
	BOOL Open(int sqliteType = SQLITE_OPEN_READONLY);
	// �ر����ݿ�   
	void Close();
	// ִ�зǲ�ѯ���������»�ɾ����   
	BOOL ExcuteNonQuery(LPCTSTR lpSql);
	BOOL ExcuteNonQuery(SQLiteCommand* pCmd);
	// ��ѯ   
	SQLiteDataReader* ExcuteQuery(LPCTSTR lpSql);
	// ��ѯ���ص���ʽ��   
	BOOL ExcuteQuery(LPCTSTR lpSql, QueryCallback pCallBack);

	// ��ʼ����   
	BOOL BeginTransaction();
	// �ύ����   
	BOOL CommitTransaction();
	// �ع�����   
	BOOL RollbackTransaction();

	// ��ȡ��һ��������Ϣ   
	LPCTSTR GetLastErrorMsg();
public:
	friend class SQLiteCommand;
private:
	LPCTSTR s_name;
	LPCTSTR s_pwd;
	Operation oper;
	sqlite3 *w_db;
	TCHAR exeFullPath[MAX_PATH];
};