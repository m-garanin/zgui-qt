#ifndef NETSOURCEDLG_H
#define NETSOURCEDLG_H

#include <QDialog>

namespace Ui {
class NetSourceDlg;
}

class NetSourceDlg : public QDialog
{
    Q_OBJECT
    
public:
    explicit NetSourceDlg(QWidget *parent = 0);
    ~NetSourceDlg();

    QString getURI();
private:
    Ui::NetSourceDlg *ui;    

};

#endif // NETSOURCEDLG_H
