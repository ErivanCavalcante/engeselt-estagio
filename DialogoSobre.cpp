#include "DialogoSobre.h"
#include "ui_DialogoSobre.h"

DialogoSobre::DialogoSobre(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogoSobre)
{
    ui->setupUi(this);
}

DialogoSobre::~DialogoSobre()
{
    delete ui;
}
