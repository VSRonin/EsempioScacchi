#include "scacchiera.h"
#include "pezzo.h"
#include <QGridLayout>
#include <QTableView>
#include <QHeaderView>
#include <QStandardItemModel>
#include <QStandardItem>
#include "pezzodelegate.h"
#include "scacchiglobal.h"
#include <QLabel>
Scacchiera::Scacchiera(QWidget *parent)
    : QWidget(parent)
    , m_turnoBianco(true)
    , m_pezzoCorrente(-1,-1)
{
    m_turnoLabel = new QLabel(this);
    m_turnoLabel->setAlignment(Qt::AlignCenter);
    m_turnoLabel->setFont(QFont("Arial",20));
    m_tableView=new QTableView(this);
    m_model=new QStandardItemModel(this);
    m_model->insertRows(0,8);
    m_model->insertColumns(0,8);
    m_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    m_tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    m_tableView->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    m_tableView->setModel(m_model);
    m_tableView->setSortingEnabled(false);
    m_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tableView->setItemDelegate(new PezzoDelegate(this));
    m_tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_tableView->setSelectionBehavior(QAbstractItemView::SelectItems);
    for(int i=0;i<8;++i){
        m_tableView->horizontalHeader()->resizeSection(i,50);
        m_tableView->verticalHeader()->resizeSection(i,50);
        m_model->setHeaderData(i,Qt::Horizontal,QChar('A'+i));
        m_model->setHeaderData(i,Qt::Vertical,i+1);
        for(int j=0;j<8;++j){
            m_model->setItemData(m_model->index(i,j), QMap<int,QVariant>{{
                std::make_pair<int,QVariant>(Qt::BackgroundRole,QBrush(j%2==i%2 ? Qt::white : Qt::lightGray))
                , std::make_pair<int,QVariant>(Qt::TextAlignmentRole,Qt::AlignCenter)
            }});
            m_model->item(i,j)->setFlags(Qt::ItemIsEnabled);
        }
    }
    QGridLayout* mainlay=new QGridLayout(this);
    mainlay->addWidget(m_tableView,2,1);
    mainlay->addWidget(m_turnoLabel,1,1);
    mainlay->addItem(new QSpacerItem(0,0,QSizePolicy::Expanding,QSizePolicy::Preferred),1,0);
    mainlay->addItem(new QSpacerItem(0,0,QSizePolicy::Expanding,QSizePolicy::Preferred),1,2);
    mainlay->addItem(new QSpacerItem(0,0,QSizePolicy::Preferred,QSizePolicy::Expanding),0,1);
    mainlay->addItem(new QSpacerItem(0,0,QSizePolicy::Preferred,QSizePolicy::Expanding),3,1);
    connect(m_tableView,&QTableView::clicked,this,&Scacchiera::cliccato);
    retranslateUi();
    startGame();
}
void Scacchiera::startGame(){
    m_turnoBianco=true;
    Pezzo tempPezzo(Pezzo::Tipo::Pedone,Pezzo::Colore::Nero);
    for(int i=0;i<8;++i){
        m_model->setData(m_model->index(1,i),QVariant::fromValue(tempPezzo));
    }
    tempPezzo.colore = Pezzo::Colore::Bianco;
    for(int i=0;i<8;++i){
        m_model->setData(m_model->index(6,i),QVariant::fromValue(tempPezzo));
    }
    tempPezzo.tipo=Pezzo::Tipo::Torre;
    m_model->setData(m_model->index(7,0),QVariant::fromValue(tempPezzo));
    m_model->setData(m_model->index(7,7),QVariant::fromValue(tempPezzo));
    tempPezzo.colore = Pezzo::Colore::Nero;
    m_model->setData(m_model->index(0,0),QVariant::fromValue(tempPezzo));
    m_model->setData(m_model->index(0,7),QVariant::fromValue(tempPezzo));
    tempPezzo.colore = Pezzo::Colore::Bianco;
    tempPezzo.tipo=Pezzo::Tipo::Cavallo;
    m_model->setData(m_model->index(7,1),QVariant::fromValue(tempPezzo));
    m_model->setData(m_model->index(7,6),QVariant::fromValue(tempPezzo));
    tempPezzo.colore = Pezzo::Colore::Nero;
    m_model->setData(m_model->index(0,1),QVariant::fromValue(tempPezzo));
    m_model->setData(m_model->index(0,6),QVariant::fromValue(tempPezzo));
    tempPezzo.colore = Pezzo::Colore::Bianco;
    tempPezzo.tipo=Pezzo::Tipo::Alfiere;
    m_model->setData(m_model->index(7,2),QVariant::fromValue(tempPezzo));
    m_model->setData(m_model->index(7,5),QVariant::fromValue(tempPezzo));
    tempPezzo.colore = Pezzo::Colore::Nero;
    m_model->setData(m_model->index(0,2),QVariant::fromValue(tempPezzo));
    m_model->setData(m_model->index(0,5),QVariant::fromValue(tempPezzo));
    tempPezzo.colore = Pezzo::Colore::Bianco;
    tempPezzo.tipo=Pezzo::Tipo::Re;
    m_model->setData(m_model->index(7,4),QVariant::fromValue(tempPezzo));
    tempPezzo.colore = Pezzo::Colore::Nero;
    m_model->setData(m_model->index(0,4),QVariant::fromValue(tempPezzo));
    tempPezzo.colore = Pezzo::Colore::Bianco;
    tempPezzo.tipo=Pezzo::Tipo::Regina;
    m_model->setData(m_model->index(7,3),QVariant::fromValue(tempPezzo));
    tempPezzo.colore = Pezzo::Colore::Nero;
    m_model->setData(m_model->index(0,3),QVariant::fromValue(tempPezzo));
}

