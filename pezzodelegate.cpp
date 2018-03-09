#include "pezzodelegate.h"
#include "pezzo.h"
#include "scacchiglobal.h"
#include <QApplication>
#include <QPainter>
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
    ,m_baseMangiabile(50,50)
    ,m_baseMovibile(50,50)
{
    QPainter baseMangiabilePainter(&m_baseMangiabile);
    baseMangiabilePainter.setPen(QPen(QBrush(Qt::red),10));
    baseMangiabilePainter.drawRect(5,5,40,40);
    QPainter baseMovibilePainter(&m_baseMovibile);
    baseMovibilePainter.setPen(QPen(QBrush(QColor(108, 161, 247)),10));
    baseMovibilePainter.drawRect(5,5,40,40);
}

void PezzoDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const{
    Q_ASSERT(index.isValid());
    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);
    opt.font = QFont("Arial",20);
    if(index.data(ScaccoRole).toInt()==CellaScacco)
        opt.backgroundBrush = QBrush(Qt::red);
    switch(index.data(StatusCellRole).toInt()){
    case PezzoSelezionato:
        opt.backgroundBrush = QBrush(QColor(108, 161, 247));
        break;
    case CellaVuotaMuovibile:
        opt.backgroundBrush = QBrush(m_baseMovibile.scaled(option.rect.size()));
        break;
    case CellaMangiabile:
        opt.backgroundBrush = QBrush(m_baseMangiabile.scaled(option.rect.size()));
        break;
    default:
        break;
    }
    const QWidget *widget = option.widget;
    QStyle *style = widget ? widget->style() : QApplication::style();
    style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, widget);
}


