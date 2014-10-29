#include "factoryview.h"

#include "carmodel.h"
#include "arrownode.h"
#include "devicenode.h"

#include <Qt3D/qglabstractscene.h>
#include <Qt3D/qglscenenode.h>
#include <Qt3D/qglcube.h>
#include <Qt3D/qglbuilder.h>
#include <Qt3D/qglsphere.h>
#include <Qt3D/qgraphicsrotation3d.h>
#include <Qt3D/qgraphicstranslation3d.h>
#include <Qt3D/qgraphicsscale3d.h>
#include <Qt3D/qplane3d.h>

#include <Qt3D/qgltexture2d.h>

#include <QGraphicsItem>
#include <QVector3D>
#include <QQuaternion>
#include <QPropertyAnimation>
#include <QKeyEvent>

FactoryView::FactoryView(QWidget *parent) :
    QGLView(parent)
{
    //setAutoFillBackground(false);
    //setAutoBufferSwap(false);
    //this->setFormat(QGLFormat(QGL::SampleBuffers));
    this->arrowNode=NULL;


}

QGLMaterial *FactoryView::flatMaterial(QString path, int width)
{
    quint32 sz = width;

    QImage image(QSize(sz, sz), QImage::Format_ARGB32);


    QRgb col = qRgba(196, 196, 200, 255);

    if(!path.isEmpty()){
        image.load(path);
    }else{
        image.fill(col);

        QPainter painter(&image);
        QPen pen = painter.pen();
        pen.setColor(QColor(col).darker());

        pen.setWidth(2);
        painter.setPen(pen);

        for (quint32 i = 0; i < sz; i += (sz / 8))
        {
            painter.drawLine(0, i, sz-1, i);
            painter.drawLine(i, 0, i, sz-1);
        }

        painter.end();

    }


    QGLMaterial *mat = new QGLMaterial;
    QGLTexture2D *tex = new QGLTexture2D(mat);

    tex->setImage(image);
    mat->setTexture(tex);


    return mat;

    //   QGLMaterial *mat = new QGLMaterial;
    //   QGLTexture2D *tex = new QGLTexture2D(mat);

    //tex->setMaxImageSize();


}

void FactoryView::init()
{

    QGLBuilder b;
    QGLSceneNode*root=b.sceneNode();


    //地板宽度
    mainWidth=500;

    //加载地板纹理
    QGLMaterial*matFloor=flatMaterial("",mainWidth);
    int matFloorId=root->palette()->addMaterial(matFloor);


    //墙壁纹理
    QGLMaterial*matWall=flatMaterial(":/rs/wall_back.jpg",mainWidth);
    int matWallId=root->palette()->addMaterial(matWall);


    //地板矩阵
    QMatrix4x4 matrixFloor;
    matrixFloor.rotate(90,QVector3D(1.0,0.0,0.0));


    //加载前面墙壁
    b.newNode()->setObjectName(tr("back_wall"));
    b.addPane(QSizeF(mainWidth,mainWidth/2));

    QMatrix4x4 matrixBack;
    matrixBack.translate(QVector3D(0, mainWidth/4,-mainWidth/2));
    b.currentNode()->setLocalTransform(matrixBack);
    b.currentNode()->setMaterialIndex(matWallId);
    b.currentNode()->setEffect(QGL::LitDecalTexture2D);

    b.newNode()->setObjectName(tr("back_left"));
    b.addPane(QSizeF(mainWidth,mainWidth/2));

    QMatrix4x4 matrixLeft;
    matrixLeft.rotate(90.0f,QVector3D(0,1,0));
    matrixLeft.translate(QVector3D(0,mainWidth/4,-mainWidth/2));
    b.currentNode()->setLocalTransform(matrixLeft);
    b.currentNode()->setMaterialIndex(matWallId);
    b.currentNode()->setEffect(QGL::LitDecalTexture2D);


    b.newNode()->setObjectName(tr("back_right"));
    b.addPane(QSizeF(mainWidth,mainWidth/2));

    QMatrix4x4 matrixRight;
    matrixRight.rotate(90.0f,QVector3D(0,1,0));
    matrixRight.translate(QVector3D(0,mainWidth/4,mainWidth/2));
    b.currentNode()->setLocalTransform(matrixRight);
    b.currentNode()->setMaterialIndex(matWallId);
    b.currentNode()->setEffect(QGL::LitDecalTexture2D);


    //创造地板
    b.newNode()->setObjectName(tr("sky_bottom"));
    b.addPane(QSizeF(mainWidth,mainWidth));
    b.currentNode()->setMaterialIndex(matFloorId);
    b.currentNode()->setEffect(QGL::FlatReplaceTexture2D);
    b.currentNode()->setLocalTransform(matrixFloor);


    this->mainNode=b.finalizedSceneNode();

    addCar();

    addDevieces();

    camera()->setEye(camera()->eye() + QVector3D(0.0f, 200.0f, 400.0f));

}


