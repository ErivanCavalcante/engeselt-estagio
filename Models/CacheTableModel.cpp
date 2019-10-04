#include "CacheTableModel.h"

#include <QDebug>

CacheTableModel::CacheTableModel(QObject *parent): QAbstractTableModel(parent)
{
    //Conexao com o banco de dados
    db = QSqlDatabase::database();

    dirty = false;
}

CacheTableModel::~CacheTableModel()
{
    for(auto r : dados)
        delete r;

    for(auto p : cacheRemove)
        delete p.second;

    dados.clear();
    cacheRemove.clear();
}

int CacheTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return dados.size();
}

int CacheTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return colunas.count();
}

QVariant CacheTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
        return QString("Coluna %1").arg(section);
    else
        return QString("Linha %1").arg(section);
}

QVariant CacheTableModel::data(const QModelIndex &index, int role) const
{
    int linha = index.row();
    int coluna = index.column();

    if(!index.isValid() || linha > dados.size() || coluna > colunas.count())
        return QVariant();

    if(role == Qt::DisplayRole)
    {
        //Pega o valor da lista e retorna
        RowChanged *r = dados[linha];

        return r->pegarRecord().value(coluna);
    }
    else
        return QVariant();
}

Qt::ItemFlags CacheTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
            return Qt::ItemIsEnabled;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool CacheTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole)
    {
        //Pega o valor da lista e retorna
        RowChanged *r = dados[index.row()];

        //So muda o status se ele nao for para adicionar
        if(r->pegarOperacao() != TipoOp::ADICIONAR)
            r->setarOperacao(TipoOp::ATUALIZAR);

        r->pegarRecord().setValue(index.column(), value);

        dirty = true;

        emit dataChanged(index, index, {role});
        return true;
    }
    return false;
}

bool CacheTableModel::select(QString sql)
{
    //Limpa o cache
    if(cacheRemove.size() > 0)
    {
        for(auto p : cacheRemove)
            delete p.second;

        cacheRemove.clear();
    }

    //Remove os dados para colocar novos
    if(dados.size() > 0)
    {
        beginRemoveRows(QModelIndex(), 0, dados.size() - 1);

        //Deleta os ponteiros
        for(auto *r : dados)
            delete r;

        dados.clear();

        endRemoveRows();
    }

    //Cria a query
    QSqlQuery q(db);

    //Executa
    if(!q.exec(sql))
    {
        lastError = q.lastError();
        return false;
    }

    //Pegas as colunas
    colunas = q.record();

    //Conta o numero de linhas
    int i = 0;
    while (q.next())
        i++;

    if(i > 0)
    {
        beginInsertRows(QModelIndex(), 0, i - 1);

        //Loop no resultado
        q.first();
        do
        {
            //Cria com a operacao nenhum
            RowChanged *r = new RowChanged(q.record());

            dados.push_back(r);
        }
        while (q.next());

        endInsertRows();

        dirty = false;
    }

    return true;
}

bool CacheTableModel::select(QSqlQuery &q)
{
    //Limpa o cache
    if(cacheRemove.size() > 0)
    {
        for(auto p : cacheRemove)
            delete p.second;

        cacheRemove.clear();
    }

    //Remove os dados para colocar novos
    if(dados.size() > 0)
    {
        beginRemoveRows(QModelIndex(), 0, dados.size() - 1);

        //Deleta os ponteiros
        for(auto *r : dados)
            delete r;

        dados.clear();

        endRemoveRows();
    }

    //Executa
    if(!q.exec())
    {
        lastError = q.lastError();
        return false;
    }

    //Pegas as colunas
    colunas = q.record();

    //Conta o numero de linhas
    int i = 0;
    while (q.next())
        i++;

    if(i > 0)
    {
        beginInsertRows(QModelIndex(), 0, i - 1);

        //Loop no resultado
        q.first();
        do
        {
            //Cria com a operacao nenhum
            RowChanged *r = new RowChanged(q.record());

            dados.push_back(r);
        }
        while (q.next());

        endInsertRows();

        dirty = false;
    }

    return true;
}

