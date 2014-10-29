#include "basenode.h"

#include <Qt3D/qglabstractscene.h>

#include <Qt3D/qglabstractscene.h>
#include <Qt3D/qglscenenode.h>
#include <Qt3D/qgraphicsrotation3d.h>
#include <Qt3D/qgraphicstranslation3d.h>
#include <Qt3D/qgraphicsscale3d.h>
#include <Qt3D/qgltexture2d.h>
#include <Qt3D/qglview.h>




BaseNode::BaseNode(QString filePath, QObject *parent) :
    QGLSceneNode(parent)
{

    //创建模型节点
    mainNode=BaseNode::createNodeFrom3ds(filePath);

    //缩放比例
    mainScaleNum=0.1;

    //添加缩放值
    mainScale=new QGraphicsScale3D;
    mainScale->setScale(QVector3D(mainScaleNum,mainScaleNum,mainScaleNum));
    mainNode->addTransform(mainScale);


    mainRoateNum=0.0f;
    //旋转值
    mainRotate=new QGraphicsRotation3D;
    mainRotate->setAngle(0.0f);
    mainRotate->setAxis(QVector3D(0.0,1.0,0.0));

    mainNode->addTransform(mainRotate);

    //位移值
    mainTranslte=new QGraphicsTranslation3D;
    mainNode->addTransform(mainTranslte);

    //将模型节点作为自己的子节点
    this->addNode(mainNode);


}

BaseNode::BaseNode(QObject *parent):QGLSceneNode(parent)
{
}


QGLSceneNode *BaseNode::createNodeFrom3ds(QString filePath)
{
    QGLAbstractScene*carSense = QGLAbstractScene::loadScene(filePath);

    QGLSceneNode*node;

    if (carSense){

        node= carSense->mainNode();

    }else{

        node=new QGLSceneNode;
    }

    return node;

}

void BaseNode::setMainScale(qreal newScale)
{
    this->mainScaleNum=newScale;
    this->mainScale->setScale(QVector3D(mainScaleNum,mainScaleNum,mainScaleNum));

    emit this->updated();

}

void BaseNode::setMainRoate(qreal newRoate)
{
    this->mainRoateNum=newRoate;
    this->mainRotate->setAngle(mainRoateNum);

    emit this->updated();

}

void BaseNode::setMaintranslation(qreal x, qreal y, qreal z)
{
    QVector3D dt=this->mainTranslte->translate();

    dt.setX(x);
    dt.setX(y);
    dt.setZ(z);

    mainTranslte->setTranslate(dt);

    emit this->updated();

}
