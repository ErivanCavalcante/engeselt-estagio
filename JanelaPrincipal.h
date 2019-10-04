#ifndef JANELAPRINCIPAL_H
#define JANELAPRINCIPAL_H

#include <QMainWindow>

#include "Controllers/ControllerContato.h"

namespace Ui {
    class JanelaPrincipal;
}

class JanelaPrincipal : public QMainWindow
{
    Q_OBJECT

    public:
    explicit JanelaPrincipal(QWidget *parent = nullptr);
    ~JanelaPrincipal();

    private slots:

    void on_btCadastrar_clicked();

    void on_btCancelar_clicked();

    void on_btAdd_clicked();

    void on_btInfo_clicked();

    void on_btEditar_clicked();

    void on_btFechar_clicked();

    void on_btAcoes_triggered(QAction *a);

    void on_btRemover_clicked();

    void on_listaContatos_clicked(const QModelIndex &index);

    void onFiltrar(QString nome, QString tel, QString email);

    void on_btPesquisa_clicked();

    private:
    Ui::JanelaPrincipal *ui;

    //Usado para editar um contato
    //Se o id for nulo entao o contato eh invalido
    Contato m_contato;
    //Controller
    ControllerContato m_ctrlContato;

    void limparCampos();

    void habilitaBotoes(bool ok);
};

#endif // JANELAPRINCIPAL_H
