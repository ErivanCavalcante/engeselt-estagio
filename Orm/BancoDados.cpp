#include "BancoDados.h"

BancoDados::BancoDados()
{

}

bool BancoDados::inicarConexao()
{
    //Testa se o mysql esta disponivel
    if(!QSqlDatabase::isDriverAvailable("QSQLITE"))
    {
        return false;
    }

    //Necessaria conexoes
    db = QSqlDatabase::addDatabase("QSQLITE");

    //Seta as credenciais
    db.setDatabaseName("catalogo.sqlite");

    if(!db.open())
    {
        return false;
    }

    return true;
}

void BancoDados::removerConexao()
{
    QSqlDatabase::removeDatabase("DEFAULT");
}

