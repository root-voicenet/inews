#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>


namespace Ui {
class MainWindow;
}

class Connector;

QT_FORWARD_DECLARE_CLASS(QListWidget)
class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private:
    Ui::MainWindow *ui;
    Connector* m_connector;

    // initialize widgets
    void initWidgets();

    void selectTids(QListWidget *widget, const QList<int> &tids);
    QList<int> getSelectedTids(QListWidget *widget);
private slots:
    void taxonomyLoaded();
    void createNode();
    void syncClicked();
    void rssItemSelected(QModelIndex index);
};

#endif // MAINWINDOW_H
