#include "DialogoFiltros.h"
#include "ui_DialogoFiltros.h"

DialogoFiltros::DialogoFiltros(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogoFiltros)
{
    ui->setupUi(this);

    ui->edtTel->setVisible(false);
    ui->edtNome->setVisible(false);
    ui->edtEmail->setVisible(false);
}

DialogoFiltros::~DialogoFiltros()
{
    delete ui;
}

void DialogoFiltros::on_cbNome_stateChanged(int arg)
{
    ui->edtNome->setVisible(arg);
}

void DialogoFiltros::on_cbTel_stateChanged(int arg)
{
    ui->edtTel->setVisible(arg);
}

void DialogoFiltros::on_cbEmail_stateChanged(int arg)
{
    ui->edtEmail->setVisible(arg);
}

void DialogoFiltros::on_btOk_clicked()
{
    emit filtrar(ui->edtNome->text(), ui->edtTel->text(), ui->edtEmail->text());
}
