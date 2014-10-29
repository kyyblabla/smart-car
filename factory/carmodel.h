#ifndef CARMODEL_H
#define CARMODEL_H

#include "basenode.h"

#include <QObject>

#include <Qt3D/qglscenenode.h>


class QTime;

class CarModel : public BaseNode
{
    Q_OBJECT
public:
    explicit CarModel(QString filePath,QObject *parent = 0);
    QGraphicsTranslation3D *getCarTranslte() const;

signals:
    //已经到达目标点
    void gotToAim(qreal x,qreal z);
public slots:
    void doKeyEvent(int keyCode);

    //移动到某个点
    void moveToPoint(qreal x,qreal z);

private slots:
    void doLeft();
    void doRight();
    void doFoward();
    void doBack();
    void doStop();
    void doRest();

    //小车开始移动,触发timer
    void carStartMove();

    //下车移动过程
    void carMove(qreal v1, qreal v2);
protected:
    void timerEvent(QTimerEvent *timer);
private:

    //左右转向时角度增加/减少的偏移值
    qreal carRotatePlud;

    //角度分片数目
    int carRotateSplice;

    //每次前进，后退的偏移量
    qreal carTranslatePlud;

    //小车当前的速度
    qreal carCurrSpeed;

    //小车通常的加速度
    qreal carAcceleration;

    //小车匀速运行时的速度
    qreal carMoveSpeedNormal;

    //下车停止标志
    bool carStop;

    //用于记录小车从上一次移动到本次移动经历的时间
     QTime*carLastMobeTime;

    //关于小车运动的计时器id
    int carTimerId;

    //timer 时间片长度
    int carTimerSplice;

    //是否需要循迹到目标点

    bool needAim;
    //目标点
    qreal carAimX;
    qreal carAimZ;

    //上次计算的距离
    qreal lastDistance;

    //循迹误差范围
    qreal carAimWrong;

    //检测是否到达目标点
    bool checkIsGetAim();
    //检测误差
    bool checkAimWrong(qreal p1,qreal p2);

};

#endif // CARMODEL_H
