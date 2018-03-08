#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
class QTableView;
class QStandardItemModel;
class Scacchiera : public QWidget
{
    Q_OBJECT
    Q_DISABLE_COPY(Scacchiera)
public:
    explicit Scacchiera(QWidget *parent = Q_NULLPTR);
    Q_SLOT void startGame();
private:
    QTableView* m_tableView;
    QStandardItemModel* m_model;
    bool m_turnoBianco;
    Q_SLOT void preparaTurno();
    Q_SLOT void cliccato(const QModelIndex &idx);
};



#endif // WIDGET_H
