#include "devicenode.h"

#include <Qt3D/qgraphicstranslation3d.h>

DeviceNode::DeviceNode(QString filePath, QObject *parent) :
    BaseNode(filePath,parent)
{
    this->setMainScale(1);
}

DeviceNode::DeviceNode(QObject *parent):BaseNode(parent)
{
}

//DeviceNode::DeviceNode(QGLSceneNode *node)
//{
//    //创建模型节点
//    mainNode= new QGLSceneNode(this);
//    mainNode->addNode(node);

//    //位移值
//    mainTranslte=new QGraphicsTranslation3D;
//    mainNode->addTransform(mainTranslte);

//}
