#include "JanelaPrincipal.h"
#include "ui_JanelaPrincipal.h"

#include <QMenu>
#include <QMessageBox>
#include "DialogoSobre.h"
#include "DialogoFiltros.h"
#include "Dpi.h"
#include "ItemDelegateListaContato.h"

JanelaPrincipal::JanelaPrincipal(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::JanelaPrincipal),
    m_ctrlContato(this)
{
    ui->setupUi(this);

    ui->edtPesquisa->setMinimumWidth(Dpi::dp(300));
    ui->areaInfo->setMinimumWidth(Dpi::dp(250));
    ui->btAcoes->setMinimumWidth(Dpi::dp(120));

    //Seta o menu do botao de acoes da lista
    QMenu *menu = new QMenu(this);

    menu->addAction("Adicionar");
    menu->addAction("Editar");
    menu->addAction("Remover");
    menu->addAction("Filtrar");

    menu->setMinimumWidth(Dpi::dp(120));

    ui->btAcoes->setMenu(menu);

    //Esconde a area das informacoes
    //Ela so deve ser apresentada ao clicar em um item da lista
    ui->areaInfo->setVisible(false);

    //Adiciona o modelo da lista
    ui->listaContatos->setModel(m_ctrlContato.pegarModelo());
    ui->listaContatos->setItemDelegate(new ItemDelegateListaContato(this));

    //Carrega a lista
    m_ctrlContato.listarContatos();
}

JanelaPrincipal::~JanelaPrincipal()
{
    delete ui;
}

void JanelaPrincipal::on_btCadastrar_clicked()
{
    //Cria o contato
    Contato c;

    //Testa se eh pra editar ou nao
    if(m_contato.m_id != 0)
        c = m_contato;

    //Coloca as variaveis do contato
    c.m_nome = ui->edtNome->text();
    c.m_dataNasc = ui->edtDataNasc->date();
    c.m_sexo = (Sexo)ui->cbSexo->currentIndex();
    c.m_email = ui->edtEmail->text();
    c.m_telefone = ui->edtTelefone->text();
    c.m_rua = ui->edtRua->text();
    c.m_numero = ui->spNumero->value();
    c.m_bairro = ui->edtBairro->text();
    c.m_cidade = ui->edtCidade->text();
    c.m_estado = ui->edtEstado->text();

    if(c.m_id == 0)
    {
        //Adiciona ao banco de dados
        if(!m_ctrlContato.adicionarContato(c))
        {
            QMessageBox::critical(this, "Erro ao adicionar o contato.", m_ctrlContato.pegarUltimoErro());
            return;
        }
    }
    else
    {
        //Atualiza o banco de dados
        if(!m_ctrlContato.atualizarContato(c))
        {
            QMessageBox::critical(this, "Erro ao atualizar o contato.", m_ctrlContato.pegarUltimoErro());
            return;
        }

        //Reverte o id do contato depois da atualizacao
        m_contato.m_id = 0;
    }

    //Carrega novamente os contatos
    m_ctrlContato.listarContatos();

    //Habilita os botoes
    habilitaBotoes(true);

    //Mostra a lista
    ui->stack->setCurrentIndex(0);
}

void JanelaPrincipal::on_btCancelar_clicked()
{
    //Habilita os botoes
    habilitaBotoes(true);

    //Mostra a lista
    ui->stack->setCurrentIndex(0);
}

void JanelaPrincipal::on_btAdd_clicked()
{
    //Antes d fazer qlq coisa limpa os campos de informações anteriores
    limparCampos();

    //Desabilita os botoes
    habilitaBotoes(false);

    //Muda pra tela de novo cadastro
    ui->stack->setCurrentIndex(1);
}

void JanelaPrincipal::on_btInfo_clicked()
{
    //Mostra um dialogo sobre a aplicação
    DialogoSobre dlg(this);

    dlg.exec();
}

void JanelaPrincipal::on_btEditar_clicked()
{
    //Testa se realmente existe algo selecionado
    int linha = ui->listaContatos->currentIndex().row();

    if(linha == -1)
        return;

    //Pega o contato da linha selecionada
    QSqlRecord rc = m_ctrlContato.pegarModelo()->record(linha);

    m_contato.converteDeRecord(rc);

    //Coloca nos campos
    ui->edtNome->setText(m_contato.m_nome);
    ui->edtDataNasc->setDate(m_contato.m_dataNasc);
    ui->cbSexo->setCurrentIndex(m_contato.m_sexo);
    ui->edtEmail->setText(m_contato.m_email);
    ui->edtTelefone->setText(m_contato.m_telefone);
    ui->edtRua->setText(m_contato.m_rua);
    ui->spNumero->setValue(m_contato.m_numero);
    ui->edtCidade->setText(m_contato.m_cidade);
    ui->edtEstado->setText(m_contato.m_estado);
    ui->edtBairro->setText(m_contato.m_bairro);

    habilitaBotoes(false);

    //Mostra a tela pra editar
    ui->stack->setCurrentIndex(1);
}

