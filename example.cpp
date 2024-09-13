#include <iostream>
#include <sqlite3.h>
#include <string>
using namespace std; 

 int executeSQL(sqlite3 *db, const char *sql){
    char *errMsg = nullptr; 
    int rc = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);
    if(rc != SQLITE_OK){
        cerr << "Error al ejecutar SQL: " << errMsg << endl;
        sqlite3_free(errMsg);
    }
    return rc;
 }
 void insertarUsuario(sqlite3 *db){
    string nombre, correo;
    cout <<" Introduce el nombre del usuario: ";
    cin >> nombre; 
    cout << "Introduce el correo electronico: "; 
    cin >> correo; 

    string sql = "INSERT INTO usuarios (nombre, correoElectronico) VALUES ('" + nombre + "', '" + correo + "');";
    int rc = executeSQL(db, sql.c_str());

    if(rc==SQLITE_OK){
        cout << "Usuario insertado con exito. "<< endl; 
    }
 }
void insertarJuego(sqlite3 *db) {
    string nombre;
    int precio;
    cout << "Introduce el nombre del juego: ";
    cin >> nombre;
    cout << "Introduce el precio del juego: ";
    cin >> precio;

    string sql = "INSERT INTO juegos (nombre, precio) VALUES ('" + nombre + "', " + to_string(precio) + ");";
    int rc = executeSQL(db, sql.c_str());

    if (rc == SQLITE_OK) {
        cout << "Juego insertado con éxito." << endl;
    }
}
void mostrarUsuarios(sqlite3 *db) {
    const char *sql = "SELECT * FROM usuarios;";
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    cout << "ID | Nombre | Correo Electrónico" << endl;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const unsigned char *nombre = sqlite3_column_text(stmt, 1);
        const unsigned char *correo = sqlite3_column_text(stmt, 2);

        cout << id << " | " << nombre << " | " << correo << endl;
    }
    sqlite3_finalize(stmt);
}

void mostrarJuegos(sqlite3 *db) {
    const char *sql = "SELECT * FROM juegos;";
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    cout << "ID | Nombre | Precio" << endl;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const unsigned char *nombre = sqlite3_column_text(stmt, 1);
        int precio = sqlite3_column_int(stmt, 2);

        cout << id << " | " << nombre << " | " << precio << endl;
    }
    sqlite3_finalize(stmt);
}

int main() {
    sqlite3 *db;
    int rc;

    rc = sqlite3_open("tablas.db", &db);  // Abre una base de datos en archivo

    if (rc) {
        cerr << "No se pudo abrir la base de datos: " << sqlite3_errmsg(db) <<endl;
        return rc;
    } else {
        cout << "Base de datos abierta con éxito." << endl;
    }

    const char *createTableSQL1 = R"(
        CREATE TABLE IF NOT EXISTS usuarios(
         id INTEGER PRIMARY KEY AUTOINCREMENT,
         nombre VARCHAR(50),
         correoElectronico VARCHAR(250) UNIQUE
         );
         )";

    const char *createTableSQL2  = R"(
        CREATE TABLE IF NOT EXISTS juegos(
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        nombre VARCHAR(250) UNIQUE,
        precio INTEGER NOT NULL);
        )";

    executeSQL(db, createTableSQL1);
    executeSQL(db, createTableSQL2);

   

        //menu

     int opcion;
    do {
        cout << "\n---- Menú ----\n";
        cout << "1. Insertar Usuario\n";
        cout << "2. Insertar Juego\n";
        cout << "3. Ver Usuarios\n";
        cout << "4. Ver Juegos\n";
        cout << "5. Salir\n";
        cout << "Elige una opción: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                insertarUsuario(db);
                break;
            case 2:
                insertarJuego(db);
                break;
            case 3:
                mostrarUsuarios(db);
                break;
            case 4:
                mostrarJuegos(db);
                break;
            case 5:
                cout << "Saliendo..." << endl;
                break;
            default:
                cout << "Opción no válida." << endl;
        }
    } while (opcion != 5);
    sqlite3_close(db);
    return 0;
}
