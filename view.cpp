#include "view.h"
#include <QKeyEvent>
#include <QGraphicsTextItem>
#include <QGuiApplication>

#include <QDebug>

#include "graphicstree.h"
#include "graphicsfood.h"

View::View(QSize frameSize, QWidget *parent)
  : QGraphicsView(parent),
    m_OriginalSize(frameSize.width(), frameSize.height()),
    m_frameSize(frameSize.width(), frameSize.height()), m_IsNeedToRestore(false),
    m_scaleLevel(scaleLvl::MIN), m_centerOnPoint(0,0), m_scaler(8, 2) {
  setScene(&m_scene);
  setTransformationAnchor(QGraphicsView::NoAnchor);
  setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
  setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
  setFixedSize(512,512);

  QRect scene = QRect(-m_OriginalSize/2, m_OriginalSize/2);
  m_scene.setSceneRect(scene);
  setSceneRect(scene);
  fitInView(m_scene.sceneRect(), Qt::KeepAspectRatio);

  m_scene.addItem(new GraphicsTree(QRect(50, 80, 100, 100)));
  m_scene.addItem(new GraphicsTree(QRect(250, 60, 150, 150)));
  m_scene.addItem(new GraphicsTree(QRect(-60, -30, 50, 50)));
}

void View::resizeEvent(QResizeEvent *event) {
  QPoint centerPos = m_centerOnPoint.toPoint();
  QRect centerRect = QRect((centerPos - m_frameSize/2), (centerPos + m_frameSize/2));
  this->setSceneRect(centerRect);
  this->fitInView(centerRect, Qt::KeepAspectRatio);
  QGraphicsView::resizeEvent(event);
}


void View::wheelEvent(QWheelEvent *event)
{
  if ( hasFocus() )
  {
    qreal scaleStep = 1;
    if(event->delta() > 0)
    {
        if(m_scaler.levelUp())
        {
            scaleStep = m_scaler.getScale();
            m_frameSize /= scaleStep;
            QPointF cursorPoint = mapToScene(event->pos());
            correctView(cursorPoint, scaleStep);
            scale(scaleStep, scaleStep);
            centerOn(m_centerOnPoint);
        }
    }
    else
    {
        if(m_scaler.levelDown())
        {
            if(m_scaler.isMaxlevel())
            {
                m_centerOnPoint = QPointF(0,0);
            }
            else
            {
                scaleStep = 1 / m_scaler.getScale();
                m_frameSize /= scaleStep;
                QPointF cursorPoint = mapToScene(event->pos());
                correctView(cursorPoint, scaleStep);
            }
            scale(scaleStep, scaleStep);
            centerOn(m_centerOnPoint);
        }
    }
  }
}

void View::correctView(const QPointF& cursorPoint, qreal scaleNum)
{
    QPointF deltaPoints = (cursorPoint - m_centerOnPoint) / scaleNum;
    m_centerOnPoint = cursorPoint - deltaPoints;
    QRectF viewRect = QRectF(m_centerOnPoint - m_frameSize/2, m_centerOnPoint + m_frameSize/2);
    QRectF originalRect = QRectF(-m_OriginalSize/2, m_OriginalSize/2);
    if(!originalRect.contains(viewRect))
    {
        QPointF delta(0,0);
        if(viewRect.topLeft().x() < originalRect.topLeft().x())
        {
            delta.setX(originalRect.topLeft().x() - viewRect.topLeft().x());
        }
        else
        {
            if(viewRect.bottomRight().x() > originalRect.bottomRight().x())
                delta.setX(originalRect.bottomRight().x() - viewRect.bottomRight().x());
        }
        if(viewRect.topLeft().y() < originalRect.topLeft().y())
        {
            delta.setY(originalRect.topLeft().y() - viewRect.topLeft().y());
        }
        else
        {
            if(viewRect.bottomRight().y() > originalRect.bottomRight().y())
                delta.setY(originalRect.bottomRight().y() - viewRect.bottomRight().y());
        }
        viewRect.translate(delta);
        m_centerOnPoint = viewRect.center();
    }
    m_scene.addItem(new GraphicsFood(cursorPoint));
    QGraphicsTextItem *textItem = new QGraphicsTextItem(QString::number(cursorPoint.x()) + QString(":") + QString::number(cursorPoint.y()));
    textItem->setPos(cursorPoint);
    m_scene.addItem(textItem);
}

void View::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::RightButton)
    {
        m_originX = event->x();
        m_originY = event->y();
        QGuiApplication::setOverrideCursor(QCursor(Qt::PointingHandCursor));
    }
}

void View::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::RightButton)
    {
        QGuiApplication::restoreOverrideCursor();
        m_centerOnPoint = mapToScene(viewport()->rect()).boundingRect().center();
    }

}

void View::mouseMoveEvent(QMouseEvent* event)
{
    if (event->buttons() & Qt::RightButton)
    {
//        qDebug() << QString("RightButton");
        QPointF oldP = mapToScene(m_originX, m_originY);
//        qDebug() << QString("oldP.x = ") << oldP.x() << QString("; oldP.y = ") << oldP.y();
        QPointF newP = mapToScene(event->pos());
//        qDebug() << QString("newP.x = ") << newP.x() << QString("; newP.y = ") << newP.y();
        QPointF translation = newP - oldP;
//        qDebug() << QString("dx = ") << translation.x() << QString("; dy = ") << translation.y();
        translate(translation.x(), translation.y());
        m_originX = event->x();
        m_originY = event->y();
    }
}
