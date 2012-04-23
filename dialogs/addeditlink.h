#ifndef ADDEDITLINK_H
#define ADDEDITLINK_H

#include <QDialog>

namespace Ui {
class LinkDialog;
}

class AddEditLink : public QDialog
{
    Q_OBJECT
    
public:
    explicit AddEditLink(QWidget *parent = 0);
    ~AddEditLink();
    
    void show( const QString& address = QString(), const QString& title = QString(),
                   const QString& target = QString() );

signals:
    void addLink( const QString& address, const QString& target, const QString& title );


private slots:
    virtual void done(int button);
    void slotClearLinkCache();
private:
    Ui::LinkDialog *ui;
};

#endif // ADDEDITLINK_H