void Scacchiera::retranslateUi()
{
    setWindowTitle(tr("Scacchi"));
    m_turnoLabel->setText(tr("Turno di %1").arg(m_turnoBianco ? QChar(0x2654) : QChar(0x265A)));
}


void Scacchiera::cliccato(const QModelIndex& idx)
{
    if(!idx.isValid())
        return;
    Q_ASSERT(idx.model()==m_model);
    if(m_pezzoCorrente.first<0){ //nessun pezzo selezionato
        const QVariant tempData = idx.data();
        if(tempData.isValid()){ //la cella contiene un pezzo
            Pezzo tempPezzo = tempData.value<Pezzo>();
            if((tempPezzo.colore==Pezzo::Colore::Bianco)==m_turnoBianco){ //il pezzo selezionato e' del giocatore che ha il turno
                m_pezzoCorrente.first = idx.row();
                m_pezzoCorrente.second = idx.column();
                m_model->setData(idx,PezzoSelezionato,StatusCellRole);
                calcolaMossePossibili();
            }
        }
    }
    else{
        if(m_pezzoCorrente.first == idx.row() && m_pezzoCorrente.second == idx.column()){ //deseleziona pezzo corrente
            m_pezzoCorrente.first = -1;
            m_pezzoCorrente.second = -1;
            rimuoviTuttiStati();
        }
        switch(idx.data(StatusCellRole).toInt()){
        case CellaMangiabile:
        case CellaVuotaMuovibile:{
            Pezzo tempPezzo = m_model->index(m_pezzoCorrente.first,m_pezzoCorrente.second).data().value<Pezzo>();
            tempPezzo.primaMossa = false;
            m_model->setData(idx,QVariant::fromValue(tempPezzo));
            m_model->setData(m_model->index(m_pezzoCorrente.first,m_pezzoCorrente.second),QVariant());
            cambiaTurno();
        }break;
        case CellaScacco:
            Q_UNREACHABLE();
        default: break;
        }
    }
}

void Scacchiera::calcolaMossePossibili()
{
    if(m_pezzoCorrente.first<0) return;
    Pezzo pezzoCorrente = m_model->index(m_pezzoCorrente.first,m_pezzoCorrente.second).data().value<Pezzo>();
    switch(pezzoCorrente.tipo){
    case Pezzo::Tipo::Pedone:{
        Q_ASSERT(m_pezzoCorrente.first != 0 && m_pezzoCorrente.first !=7);
        const int rigaAzione = m_pezzoCorrente.first + (pezzoCorrente.colore==Pezzo::Colore::Bianco ? -1:1);
        if(!m_model->index(rigaAzione,m_pezzoCorrente.second).data().isValid()){ // la cella di fronte e' libera
            m_model->setData(m_model->index(rigaAzione,m_pezzoCorrente.second),CellaVuotaMuovibile,StatusCellRole);
            if(pezzoCorrente.primaMossa){
                const int doppiaRigaAzione = rigaAzione + (pezzoCorrente.colore==Pezzo::Colore::Bianco ? -1:1);
                if(!m_model->index(doppiaRigaAzione,m_pezzoCorrente.second).data().isValid()){ // la cella 2 avanti e' libera
                    m_model->setData(m_model->index(doppiaRigaAzione,m_pezzoCorrente.second),CellaVuotaMuovibile,StatusCellRole);
                }
            }
        }
        controllaDiagonalePedone(rigaAzione,m_pezzoCorrente.second+1,pezzoCorrente);
        controllaDiagonalePedone(rigaAzione,m_pezzoCorrente.second-1,pezzoCorrente);
        }break;
    }
}

void Scacchiera::controllaDiagonalePedone(int rig, int colDiag, const Pezzo& pedone){
    if(rig<0 || rig>=8 || colDiag<0 || colDiag>=8 || pedone.tipo!=Pezzo::Tipo::Pedone)
        return;
    const QVariant cellaDiagonale = m_model->index(rig,colDiag).data();
    if(cellaDiagonale.isValid()){ // c'e' un altro pezzo nella cella in diagonale
        const Pezzo tempPezzo = cellaDiagonale.value<Pezzo>();
        if(tempPezzo.colore != pedone.colore) // il pezzo e' dell'avversario
            m_model->setData(m_model->index(rig,colDiag),CellaMangiabile,StatusCellRole);
    }
}

void Scacchiera::rimuoviTuttiStati()
{
    for(int i=0;i<8;++i){
         for(int j=0;j<8;++j)
            m_model->setData(m_model->index(i,j),QVariant(),StatusCellRole);
    }

}

void Scacchiera::cambiaTurno()
{
    m_pezzoCorrente.first = -1;
    m_pezzoCorrente.second = -1;
    m_turnoBianco = !m_turnoBianco;
    m_turnoLabel->setText(tr("Turno di %1").arg(m_turnoBianco ? QChar(0x2654) : QChar(0x265A)));
    rimuoviTuttiStati();
}

