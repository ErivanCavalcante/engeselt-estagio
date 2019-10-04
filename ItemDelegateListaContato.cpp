#include "ItemDelegateListaContato.h"
#include <QPainter>
#include <QSqlRecord>
#include "Models/ModelContato.h"

ItemDelegateListaContato::ItemDelegateListaContato(QObject *parent)
{

}

QSize ItemDelegateListaContato::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize(100, 48);
}

void ItemDelegateListaContato::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    ModelContato *mdl = (ModelContato*)index.model();

    if(!mdl)
        return;

    QSqlRecord rc = mdl->record(index.row());

    QBrush cor;
    QFont ft = option.font;

    ft.setPointSize(12);

    //Se esta selecionado
    if(option.state & QStyle::State_Selected)
    {
        painter->fillRect(option.rect, option.palette.highlight());
        cor = option.palette.highlightedText();
    }
    else {
        cor = option.palette.text();
    }

    painter->setFont(ft);
    painter->setBrush(cor);

    QRect rt = option.rect;
    rt.adjust(10, 10, -10, -10);

    QPixmap img(rt.height(), rt.height());

    unsigned sexo = rc.value("sexo").toUInt();

    if(sexo == 1)
        img.load(":/Icones/gender-female.png");
    else if(sexo == 2)
        img.load(":/Icones/gender-male.png");

    painter->setRenderHint(QPainter::Antialiasing);

    painter->drawPixmap(rt.x(), rt.y(), rt.height(), rt.height(), img);
    painter->drawText(rt, Qt::AlignCenter, rc.value("nome").toString());
    painter->drawText(rt, Qt::AlignVCenter | Qt::AlignRight, rc.value("telefone").toString());
}
