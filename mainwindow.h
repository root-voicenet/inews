#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class Connector;
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

private slots:
    void taxonomyLoaded();
};

#endif // MAINWINDOW_H
