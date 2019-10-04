#ifndef CACHETABLEMODEL_H
#define CACHETABLEMODEL_H

#include <QAbstractTableModel>
#include <QSqlRecord>
#include <QSqlDatabase>
#include <QSqlIndex>
#include <QSqlError>
#include <utility>
#include <QVector>
#include <QSqlQuery>
#include <algorithm>

#include "../configuracao.h"
#include "../query.h"

using namespace std;
using namespace orm;

class RowChanged
{
    public:
    RowChanged(QSqlRecord rc) : op(TipoOp::NENHUM), rc(rc){}

    void setarOperacao(TipoOp op){ this->op = op; }
    TipoOp pegarOperacao(){ return op; }

    void setarRecord(QSqlRecord rc){ this->rc = rc; }
    QSqlRecord& pegarRecord() { return rc; }

    private:
    TipoOp op;
    QSqlRecord rc;
};

class CacheTableModel : public QAbstractTableModel
{
    Q_OBJECT

    public:
    CacheTableModel(QObject *parent);
    virtual ~CacheTableModel() override;

    //Read-only
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    //Write-only
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value,
                     int role = Qt::EditRole) override;

    QSqlRecord record(int i) const;
    QSqlRecord record() const;

    void setRecord(int row, QSqlRecord &r);

    //Testa se o valor da coluna ja existe
    bool exists(QString coluna, QVariant valor);

    //Expand
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    bool insertRecord(int row, QSqlRecord &r);

    int indexField(QString coluna);

    bool isDirty();

    bool select(QString sql);

    bool select(QSqlQuery &q);

    bool save();

    QString getLastError();

    protected:
    //Banco de dados
    QSqlDatabase db;

    //Lista de todos os records
    QVector<RowChanged*> dados;
    QSqlRecord colunas;

    //Cache das operacoes de delete
    QVector<pair<int, RowChanged*>> cacheRemove;

    bool dirty;

    QSqlError lastError;

    //Operacoes sql usadas quando for salvar
    //Retorna uma string com sql
    virtual bool operacaoCreate(QSqlQuery &q, QSqlRecord &rc) = 0;
    virtual bool operacaoUpdate(QSqlQuery &q, QSqlRecord &rc) = 0;
    virtual bool operacaoDelete(QSqlQuery &q, QSqlRecord &rc) = 0;
};

#endif // CACHETABLEMODEL_H
