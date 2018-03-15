#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QList>
#include "pezzo.h"
class OggettoScacchi;
class QTableView;
class QLabel;
class QPushButton;
class Scacchiera : public QWidget
{
    Q_OBJECT
    Q_DISABLE_COPY(Scacchiera)
public:
    explicit Scacchiera(QWidget *parent = Q_NULLPTR);
private:
    QTableView* m_tableView;
    OggettoScacchi* m_scacchi;
    QLabel* m_turnoLabel;
    QPushButton* m_undoButton;
    QPushButton* m_redoButton;
    Q_SLOT void cliccato(const QModelIndex &idx);
    Q_SLOT void segnalaScaccoMatto(Pezzo::Colore vincitore);
    Q_SLOT void segnalaStallo();
    Q_SLOT void turnoDi(Pezzo::Colore giocatore);
    Q_SLOT void pedoneDaTrasformare();
};



#endif // WIDGET_H
