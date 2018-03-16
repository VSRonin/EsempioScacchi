#ifndef OGGETTOSCACCHI_H
#define OGGETTOSCACCHI_H
#include <QObject>
#include <QList>
#include <QPoint>
#include <QStack>
#include "pezzo.h"
class QStandardItemModel;
class QAbstractItemModel;
class Mossa;
class OggettoScacchi : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QAbstractItemModel* model READ model)
    Q_DISABLE_COPY(OggettoScacchi)
public:
    explicit OggettoScacchi(QObject *parent = Q_NULLPTR);
    Q_SLOT void startGame();
    Q_SLOT bool usaPezzo(const QPoint& fromPt);
    Q_SLOT bool muovi(const QPoint& toPt);
    Q_SLOT bool usaPezzo(const QModelIndex& fromIdx);
    Q_SLOT bool muovi(const QModelIndex& toIdx);
    Q_SLOT bool muovi(const QModelIndex& fromIdx,const QModelIndex& toIdx);
    Q_SLOT bool muovi(const QPoint& fromPt,const QPoint& toPt);
    Q_SLOT bool trasformaPedone(Pezzo::Tipo typ);
    Q_SLOT bool undoMossa();
    Q_SLOT bool redoMossa();
    Q_SLOT bool muovi(const Mossa& mossa);

    Q_SIGNAL void mangiato(const Pezzo& pedina);
    Q_SIGNAL void scaccoMatto(Pezzo::Colore vincitore);
    Q_SIGNAL void stallo();
    Q_SIGNAL void partitaIniziata();
    Q_SIGNAL void turnoDi(Pezzo::Colore colr);
    Q_SIGNAL void richiediTrasformazionePedone();
    Q_SIGNAL void undoEnabled(bool);
    Q_SIGNAL void redoEnabled(bool);
    QAbstractItemModel* model() const;
    const QStack<Mossa>& registroMosse() const;
private:
    bool muovi(const Mossa& mossa, bool salvaRedo);
    bool muovi(const QModelIndex& toIdx, Pezzo::Tipo typPedone, bool clearRedo = true);
    bool muovi(const QPoint& fromPt,const QPoint& toPt, Pezzo::Tipo typPedone, bool clearRedo = true);
    QPoint m_pedoneDaTrasformare;
    QAbstractItemModel* m_model;
    Pezzo::Colore m_turnoDi;
    QPoint m_pezzoCorrente;
    void formattaMossePossibili();
    void rimuoviTuttiStati(Pezzo::Colore colr,bool rimuoviScacco);
    void rimuoviTuttiStati(bool rimuoviScacco);
    void cambiaTurno();
    QPoint posizioneRe(Pezzo::Colore colr) const;
    bool scacco(Pezzo::Colore colr, const QPoint& posRe) const;
    bool scacco(Pezzo::Colore colr) const;
    bool isStallo(Pezzo::Colore colr) const;
    bool controllaDiagonalePedone(int rigaPartenza, int rigaArrivo, int colDiag, const Pezzo& pedone) const;
    bool controllaArrocco(Pezzo::Colore colr, bool aSinistra) const;
    void filtraScacco(QList<QPoint>& mosse,const QPoint& pedina) const;
    QList<QPoint> mossePossibili(const QPoint& pedina, bool cntrlArrocco=false) const;
    QModelIndex indexForPoint(const QPoint& pnt) const;
    QPoint pointForIndex(const QModelIndex& idx) const;
    QStack<Mossa> m_registroMosse;
    QStack<Mossa> m_redoStack;
};
#endif // OGGETTOSCACCHI_H
