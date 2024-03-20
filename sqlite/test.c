#include <stdio.h>
#include <sqlite3.h>

int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    for (int i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

int main() {
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;

    // 打开数据库或创建一个新的数据库
    rc = sqlite3_open("example.db", &db);
    if (rc) {
        fprintf(stderr, "无法打开数据库: %s\n", sqlite3_errmsg(db));
        return rc;
    } else {
        fprintf(stdout, "成功打开数据库\n");
    }

    // 创建表
    char *sql = "CREATE TABLE COMPANY(ID INT PRIMARY KEY NOT NULL, NAME TEXT NOT NULL, AGE INT NOT NULL, ADDRESS CHAR(50), SALARY REAL);";
    rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL 错误: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "表创建成功\n");
    }

    // 插入数据
    sql = "INSERT INTO COMPANY (ID, NAME, AGE, ADDRESS, SALARY) VALUES (1, 'Alice', 25, 'New York', 2000.0);"
          "INSERT INTO COMPANY (ID, NAME, AGE, ADDRESS, SALARY) VALUES (2, 'Bob', 30, 'London', 3000.0);"
          "INSERT INTO COMPANY (ID, NAME, AGE, ADDRESS, SALARY) VALUES (3, 'Charlie', 35, 'Paris', 4000.0);";
    rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL 错误: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "数据插入成功\n");
    }

    // 查询数据
    sql = "SELECT * FROM COMPANY;";
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL 错误: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }

    // 关闭数据库连接
    sqlite3_close(db);

    return 0;
}
