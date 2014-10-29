#include "mainview.h"
#include "ui_mainview.h"

#include "factory/factoryview.h"

MainView::MainView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainView)
{
    ui->setupUi(this);


    mv=new FactoryView(this);

    ui->widget_2->layout()->addWidget(mv);

    connect(this,SIGNAL(moveToPoint(qreal,qreal)),mv,SIGNAL(moveToPoint(qreal,qreal)));

    mv->setFocus();
}

MainView::~MainView()
{
    delete ui;
}