void JanelaPrincipal::on_btFechar_clicked()
{
    //Fecha as informacoes do item selecionado
    ui->areaInfo->setVisible(false);
}

void JanelaPrincipal::on_btAcoes_triggered(QAction *a)
{
    if(a->text() == "Adicionar")
    {
        on_btAdd_clicked();
    }
    else if(a->text() == "Editar")
    {
        on_btEditar_clicked();
    }
    else if(a->text() == "Remover")
    {
        on_btRemover_clicked();
    }
    else if(a->text() == "Filtrar")
    {
        //Mostra um dialogo para filtrar a pesquisa
        DialogoFiltros dlg(this);

        connect(&dlg, SIGNAL(filtrar(QString,QString,QString)),
                this, SLOT(onFiltrar(QString,QString,QString)));

        dlg.exec();
    }
}

void JanelaPrincipal::limparCampos()
{
    //Limpa td
    ui->edtNome->setText("");
    ui->edtDataNasc->setDate(QDate(2000, 1, 1));
    ui->cbSexo->setCurrentIndex(0);
    ui->edtEmail->setText("");
    ui->edtTelefone->setText("");
    ui->edtRua->setText("");
    ui->spNumero->setValue(0);
    ui->edtBairro->setText("");
    ui->edtCidade->setText("");
    ui->edtEstado->setText("");
}

void JanelaPrincipal::habilitaBotoes(bool ok)
{
    ui->edtPesquisa->setVisible(ok);
    ui->btPesquisa->setVisible(ok);
    ui->btAdd->setVisible(ok);
}

void JanelaPrincipal::on_btRemover_clicked()
{
    //Testa se algum item foi selecionado
    int linha = ui->listaContatos->currentIndex().row();

    if(linha == -1)
        return;

    //Pega o contato da linha selecionada
    QSqlRecord rc = m_ctrlContato.pegarModelo()->record(linha);

    Contato c;
    c.converteDeRecord(rc);

    //Pergunta se o usario realmente quer fazer a operação
    QMessageBox::StandardButton bt = QMessageBox::question(this, "Deseja realmente deletar?", "Essa ação não pode ser revertida.");

    if(bt == QMessageBox::No)
        return;

    //Remove o contato
    if(!m_ctrlContato.removerContato(c))
    {
        QMessageBox::critical(this, "Erro ao remover o contato.", m_ctrlContato.pegarUltimoErro());
        return;
    }

    //Esconde a area de info pois o contato atual foi deletado
    ui->areaInfo->setVisible(false);

    //Lista novamente os contatos
    m_ctrlContato.listarContatos();
}

void JanelaPrincipal::on_listaContatos_clicked(const QModelIndex &index)
{
    //Testa se realmente tem algo selecionado
    int linha = index.row();

    if(linha == -1)
        return;

    //Pega o contato do item selecionado
    QSqlRecord rc = m_ctrlContato.pegarModelo()->record(linha);

    Contato c;
    c.converteDeRecord(rc);

    //Coloca na tela as informacoes
    ui->txtNome->setText(c.m_nome);
    ui->txtEmail->setText(c.m_email);
    ui->txtTelefone->setText(c.m_telefone);
    ui->txtRua->setText(c.m_rua);
    ui->txtNum->setText(QString::number(c.m_numero));
    ui->txtCidade->setText(c.m_cidade);
    ui->txtEstado->setText(c.m_estado);
    ui->txtBairro->setText(c.m_bairro);

    //Mostra a area de informacoes
    ui->areaInfo->setVisible(true);
}

void JanelaPrincipal::onFiltrar(QString nome, QString tel, QString email)
{
    m_ctrlContato.listarContatos(nome, email, tel);
}

void JanelaPrincipal::on_btPesquisa_clicked()
{
    //So pesquisa s for digitado ao no campo de pesquisa
    if(ui->edtPesquisa->text().isEmpty())
        return;

    m_ctrlContato.listarContatos(ui->edtPesquisa->text(), "", "");
}
