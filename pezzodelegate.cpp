#include "pezzodelegate.h"
#include "pezzo.h"
#include <QApplication>

QString PezzoDelegate::displayText(const QVariant &value, const QLocale &locale) const{
    Q_UNUSED(locale)
    Pezzo tempPezzo = value.value<Pezzo>();
    if(tempPezzo.colore==Pezzo::Colore::Nessuno)
        return QString();
    switch (tempPezzo.tipo) {
    case Pezzo::Tipo::Pedone:
        return tempPezzo.colore == Pezzo::Colore::Bianco ? QChar(0x2659) : QChar(0x265F);
    case Pezzo::Tipo::Torre:
       return tempPezzo.colore == Pezzo::Colore::Bianco ? QChar(0x2656) : QChar(0x265C);
    case Pezzo::Tipo::Cavallo:
        return tempPezzo.colore == Pezzo::Colore::Bianco ? QChar(0x2658) : QChar(0x265E);
    case Pezzo::Tipo::Alfiere:
        return tempPezzo.colore == Pezzo::Colore::Bianco ? QChar(0x2657) : QChar(0x265D);
    case Pezzo::Tipo::Regina:
        return tempPezzo.colore == Pezzo::Colore::Bianco ? QChar(0x2655) : QChar(0x265B);
    case Pezzo::Tipo::Re:
         return tempPezzo.colore == Pezzo::Colore::Bianco ? QChar(0x2654) : QChar(0x265A);
    default:
        return QString();
    }
}

PezzoDelegate::PezzoDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{

}

void PezzoDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const{
    Q_ASSERT(index.isValid());
    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);
    opt.font = QFont("Arial",20);
    const QWidget *widget = option.widget;
    QStyle *style = widget ? widget->style() : QApplication::style();
    style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, widget);
}


