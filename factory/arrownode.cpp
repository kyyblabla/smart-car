#include "arrownode.h"

#include <Qt3D/qglabstractscene.h>
#include <Qt3D/qglscenenode.h>
#include <Qt3D/qgraphicsrotation3d.h>
#include <Qt3D/qgraphicstranslation3d.h>
#include <Qt3D/qgraphicsscale3d.h>
#include <Qt3D/qgltexture2d.h>
#include <Qt3D/qglview.h>
#include <Qt3D/qglbuilder.h>
#include <Qt3D/qglcube.h>


#include <QtCore/qmath.h>
#include <QVector3D>
#include <QTime>
#include <QPropertyAnimation>


ArrowNode::ArrowNode(qreal scale, QObject *parent) :
    QGLSceneNode(parent)
{

    this->arrowScaleNum=scale;

    QGLMaterial *mat = new QGLMaterial;
    mat->setColor(QColor(135, 208, 81));

    QGLBuilder  mBulider;

    //创造箭头
    mBulider.pushNode()->setObjectName(tr("arrow"));

    QGeometryData triangle;
    QVector3D pa(-1, 2, 0);
    QVector3D pb(0, 0, 0);
    QVector3D pc(0, 1, 0);
    QVector3D pd(1, 2, 0);

    triangle.appendVertex(pa, pb, pc, pd);
    mBulider.addTriangleStrip(triangle);

    arrowNode=mBulider.finalizedSceneNode();


    arrowScale=new QGraphicsScale3D;
    arrowScale->setScale(QVector3D(arrowScaleNum,arrowScaleNum,arrowScaleNum));
    arrowNode->addTransform(arrowScale);

    arrowTranslte=new QGraphicsTranslation3D;
    arrowNode->addTransform(arrowTranslte);

    arrowRotate=new QGraphicsRotation3D;
    arrowRotate->setAngle(0.0f);
    arrowRotate->setAxis(QVector3D(0.0f,1.0f,0.0f));
    arrowNode->addTransform(arrowRotate);

    this->addNode(arrowNode);


    this->setMaterial(mat);
    this->setEffect(QGL::LitMaterial);

    //    QPropertyAnimation *anim = new QPropertyAnimation(arrowRotate, "angle");

    //    anim->setDuration(2000);
    //    anim->setStartValue(0);
    //    anim->setEndValue(360.0f);
    //    anim->setLoopCount(-1);
    //    anim->start();


}

void ArrowNode::setCurrPosition(qreal x, qreal z)
{
    QVector3D ct= arrowTranslte->translate();

    ct.setX(x);
    ct.setZ(z);

    arrowTranslte->setTranslate(ct);

    emit this->updated();

}





