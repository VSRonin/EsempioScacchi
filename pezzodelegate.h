#ifndef PEZZODELEGATE_H
#define PEZZODELEGATE_H
#include <QStyledItemDelegate>

class PezzoDelegate : public QStyledItemDelegate
{
    Q_OBJECT
    Q_DISABLE_COPY(PezzoDelegate)
public:
    QString displayText(const QVariant &value, const QLocale &locale) const Q_DECL_OVERRIDE;
    explicit PezzoDelegate(QObject* parent = Q_NULLPTR);
    void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const Q_DECL_OVERRIDE;
    QSize sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const Q_DECL_OVERRIDE;
private:
    QPixmap m_baseMovibile;
    QPixmap m_baseMangiabile;
};

#endif // PEZZODELEGATE_H
