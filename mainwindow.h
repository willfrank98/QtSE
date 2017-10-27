#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <iostream>
#include <QMainWindow>
#include <QWheelEvent>
#include <QTimeLine>
#include <QGraphicsPixmapItem>
#include <QUrl>
#include <QAction>
#include <QSignalMapper>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QPoint>
#include <QWidget>
#include <QPushButton>
#include <QDebug>
#include <QGridLayout>
#include <QGraphicsProxyWidget>
#include <sstream>
#include <QVector>
using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public slots:
    void animFinished();
    void scalingTime(qreal x);
    void buttonClicked(const QString &text);

signals:
    void scale(qreal, qreal);
    void pressed(const QString &text);


protected:

public:

    int _numScheduledScalings;
    void wheelEvent(QWheelEvent *event);
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    QPushButton *button[128][128];
    QSignalMapper *signalMapper;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
