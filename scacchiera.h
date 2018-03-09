#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
class QTableView;
class QStandardItemModel;
class QLabel;
class Pezzo;
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
    std::pair<int,int> m_pezzoCorrente;
    void calcolaMossePossibili();
    void rimuoviTuttiStati();
    void cambiaTurno();
    bool scacco() const;
    bool stallo() const;
    void controllaDiagonalePedone(int rig, int colDiag, const Pezzo& pedone);
};



#endif // WIDGET_H
