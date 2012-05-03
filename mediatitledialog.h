#ifndef MEDIATITLEDIALOG_H
#define MEDIATITLEDIALOG_H

#include <QDialog>
#include <QString>

namespace Ui {
class MediaTitleDialog;
}

class MediaTitleDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit MediaTitleDialog(QWidget *parent = 0);
    ~MediaTitleDialog();

    void setTitle(const QString& title);
    void setDescription(const QString& description);

    QString getTitle() const;
    QString getDescription() const;
private:
    Ui::MediaTitleDialog *ui;
};

#endif // MEDIATITLEDIALOG_H
