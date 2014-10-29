#ifndef BASENODE_H
#define BASENODE_H


#include <Qt3D/qglscenenode.h>

#include <QObject>


class QGraphicsTranslation3D;
class QGLSceneNode;
class QGraphicsScale3D;
class QGraphicsRotation3D;


class BaseNode : public QGLSceneNode
{
    Q_OBJECT
public:
    explicit BaseNode(QString filePath, QObject *parent = 0);
    explicit BaseNode(QObject *parent = 0);
    static  QGLSceneNode *createNodeFrom3ds(QString filePath);

    void setMainScale(qreal newScale);
    void setMainRoate(qreal newRoate);
    void setMaintranslation(qreal x,qreal y,qreal z);
    QGLSceneNode*getMainNode();

signals:

protected:
    QGraphicsTranslation3D*mainTranslte; //位移
    QGraphicsScale3D*mainScale;          //缩放
    QGraphicsRotation3D*mainRotate;      //旋转

    //旋转角度
    qreal mainRoateNum;

    //缩放
    qreal mainScaleNum;

    QGLSceneNode*mainNode;

    
public slots:
    
};

#endif // BASENODE_H
