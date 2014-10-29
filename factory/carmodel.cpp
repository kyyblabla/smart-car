#include "carmodel.h"

#include <Qt3D/qglabstractscene.h>
#include <Qt3D/qglscenenode.h>
#include <Qt3D/qgraphicsrotation3d.h>
#include <Qt3D/qgraphicstranslation3d.h>
#include <Qt3D/qgraphicsscale3d.h>
#include <Qt3D/qgltexture2d.h>
#include <Qt3D/qglview.h>


#include <QtCore/qmath.h>
#include <QVector3D>
#include <QTime>


CarModel::CarModel(QString filePath, QObject *parent) :
    BaseNode(filePath,parent)
{

    //单位移动长度
    carTranslatePlud=1.0f;

    carRotateSplice=9;
    //单位旋转角度
    carRotatePlud=90.0f/carRotateSplice;

    //开始时小车是停止的
    carStop=true;

    carTimerId=0;

    //时间片为20ms
    carTimerSplice=20;

    //小车当前的速度
    carCurrSpeed=0.0f;

    //小车匀速运行时的速度,即 1 个时间片内走过的路程
    carMoveSpeedNormal=1.0f/carTimerSplice;

    //小车通常的加速度
    // a=v/t   下车需要100毫秒才能加速到最大速度
    carAcceleration=carMoveSpeedNormal/100;


    //carLastMobeTime=new QTime;

    //初始化时不需要循迹
    this->needAim=false;

    //初始化循迹误差范围
    carAimWrong=1.0f*5;

    carLastMobeTime=new QTime;

}



void CarModel::doKeyEvent(int keyCode)
{

    switch (keyCode) {
    case Qt::Key_W:
        this->doFoward();
        break;
    case Qt::Key_S:
        this->doBack();
        break;
    case Qt::Key_A:
        this->doLeft();
        break;
    case Qt::Key_D:
        this->doRight();
        break;
    case Qt::Key_Space:
        doStop();
        break;
    case Qt::Key_R:
        doRest();
        break;

    default:
        return;
    }
}

void CarModel::doLeft()
{
    qreal newRoate=mainRoateNum+carRotatePlud;
    setMainRoate(newRoate);
}

void CarModel::doRight()
{
    qreal newRoate=mainRoateNum-carRotatePlud;
    setMainRoate(newRoate);

}

void CarModel::doFoward()
{

    //加速度为正，开始移动
    this->carAcceleration=qAbs(carAcceleration);
    carStartMove();

}

void CarModel::doBack()
{

    //加速度为负，开始移动
    this->carAcceleration=-qAbs(carAcceleration);
    carStartMove();

}

void CarModel::doStop()
{
    if(carTimerId!=0){
        killTimer(carTimerId);
        carTimerId=0;
    }

    this->mainNode->setPosition(QVector3D(0,0,0));

}

void CarModel::doRest()
{

    doStop();

    QVector3D ct=this->mainTranslte->translate();

    ct.setX(0);
    ct.setZ(0);

    mainTranslte->setTranslate(ct);

    //this->carNode->setPosition(ct);


}

void CarModel::carStartMove()
{

    if(carTimerId==0){
        carLastMobeTime->start();
        carTimerId=startTimer(carTimerSplice);
    }
}

void CarModel::carMove(qreal v1,qreal v2)
{

    int times=carLastMobeTime->elapsed();

    carLastMobeTime->restart();

    qreal s=0.0f;
    //计算移动的距离
    //匀速运行
    if(v1==v2){

        s=carCurrSpeed*times;

    }else{

        s = v1*times+1.0f/2.0f*carAcceleration*times*times;
        //s= ((v2*v2)-(v1*v1))/(2*carAcceleration);
    }

    //获取当前变形矩阵
    QVector3D c=mainTranslte->translate();


    //得到当前旋转的角度，需要减去基数 90 度
    qreal cRoateNum=this->mainRoateNum;

    //x轴位移
    qreal xplud=qCos(cRoateNum/180*M_PI)*s;

    //z轴位移
    qreal zplud=-qSin(cRoateNum/180*M_PI)*s;

    c+=QVector3D(xplud,0,zplud);


    mainTranslte->setTranslate(c);



}

