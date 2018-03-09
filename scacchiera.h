#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QList>
#include "pezzo.h"
class QTableView;
class QStandardItemModel;
class QLabel;
class Scacchiera : public QWidget
{
    Q_OBJECT
    Q_DISABLE_COPY(Scacchiera)
public:
    explicit Scacchiera(QWidget *parent = Q_NULLPTR);
    Q_SLOT void startGame();
    Q_SLOT void retranslateUi();
private:
    QTableView* m_tableView;
    QStandardItemModel* m_model;
    QLabel* m_turnoLabel;
    bool m_turnoBianco;
    Q_SLOT void cliccato(const QModelIndex &idx);
    QPoint m_pezzoCorrente;
    void formattaMossePossibili();
    void rimuoviTuttiStati(bool rimuoviScacco);
    void cambiaTurno();
    QPoint posizioneRe(Pezzo::Colore colr) const;
    bool scacco(Pezzo::Colore colr, const QPoint& posRe) const;
    bool scacco(Pezzo::Colore colr) const;
    bool stallo(Pezzo::Colore colr) const;
    bool controllaDiagonalePedone(int rig, int colDiag, const Pezzo& pedone) const;
    bool controllaArrocco(Pezzo::Colore colr, bool aSinistra) const;
    void filtraScacco(QList<QPoint>& mosse,const QPoint& pedina) const;
    QList<QPoint> mossePossibili(const QPoint& pedina) const;
    QModelIndex indexForPoint(const QPoint& pnt) const;
};



#endif // WIDGET_H
