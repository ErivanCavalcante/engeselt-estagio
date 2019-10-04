#ifndef ITEMDELEGATELISTACONTATO_H
#define ITEMDELEGATELISTACONTATO_H

#include <QStyledItemDelegate>

class ItemDelegateListaContato : public QStyledItemDelegate
{
    public:
    ItemDelegateListaContato(QObject *parent);

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;


};

#endif // ITEMDELEGATELISTACONTATO_H
