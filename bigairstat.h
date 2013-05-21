#ifndef BIGAIRSTAT_H
#define BIGAIRSTAT_H

#include <QDialog>

namespace Ui {
class Bigairstat;
}

class Bigairstat : public QDialog
{
    Q_OBJECT
    
public:
    explicit Bigairstat(QWidget *parent = 0);
    ~Bigairstat();
    
    void updateStat(int fps, int btr);
private:
    Ui::Bigairstat *ui;
};

#endif // BIGAIRSTAT_H
