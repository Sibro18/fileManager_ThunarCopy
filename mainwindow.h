#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>
#include <QVBoxLayout>
#include <QDebug>
#include <QTextStream>
#include <QDesktopServices>
#include <QUrl>
#include <QSplitter>
#include <QStack>
#include <QStringListModel>
#include <QListView>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_listView_doubleClicked(const QModelIndex &index);

    void on_ButprVisFold_clicked();

    void on_ButnVisFold_clicked();

    void on_ButSearch_clicked();

    void on_lineEdit_textChanged(const QString &arg1);

    void on_Buthome_clicked();

    void on_ButparFold_clicked();

    void on_listViewMenu_clicked(const QModelIndex &index);

    void OWNsetStrModelIndex();

 //   void on_listView_right_click(QMouseEvent &event);

    void on_actionReaload_triggered();

    void on_actionSplit_View_triggered(bool checked);

    void on_actionEntry_Style_2_triggered(bool checked);

private:
    Ui::MainWindow *ui;
    QFileSystemModel* fileModel;
    QStack<QString> historyPrev;
    QStack<QString> historyNext;
    QStringListModel* strModel;
    QListView* listViewInSplitMod = new QListView();
    QSplitter* SplitMod= new QSplitter(Qt::Horizontal, this);
};
#endif // MAINWINDOW_H
