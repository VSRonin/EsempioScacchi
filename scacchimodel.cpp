#include "scacchimodel.h"
#include <QAbstractItemModel>
#include <QVector>
#include <QBrush>
ScacchiModel::ScacchiModel(QObject *parent)
    : QAbstractItemModel(parent)
{}

QModelIndex ScacchiModel::index(int row, int column, const QModelIndex &parent) const{
    if(parent.isValid() || row<0 || column<0 || row>=8 || column>=8 )
        return QModelIndex();
    return createIndex(row,column);
}
QModelIndex ScacchiModel::parent(const QModelIndex &index) const{
    Q_UNUSED(index)
    return QModelIndex();
}
int ScacchiModel::rowCount(const QModelIndex &parent) const {
    Q_ASSERT(!parent.isValid() || parent.model()==this);
    return parent.isValid() ? 0:8;
}
int ScacchiModel::columnCount(const QModelIndex &parent) const {
    Q_ASSERT(!parent.isValid() || parent.model()==this);
    return parent.isValid() ? 0:8;
}
QVariant ScacchiModel::data(const QModelIndex &index, int role) const{
    if(!index.isValid())
        return QVariant();
    Q_ASSERT(index.model()==this);
    if(role == Qt::DisplayRole)
        role=Qt::EditRole;
    if(role == Qt::EditRole)
        return m_data[index.row()][index.column()][0];
    if(role == StatusCellRole)
        return m_data[index.row()][index.column()][1];
    if(role == ScaccoRole)
        return m_data[index.row()][index.column()][2];
    if(role == DoppioPassoRole)
        return m_data[index.row()][index.column()][3];
    if(role == Qt::BackgroundRole)
        return QBrush(index.row()%2==index.column()%2 ? Qt::white : Qt::lightGray);
    if(role == Qt::TextAlignmentRole)
        return Qt::AlignCenter;
    return QVariant();
}
Qt::ItemFlags ScacchiModel::flags(const QModelIndex &index) const {
    if(!index.isValid())
        return Qt::NoItemFlags;
    return Qt::ItemIsEnabled | Qt::ItemNeverHasChildren;
}
bool ScacchiModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if(!index.isValid())
        return false;
    Q_ASSERT(index.model()==this);
    if(role == Qt::DisplayRole)
        role=Qt::EditRole;
    if(role == Qt::EditRole)
        m_data[index.row()][index.column()][0] = value;
    else if(role == StatusCellRole)
        m_data[index.row()][index.column()][1] = value;
    else if(role == ScaccoRole)
        m_data[index.row()][index.column()][2] = value;
    else if(role == DoppioPassoRole)
        m_data[index.row()][index.column()][3] = value;
    else
        return false;
    dataChanged(index,index,QVector<int>(1,role));
    return true;
}
QVariant ScacchiModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if(role == Qt::DisplayRole)
        role=Qt::EditRole;
    if(section<0 ||  section>=8 || role!=Qt::EditRole)
        return QVariant();
    return orientation==Qt::Horizontal ? QVariant::fromValue(QChar('A'+section)) : QVariant::fromValue(section+1);
}
