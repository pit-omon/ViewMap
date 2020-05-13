#ifndef VIEW_H
#define VIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include "scaler.h"

class View : public QGraphicsView {
  Q_OBJECT
public:
  explicit View(QSize frameSize, QWidget *parent = 0);
protected slots:
  void wheelEvent (QWheelEvent *event);
  void resizeEvent(QResizeEvent *event);
  void mousePressEvent(QMouseEvent* event);
  void mouseReleaseEvent(QMouseEvent* event);
  void mouseMoveEvent(QMouseEvent* event);
private:
  enum scaleLvl{MIN = 0, MAX = 8};
  QGraphicsScene m_scene;
  QPoint m_frameSize;
  const QPoint m_OriginalSize;
  int m_originX;
  int m_originY;
  int m_scaleLevel;
  bool m_IsNeedToRestore;
  QPointF m_centerOnPoint;
  Scaler m_scaler;
  void correctView(const QPointF& cursorPoint, qreal scaleNum);
};

#endif // VIEW_H
