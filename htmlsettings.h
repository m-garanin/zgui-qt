#ifndef HTMLSETTINGS_H
#define HTMLSETTINGS_H

#include <QDialog>
#include <QLabel>
#include <QWebPage>
#include <QWebFrame>
#include <QWebView>

namespace Ui {
class HTMLSettings;
}

class HTMLSettings : public QDialog
{
    Q_OBJECT
    
public:
    explicit HTMLSettings(QWidget *parent = 0);
    ~HTMLSettings();

    void openURL(QUrl url);
    void showFrame(QImage img);
private:
    Ui::HTMLSettings *ui;
    QWebView* m_view;
    QLabel* m_lbl;

};

#endif // HTMLSETTINGS_H
