#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QMainWindow>

class FactoryView;

namespace Ui {
class MainView;
}

class MainView : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainView(QWidget *parent = 0);
    ~MainView();
signals:
    void moveToPoint(qreal x,qreal z);
private slots:

private:
    Ui::MainView *ui;
     FactoryView*mv;
};

#endif // MAINVIEW_H
