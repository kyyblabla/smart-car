#ifndef ARROWNODE_H
#define ARROWNODE_H


#include <Qt3D/qglscenenode.h>


#include <QObject>


class QGraphicsScale3D;
class QGraphicsTranslation3D;
class QGraphicsRotation3D;
class QGLBuilder;

class ArrowNode : public QGLSceneNode
{
    Q_OBJECT
public:
    explicit ArrowNode(qreal scale,QObject *parent = 0);
    void setCurrPosition(qreal x, qreal z);
    void setVisiable(bool visable);
signals:
public slots:
    
private:
    qreal arrowScaleNum;
    QGraphicsScale3D*arrowScale;          //缩放
    QGraphicsTranslation3D*arrowTranslte; //位移
    QGraphicsRotation3D*arrowRotate;      //旋转

    QGLSceneNode*arrowNode;

    bool visable;

};

#endif // ARROWNODE_H
