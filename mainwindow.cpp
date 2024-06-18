#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setContextMenuPolicy(Qt::CustomContextMenu);

    QSplitter* splitter = new QSplitter(Qt::Horizontal);
    splitter->addWidget(ui->listViewMenu);
    splitter->addWidget(ui->listView);
    splitter->addWidget(listViewInSplitMod);
    listViewInSplitMod->setVisible(false);
    splitter->setSizes(QList<int>() << 10 << 200 <<200);

    ui->dirsAfilesWidget->layout()->addWidget(splitter);

    ui->Buthome->setIcon(QIcon(":/icons/res/img_549868.png"));
    ui->ButparFold->setIcon(QIcon(":/icons/res/png-clipart-frames-passe-partout-glass-others-glass-angle-thumbnail.png"));
    ui->ButnVisFold->setIcon(QIcon(":/icons/res/OOjs_UI_icon_next-ltr.svg.png"));
    ui->ButprVisFold->setIcon(QIcon(":/icons/res/png-transparent-arrow-essential-left-set-solid-line-essential-ui-icon-icon.png"));
    ui->ButSearch->setIcon(QIcon(":/icons/res/ragczgop3buk_64.png"));

    QString sPath = QDir::homePath();


    sPath;
    QString homeName;

    for(int i = sPath.size() - 1; i > 0; i--)
    {
        if(sPath[i] == '/')
        {
            for(int j = i + 1; j < sPath.size(); j++)
            {
                homeName += sPath[j];
            }
            break;
        }
    }

    strModel = new QStringListModel();
    QStringList tabNames = {homeName, "Desktop", "File System"};
    strModel->setStringList(tabNames);

    ui->listViewMenu->setModel(strModel);
    historyPrev.push(sPath);

    fileModel = new QFileSystemModel(this);
    fileModel->setRootPath(sPath);

    ui->lineEdit->setText(sPath);
    ui->listView->setModel(fileModel);
    ui->listView->setViewMode(QListView::IconMode);
    ui->listView->setIconSize(QSize(70, 70));
    listViewInSplitMod->setModel(fileModel);
    listViewInSplitMod->setIconSize(QSize(70, 70));
    listViewInSplitMod->setViewMode(QListView::IconMode);

    connect(listViewInSplitMod, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(on_listView_doubleClicked(const QModelIndex &)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_listView_doubleClicked(const QModelIndex &index)
{
    QString filePath = fileModel->fileInfo(index).absoluteFilePath();
    QFileInfo file(filePath);



    if(file.isFile())
    {
        QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
    }
    else
    {
        ui->listView->setRootIndex(fileModel->setRootPath(filePath));

        if(listViewInSplitMod->isVisible())
        {
            listViewInSplitMod->setRootIndex(fileModel->setRootPath(filePath));
        }

        ui->lineEdit->setText(filePath);

        OWNsetStrModelIndex();

        historyPrev.push(filePath);
        historyNext.clear();
    }
}


void MainWindow::on_ButprVisFold_clicked()
{
    if(!historyPrev.empty()){

        qDebug() << "historyPrev: " << historyPrev.last();
        bool flag = false;
        if(historyPrev.last() == ui->lineEdit->text() && historyPrev.size() > 1){

            historyNext.push(historyPrev.pop());
            qDebug() << "historyNext: " << historyNext.last();
            flag = true;
        }

        if(!flag && historyPrev.last() != ui->lineEdit->text()){
            historyNext.push(historyPrev.last());
        }

        ui->lineEdit->setText(historyPrev.last());
        qDebug() << "historyPrev: " << historyPrev.last();
        QString filePath = historyPrev.pop();

        ui->listView->setRootIndex(fileModel->setRootPath(filePath));

        if(listViewInSplitMod->isVisible())
        {
            listViewInSplitMod->setRootIndex(fileModel->setRootPath(filePath));
        }

        OWNsetStrModelIndex();

        if(historyPrev.empty()){
            historyPrev.push(ui->lineEdit->text());
        }
    }
}

void MainWindow::on_ButnVisFold_clicked()
{
    if(!historyNext.empty()){

        historyPrev.push(ui->lineEdit->text());
        ui->lineEdit->setText(historyNext.last());
        QString filePath = historyNext.pop();
        historyPrev.push(filePath);
        qDebug() << "historyNext: " << filePath;

        if(listViewInSplitMod->isVisible())
        {
            listViewInSplitMod->setRootIndex(fileModel->setRootPath(filePath));
        }

        ui->listView->setRootIndex(fileModel->setRootPath(filePath));

        OWNsetStrModelIndex();
    }

}

void MainWindow::on_ButSearch_clicked()
{

}

void MainWindow::on_Buthome_clicked()
{
    QString sPath = "/home";
    ui->lineEdit->setText(sPath);
    ui->listView->setRootIndex(fileModel->setRootPath(sPath));

    if(listViewInSplitMod->isVisible())
    {
        listViewInSplitMod->setRootIndex(fileModel->setRootPath(sPath));
    }

    OWNsetStrModelIndex();
}

void MainWindow::on_ButparFold_clicked()
{
    QString sPath = ui->lineEdit->text();
    int size = sPath.size();


    for(int i = size - 1; i > 0; i--){

        if(sPath[i] == '/' || i == 1){
            sPath.remove(i, size - i);
            break;
        }
    }

    ui->listView->setRootIndex(fileModel->setRootPath(sPath));

    if(listViewInSplitMod->isVisible())
    {
        listViewInSplitMod->setRootIndex(fileModel->setRootPath(sPath));
    }

    ui->lineEdit->setText(sPath);
    OWNsetStrModelIndex();

}

void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{

}

void MainWindow::on_listViewMenu_clicked(const QModelIndex &index)
{
    QString filePath;


    historyPrev.push(ui->lineEdit->text());

    if(ui->lineEdit->text() != filePath)
    {
        historyNext.clear();
    }

    if(index.row() == 0)
    {
        filePath = QDir::homePath();
    }
    else if(index.row() == 1)
    {
        filePath = QDir::homePath() + QDir::separator() + "Desktop";
    }
    else if(index.row() == 2)
    {
        filePath = "/";
    }

    ui->listView->setRootIndex(fileModel->setRootPath(filePath));

    if(listViewInSplitMod->isVisible())
    {
        listViewInSplitMod->setRootIndex(fileModel->setRootPath(filePath));
    }

    ui->lineEdit->setText(filePath);

}

void MainWindow::OWNsetStrModelIndex()
{
    if(ui->lineEdit->text() == QDir::homePath())
    {
        ui->listViewMenu->setCurrentIndex(strModel->index(0));
    }
    else if(ui->lineEdit->text() == QDir::homePath() + QDir::separator() + "Desktop")
    {
        ui->listViewMenu->setCurrentIndex(strModel->index(1));
    }
    else if(ui->lineEdit->text() == "/")
    {
        ui->listViewMenu->setCurrentIndex(strModel->index(2));
    }
    else
    {
        ui->listViewMenu->setCurrentIndex(strModel->index(3));
    }
}

void MainWindow::on_actionReaload_triggered()
{
    QModelIndex topLeft = fileModel->index(0, 0);
    QModelIndex bottomRight = fileModel->index(0, 0);
    emit fileModel->dataChanged(topLeft,bottomRight);
}

void MainWindow::on_actionSplit_View_triggered(bool checked)
{
    if(checked){
        listViewInSplitMod->setVisible(true);
        //splitter
    }
    else listViewInSplitMod->setVisible(false);
}


void MainWindow::on_actionEntry_Style_2_triggered(bool checked)
{
    if(checked){
        ui->toolWidget->setVisible(true);
    }
    else{
        ui->toolWidget->setVisible(false);

    }
}

