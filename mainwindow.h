#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>


class Connector;
class TextEdit;

QT_FORWARD_DECLARE_CLASS(QListWidget)
QT_FORWARD_DECLARE_CLASS(QListView)
QT_FORWARD_DECLARE_CLASS(QLineEdit)

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private:
    Connector* m_connector;

    // initialize widgets
    void initWidgets();
    void setupUI();
    void setupDockablePanels();

    void selectTids(QListWidget *widget, const QList<int> &tids);
    QList<int> getSelectedTids(QListWidget *widget);

private: //widgets
    QListView *rssList, *themesList, *attachedRssList;
    QListWidget *taxThemeList, *taxGeoList;
    TextEdit *textEdit;
    QLineEdit *titleEdit;
private slots:
    void taxonomyLoaded();
    void createNode();
    void syncClicked();
    void rssItemSelected(QModelIndex index);
};

#endif // MAINWINDOW_H
