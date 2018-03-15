#ifndef SCACCHIMODEL_H
#define SCACCHIMODEL_H

#include <QAbstractItemModel>
#include <array>
#include <QString>
#include <QVariant>
#include "scacchiglobal.h"
class ScacchiModel : public QAbstractItemModel
{
    Q_OBJECT
    Q_DISABLE_COPY(ScacchiModel)
public:
    explicit ScacchiModel(QObject* parent = Q_NULLPTR);
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    bool hasChildren(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE{
        Q_UNUSED(parent)
        return false;
    }
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    QModelIndex buddy(const QModelIndex &index) const Q_DECL_OVERRIDE {return index;}
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole)  Q_DECL_OVERRIDE {
        Q_UNUSED(section)
        Q_UNUSED(orientation)
        Q_UNUSED(role)
        Q_UNUSED(value)
        return false;
    }
private:
    std::array<std::array<std::array<QVariant,4>,8>,8> m_data;
    std::array<std::array<QString,8>,2> m_headData;
};

#endif // SCACCHIMODEL_H
