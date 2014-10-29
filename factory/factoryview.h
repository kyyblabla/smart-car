#ifndef MANYVIEW_H
#define MANYVIEW_H



#include <Qt3D/qglview.h>

#include <QObject>

class QGLSceneNode;
class QGraphicsRotation3D;
class QGraphicsTranslation3D;
class CarModel;
class ArrowNode;

class FactoryView : public QGLView
{
    Q_OBJECT
public:
    explicit FactoryView(QWidget *parent = 0);

    //纹理平铺
    static QGLMaterial*flatMaterial(QString path="", int width=10);

signals:
    void keyEvent(int keyCode);
    void moveToPoint(qreal x,qreal z);
private slots:
    void doGotToAim(qreal x,qreal z);
protected:
    void initializeGL(QGLPainter *painter);
    void paintGL(QGLPainter *painter);
    void keyPressEvent(QKeyEvent *e);
    void paintEvent(QPaintEvent *e);

    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
private:
    QGLSceneNode*mainNode;
    void init();
    CarModel*carModel;
    QVector3D mapPointToThis(QPoint point);

    ArrowNode *arrowNode;

    void addCar();

    int deviceNum;
    void addDevieces();


    qreal mainWidth;




};

#endif // MANYVIEW_H
