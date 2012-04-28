#ifndef ADDICONIMAGETHREAD_H
#define ADDICONIMAGETHREAD_H

#include <QThread>
#include <QListWidget>
#include <QMutex>

class AddIconImageThread : public QThread
{
     Q_OBJECT
public:
    AddIconImageThread(QWidget* parent = 0);
    ~AddIconImageThread();

    void Start(QString dirPath, QListWidget* list);
    void Stop();

protected:
    void run();

private:
   QListWidget* listWidget_;
   QString dirPath_;   
   bool bTerminated_;
   QMutex mutex_;
};





#endif // ADDICONIMAGETHREAD_H