QSqlRecord CacheTableModel::record(int i) const
{
    if(i > dados.count() || i < 0)
        return QSqlRecord();

    return dados[i]->pegarRecord();
}

QSqlRecord CacheTableModel::record() const
{
    return colunas;
}

void CacheTableModel::setRecord(int row, QSqlRecord &r)
{
    if(row < 0 || row > dados.size())
        return;

    RowChanged *rc = dados[row];

    //So muda o status se ele nao for para adicionar
    if(rc->pegarOperacao() != TipoOp::ADICIONAR)
        rc->setarOperacao(TipoOp::ATUALIZAR);

    //Seta o dado
    rc->setarRecord(r);

    dirty = true;

    QModelIndex top = index(row, 0);
    QModelIndex bottom = index(row, colunas.count());

    emit dataChanged(top, bottom);
}

bool CacheTableModel::exists(QString coluna, QVariant valor)
{
    QSqlRecord rc;

    for(auto r : dados)
    {
        rc = r->pegarRecord();

        if(rc.contains(coluna))
        {
            if(rc.value(coluna) == valor)
                return true;
        }
    }

    return false;
}

bool CacheTableModel::insertRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent)

    beginInsertRows(QModelIndex(), row, row + count - 1);

    RowChanged *r = new RowChanged(QSqlRecord());

    r->setarOperacao(TipoOp::ADICIONAR);

    dados.insert(row, count, r);

    dirty = true;

    endInsertRows();

    return true;
}

bool CacheTableModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent)

    if(row + count > dados.size() || row < 0)
        return false;

    beginRemoveRows(QModelIndex(), row, row + count - 1);

    //Coloca no cache de deletados
    for(int i = row; i < row + count; ++i)
    {
        RowChanged *r = dados[i];

        //So coloca na lista se o tipo nao for de criar
        if(r->pegarOperacao() != TipoOp::ADICIONAR)
        {
            pair<int, RowChanged*> p;

            r->setarOperacao(TipoOp::REMOVER);

            //Salva a ultima linha
            p.first = i;
            p.second = r;

            cacheRemove.push_back(p);

            continue;
        }

        //Se chegou aqui eh pq eh do tipo criar
        //Entao deleta o objeto
        delete r;
    }

    //Remove da lista
    dados.remove(row, count);

    dirty = true;

    endRemoveRows();

    return true;
}

bool CacheTableModel::insertRecord(int row, QSqlRecord &r)
{
    beginInsertRows(QModelIndex(), row, row);

    RowChanged *rc = new RowChanged(r);

    rc->setarOperacao(TipoOp::ADICIONAR);

    dados.insert(row, 1, rc);

    dirty = true;

    endInsertRows();

    return true;
}

int CacheTableModel::indexField(QString coluna)
{
    return colunas.indexOf(coluna);
}

bool CacheTableModel::isDirty()
{
    return dirty;
}

bool CacheTableModel::save()
{
    if(!dirty)
        return true;

    //Query
    QSqlQuery q(db);
    bool ok = true;

    //Salva os dados modificados
    for(auto *row : dados)
    {
        if(row->pegarOperacao() != TipoOp::NENHUM)
        {
            int op = row->pegarOperacao();

            //Testa a operacao e salva no banco de dados
            switch(op)
            {
                case TipoOp::ADICIONAR:
                    ok = operacaoCreate(q, row->pegarRecord());
                    break;
                case TipoOp::ATUALIZAR:
                    ok = operacaoUpdate(q, row->pegarRecord());
                    break;
                default:
                    qDebug() << "Erro ao atualizar tabela cache";
                    break;
            }

            if(!ok)
                return false;

            //Reseta a operacao
            row->setarOperacao(TipoOp::NENHUM);
        }
    }

    //Deleta os dados
    for(auto d : cacheRemove)
    {
        //Salva no banco de dados
        ok = operacaoDelete(q, d.second->pegarRecord());

        if(!ok)
            return false;

        //Deleta o objeto pois nao vai mais ser usado
        delete d.second;
    }

    //Limpa o cache para novo uso
    cacheRemove.clear();

    dirty = false;

    return true;
}

QString CacheTableModel::getLastError()
{
    return lastError.text();
}

