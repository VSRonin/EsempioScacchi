#include "scacchiera.h"
#include "pezzo.h"
#include <QGridLayout>
#include <QTableView>
#include <QHeaderView>
#include <QStandardItemModel>
#include <QStandardItem>
#include "pezzodelegate.h"
Scacchiera::Scacchiera(QWidget *parent)
    : QWidget(parent)
    , m_turnoBianco(true)
{
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
            m_model->setData(m_model->index(i,j),QBrush(j%2==i%2 ? Qt::white : Qt::lightGray),Qt::BackgroundRole);
            m_model->setData(m_model->index(i,j),Qt::AlignCenter,Qt::TextAlignmentRole);
        }
    }
    QGridLayout* mainlay=new QGridLayout(this);
    mainlay->addWidget(m_tableView,1,1);
    mainlay->addItem(new QSpacerItem(0,0,QSizePolicy::Expanding,QSizePolicy::Preferred),1,0);
    mainlay->addItem(new QSpacerItem(0,0,QSizePolicy::Expanding,QSizePolicy::Preferred),1,2);
    mainlay->addItem(new QSpacerItem(0,0,QSizePolicy::Preferred,QSizePolicy::Expanding),0,1);
    mainlay->addItem(new QSpacerItem(0,0,QSizePolicy::Preferred,QSizePolicy::Expanding),2,1);
    connect(m_tableView,&QTableView::clicked,this,&Scacchiera::cliccato);
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
    preparaTurno();
}
void Scacchiera::preparaTurno()
{
    for(int i=0;i<8;++i){
        for(int j=0;j<8;++j){
            const QVariant tempData = m_model->index(i,j).data();
            if(tempData.isValid() && (tempData.value<Pezzo>().colore == Pezzo::Colore::Bianco) == m_turnoBianco)
                m_model->item(i,j)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            else
                m_model->item(i,j)->setFlags(Qt::ItemIsEnabled);
        }
    }
}

void Scacchiera::cliccato(const QModelIndex& idx)
{
    if(!idx.isValid())
        return;
    Q_ASSERT(idx.model()==m_model);
    const QVariant tempData = idx.data();
    if(tempData.isNull()){
        qDebug() << "La cella cliccata e' vuota";
        return;
    }
    qDebug() << "La cella cliccata contiene: " << tempData.value<Pezzo>();
}

