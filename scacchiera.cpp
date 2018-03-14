#include "oggettoscacchi.h"
#include "scacchiera.h"
#include "pezzo.h"
#include <QGridLayout>
#include <QTableView>
#include <QHeaderView>
#include "pezzodelegate.h"
#include "scacchiglobal.h"
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
Scacchiera::Scacchiera(QWidget *parent)
    : QWidget(parent)
{
    m_turnoLabel = new QLabel(this);
    m_turnoLabel->setAlignment(Qt::AlignCenter);
    m_turnoLabel->setFont(QFont("Arial",20));
    m_tableView=new QTableView(this);
    m_scacchi = new OggettoScacchi(this);
    m_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    m_tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    m_tableView->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    m_tableView->setModel(m_scacchi->model());
    m_tableView->setSortingEnabled(false);
    m_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tableView->setItemDelegate(new PezzoDelegate(this));
    m_tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_tableView->setSelectionBehavior(QAbstractItemView::SelectItems);
    for(int i=0;i<8;++i){
        m_tableView->horizontalHeader()->resizeSection(i,50);
        m_tableView->verticalHeader()->resizeSection(i,50);
    }
    QGridLayout* mainlay=new QGridLayout(this);
    mainlay->addWidget(m_tableView,2,1);
    mainlay->addWidget(m_turnoLabel,1,1);
    mainlay->addItem(new QSpacerItem(0,0,QSizePolicy::Expanding,QSizePolicy::Preferred),1,0);
    mainlay->addItem(new QSpacerItem(0,0,QSizePolicy::Expanding,QSizePolicy::Preferred),1,2);
    mainlay->addItem(new QSpacerItem(0,0,QSizePolicy::Preferred,QSizePolicy::Expanding),0,1);
    mainlay->addItem(new QSpacerItem(0,0,QSizePolicy::Preferred,QSizePolicy::Expanding),3,1);
    connect(m_tableView,&QTableView::clicked,this,&Scacchiera::cliccato);
    connect(m_scacchi,&OggettoScacchi::scaccoMatto,this,&Scacchiera::segnalaScaccoMatto);
    connect(m_scacchi,&OggettoScacchi::turnoDi,this,&Scacchiera::turnoDi);
    setWindowTitle(tr("Scacchi"));
    m_scacchi->startGame();
}





void Scacchiera::cliccato(const QModelIndex& idx)
{
    if(!idx.isValid())
        return;
    if(!m_scacchi->usaPezzo(idx))
        m_scacchi->muovi(idx);
}

void Scacchiera::segnalaScaccoMatto(Pezzo::Colore vincitore){
    QMessageBox::information(this,tr("Scacco Matto"),tr("Il Giocatore %1 Vince!").arg(vincitore == Pezzo::Colore::Nero ? tr("Nero"):tr("Bianco")));
}

void Scacchiera::segnalaStallo()
{
    QMessageBox::information(this,tr("Stallo"),tr("Pareggio!"));
}

void Scacchiera::turnoDi(Pezzo::Colore giocatore)
{
    m_turnoLabel->setText(tr("Turno di %1").arg(giocatore == Pezzo::Colore::Bianco ? QChar(0x2654) : QChar(0x265A)));
}

void Scacchiera::pedoneDaTrasformare()
{
    QMessageBox domandaTrasformazione;
    domandaTrasformazione.setWindowTitle(tr("Trasforma Pedone"));
    domandaTrasformazione.setText(tr("In cosa vuoi trasformare il pedone?"));
    QPushButton *reginaBut =domandaTrasformazione.addButton(tr("Regina"),QMessageBox::AcceptRole);
    QPushButton *torreBut =domandaTrasformazione.addButton(tr("Torre"),QMessageBox::AcceptRole);
    QPushButton *alfiereBut =domandaTrasformazione.addButton(tr("Alfiere"),QMessageBox::AcceptRole);
    QPushButton *cavalloBut =domandaTrasformazione.addButton(tr("Cavallo"),QMessageBox::AcceptRole);
    domandaTrasformazione.exec();
    if(domandaTrasformazione.clickedButton()==cavalloBut)
       Q_ASSUME(m_scacchi->trasformaPedone(Pezzo::Tipo::Cavallo));
    else if(domandaTrasformazione.clickedButton()==torreBut)
       Q_ASSUME(m_scacchi->trasformaPedone(Pezzo::Tipo::Torre));
    else if(domandaTrasformazione.clickedButton()==reginaBut)
       Q_ASSUME(m_scacchi->trasformaPedone(Pezzo::Tipo::Regina));
    else if(domandaTrasformazione.clickedButton()==alfiereBut)
       Q_ASSUME(m_scacchi->trasformaPedone(Pezzo::Tipo::Alfiere));
}