void FactoryView::addCar()
{

    //创建小车模型
    carModel=new CarModel(":/rs/Car3.3ds",this);

    //关联小车的控制事件曹函数
    connect(this,SIGNAL(keyEvent(int)),carModel,SLOT(doKeyEvent(int)));

    connect(carModel,SIGNAL(updated()),this,SLOT(update()));
    connect(carModel,SIGNAL(gotToAim(qreal,qreal)),this,SLOT(doGotToAim(qreal,qreal)));

    connect(this,SIGNAL(moveToPoint(qreal,qreal)),carModel,SLOT(moveToPoint(qreal,qreal)));

    mainNode->addNode(carModel);

}

void FactoryView::addDevieces()
{

    deviceNum=5;

    qreal xDiff=mainWidth/deviceNum;
    qreal yDiff=xDiff;
    qreal baseDiff=50.0f;

    //设备蓝本

    DeviceNode*dnode=new DeviceNode(":/rs/device.3DS",this);
    //复制设备
    for(int i=0;i<deviceNum;i++){


        qreal startX=-mainWidth/2.0f+baseDiff+xDiff*i;

        for(int j=0;j<deviceNum;j++){

            qreal startZ= -mainWidth/2.0f+baseDiff+yDiff*j;

            DeviceNode*newNode=new DeviceNode(mainNode);

            newNode->addNode(dnode);
            {
                QMatrix4x4 mat;
                mat.translate(startX, 0.0f, startZ);
                newNode->setLocalTransform(mat);
            }
        }

    }

}





QVector3D FactoryView::mapPointToThis(QPoint point)
{
    QVector3D pick = mapPoint(point);

    // find the origin of the near plane
    QRay3D eyeline(camera()->eye(), (camera()->center() - camera()->eye()).normalized());
    QVector3D nearPlaneOrigin = eyeline.point(camera()->nearPlane());

    // from the near plane origin move up and across by the pick's XY to find the point
    // on the near plane
    QRay3D up(nearPlaneOrigin, camera()->upVector());
    QVector3D sideDir = QVector3D::crossProduct(camera()->upVector(), -eyeline.direction());
    QRay3D side(up.point(pick.y()), sideDir.normalized());
    QVector3D v = side.point(pick.x());

    // intersect the ray thru the picked point on the near plane with the floor
    QRay3D ray(camera()->eye(), v - camera()->eye());
    QPlane3D floorPlane(QVector3D(), QVector3D(0, 1, 0));
    QVector3D pointOnFloor = ray.point(floorPlane.intersection(ray));

    return pointOnFloor;

}




void FactoryView::initializeGL(QGLPainter *painter)
{

    painter->setClearColor(QColor(65, 170, 231));
    init();

}

void FactoryView::paintGL(QGLPainter *painter)
{

    mainNode->draw(painter);

}

void FactoryView::keyPressEvent(QKeyEvent *e)
{
    emit this->keyEvent(e->key());
    QGLView::keyPressEvent(e);
}

void FactoryView::paintEvent(QPaintEvent *e)
{



    QGLView::paintEvent(e);


    //    QPainter p(this);
    //    p.setBrush(QBrush(Qt::red));
    //    p.drawRect(10,10,100,100);
    //    p.end();
    //    swapBuffers();




}


//鼠标按下
void FactoryView::mousePressEvent(QMouseEvent *e)
{

    QGLView::mousePressEvent(e);



    switch (e->button()) {

    case Qt::RightButton:

        QVector3D pick =this->mapPointToThis(e->pos());

        if(!this->arrowNode){

            arrowNode= new ArrowNode(10);
            mainNode->addNode(arrowNode);
            connect(arrowNode,SIGNAL(updated()),this,SLOT(update()));

        }


        arrowNode->setCurrPosition(pick.x(),pick.z());

        emit moveToPoint(pick.x(),pick.z());

        break;
    }
}

void FactoryView::mouseReleaseEvent(QMouseEvent *e)
{
    QGLView::mouseReleaseEvent(e);

    //弹起中建
    switch (e->button()) {

    case Qt::RightButton:
        break;
    default:
        break;
    }

}

void FactoryView::mouseMoveEvent(QMouseEvent *e)
{
    QGLView::mouseMoveEvent(e);
}


void FactoryView::doGotToAim(qreal x, qreal z)
{

}