void CarModel::timerEvent(QTimerEvent *timer)
{
    Q_UNUSED(timer);



    //计算 a*t  v2=v1+a*t
    qreal pludAcceleration=carAcceleration*carTimerSplice;

    qreal v1=carCurrSpeed;
    qreal v2;

    //加速
    //加速分两种情况
    //1,加速前进,  速度为正
    //2,减速后退,  速度为负
    if(this->carAcceleration>0.0f){

        //速度为正,加速运动
        if(carCurrSpeed>=0.0f){

            carCurrSpeed+=pludAcceleration;
            //加到最大速度停止加速
            if(carCurrSpeed>carMoveSpeedNormal){
                carCurrSpeed=carMoveSpeedNormal;
            }
        }
        //减速运动,速度为0是停止减速,即停止运动
        else if(carCurrSpeed<0.0f){

            carCurrSpeed+=pludAcceleration*5;
            if(carCurrSpeed>=0.0f){
                carCurrSpeed=0.0f;
            }
        }

    }
    //减速
    else{

        //速度为正,减速前进
        if(carCurrSpeed>0.0f){

            carCurrSpeed+=pludAcceleration*5;
            //减速到0时停止运动
            if(carCurrSpeed<=0.0f){
                carCurrSpeed=0.0f;
            }
        }
        //速度为负,加速后退
        else if(carCurrSpeed<=0.0f){
            carCurrSpeed+=pludAcceleration;
            if(carCurrSpeed<(-carMoveSpeedNormal)){
                carCurrSpeed= -carMoveSpeedNormal;
            }
        }
    }

    v2=carCurrSpeed;

    //如果需要循迹
    if(this->needAim){
        //检测是否到达目标点
        bool getToAim=checkIsGetAim();
        //达到目标点，下车停止移动
        if(getToAim){
            this->needAim=false;
            carCurrSpeed=0.0f;
            emit this->gotToAim(carAimX,carAimZ);
        }
    }

    //根据现有速度数据移动小车
    carMove(v1,v2);

    //小车速度为零时，停止timer
    if(this->carCurrSpeed==0.0f){

        killTimer(carTimerId);
        carTimerId=0;
    }

}



bool CarModel::checkIsGetAim()
{
    //得到当前位置向量
    QVector3D currVec=mainTranslte->translate();

    //得到现在的x，z坐标值
    qreal cx=currVec.x();
    qreal cz=currVec.z();

    // qDebug()<<"cx:"<<cx<<" cz:"<<cz;

    //计算x，z与目标点之间的距离
    qreal dx=qAbs(cx-carAimX);
    qreal dz=qAbs(cz-carAimZ);

    qreal currDiatance=dx*dx+dz*dz;

    //与上次结果比较
    if(currDiatance>lastDistance){
        return true;
    }

    lastDistance=currDiatance;

    //  是否在允许的误差范围内
    return checkAimWrong(dx,dz);

}

bool CarModel::checkAimWrong(qreal p1, qreal p2)
{

    p1=qAbs(p1);
    p2=qAbs(p2);

    //  是否在允许的误差范围内
    if(p1<=carAimWrong&&p2<=carAimWrong){
        return true;
    }else{
        return false;
    }

}


void CarModel::moveToPoint(qreal x, qreal z)
{

    QVector3D currVec=this->mainTranslte->translate();

    //得到现在的x，z坐标值
    qreal cx=currVec.x();
    qreal cz=currVec.z();

    //计算x，z与目标点之间的距离
    //计算出的值与当前坐标值为原点构成新的坐标系
    qreal dx=x-cx;
    qreal dz=z-cz;

    //检测误差范围
    if(checkAimWrong(dx,dz)){
        return;
    }

    this->carAimX=x;
    this->carAimZ=z;
    this->needAim=true;


    qreal dtan=dz/dx;

    //以反三角函数计算tan对应的角度
    qreal droate=qAtan(dtan)*180.0f/M_PI;

    qreal delRoate=((int)(droate/360))*360;
    //取 360 度数的模
    droate=qAbs(droate-delRoate);

    //同一个tan值在2PI范围内可以对应两个角度,需要根据dx，dy来判断

    /*
            │
            │
     ───────┼───────>
          0 │
            │
            ∨
    */
    //象限1
    if(dx>=0&&dz>=0){

        droate=-droate;
    }
    //象限2
    else if(dx>=0&&dz<0){

        droate=droate;

    }else if(dx<0&&dz<0){

        droate=180.0f-droate;

    }else{

        droate=droate-180.0f;

    }

    //速度和加速度都变为正值
    carCurrSpeed=qAbs(carCurrSpeed);
    carAcceleration=qAbs(carAcceleration);
    lastDistance=dx*dx+dz*dz;

    //旋转小车
    setMainRoate(droate);

    //开始移动
    this->carStartMove();


}


QGraphicsTranslation3D *CarModel::getCarTranslte() const
{
    return mainTranslte;
}






