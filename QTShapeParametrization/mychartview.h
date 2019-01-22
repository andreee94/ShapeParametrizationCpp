
#ifndef CHARTVIEW_H
#define CHARTVIEW_H

#include <QtCharts/QChartView>
#include <QtWidgets/QRubberBand>

#include <QtWidgets>
#include <QtCharts>
#include <QLayoutItem>
#include <QGraphicsSimpleTextItem>

QT_CHARTS_USE_NAMESPACE

class TooltipTextFunction {
public:
      TooltipTextFunction() {}
      virtual ~TooltipTextFunction() {}
      virtual QString operator() (QLabel *popup, const QPointF &point_px, const QPointF &point_xy, const QPointF &cursor_px, const QPointF &cursor_xy, int index) = 0;
};

class PrefixIndexTooltipText : public TooltipTextFunction {
    public:
        PrefixIndexTooltipText(QString prefix, int offset)
        {
            this->prefix = prefix;
            this->offset = offset;
        }

      virtual QString operator() (QLabel *popup, const QPointF &point_px, const QPointF &point_xy, const QPointF &cursor_px, const QPointF &cursor_xy, int index)
      {
          QString text;
          text.append(prefix);
          text.append(index + offset);
          return text;
      }

      QString prefix;
      int offset = 0;
};

class XYTooltipText : public TooltipTextFunction {
    public:
        XYTooltipText(bool popupShowX, bool popupShowY, int popupPrecision)
        {
            this->popupShowX = popupShowX;
            this->popupShowY = popupShowY;
            this->popupPrecision = popupPrecision;
        }
        XYTooltipText(int popupPrecision)
        {
            this->popupShowX = true;
            this->popupShowY = true;
            this->popupPrecision = popupPrecision;
        }

      virtual QString operator() (QLabel *popup, const QPointF &point_px, const QPointF &point_xy, const QPointF &cursor_px, const QPointF &cursor_xy, int index)
      {
          QString text;
          text.append(popupShowX ? QString::number(point_xy.x(), 'g', popupPrecision) : "");
          text.append(popupShowX && popupShowY ? ", " : "");
          text.append(popupShowY ? QString::number(point_xy.y(), 'g', popupPrecision) : "");
          return text;
      }

      bool popupShowX, popupShowY;
      int popupPrecision;
};

class XTooltipText : public XYTooltipText {
    public:
      XTooltipText(int popupPrecision)
          :XYTooltipText(true, false, popupPrecision) { }
};

class YTooltipText : public XYTooltipText {
    public:
      YTooltipText(int popupPrecision)
          :XYTooltipText(false, true, popupPrecision) { }
};

//class QChartViewPrivate;
//QT_FORWARD_DECLARE_CLASS(QChartViewPrivate)
class ChartView : public QChartView
{
    Q_OBJECT

public:

    ChartView(QWidget *parent = 0);
    ~ChartView();

    //using QChartView::QChartView;
    enum DirectionZoom{
        NotZoom,
        VerticalZoom,
        HorizontalZoom,
        BothDirectionZoom = VerticalZoom | HorizontalZoom
    };
    DirectionZoom directionZoom() const{
        return mDirectionZoom;
    }
    void setDirectionZoom(const DirectionZoom &directionZoom){
        mDirectionZoom = directionZoom;
    }

    bool canZoom() const;
    void setCanZoom(bool canZoom);

    bool canScroll() const;
    void setCanScroll(bool canScroll);

    bool canUseKeys() const;
    void setCanUseKeys(bool canUseKeys);

    bool showTooltip() const;
    void setShowTooltip(bool showTooltip);

    QLabel *getPopup() const;
    void setPopup(QLabel *value);

    int getPopupPrecision() const;
    void setPopupPrecision(int value);

    int getPopupDistanceThresholdPX() const;
    void setPopupDistanceThresholdPX(int value);

    int getClickableDistanceThresholdPX() const;
    void setClickableDistanceThresholdPX(int value);

    QRubberBand *getRubberBand() const;
    void setRubberBand(QRubberBand *value);

    void addTooltip(QScatterSeries*, TooltipTextFunction*);
    void addToggleable(QScatterSeries*);
    void addToggleable(QScatterSeries*, QScatterSeries*);


signals:
    void clickableEvent(QScatterSeries* series1, QScatterSeries* series2, QPointF point, bool from1to2);

protected:
    bool viewportEvent(QEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);

    void manageTooltip(QMouseEvent * event);
    void manageClicked(QMouseEvent * event);


private:
    bool mCanZoom = true;
    bool mCanScroll = true;
    bool mCanUseKeys = true;
    bool mShowTooltip = false;

    bool m_isTouching;
    DirectionZoom mDirectionZoom = NotZoom;
    QPointF m_lastMousePos;

    // tooltip or popup variables
    QLabel *popup;
    QGraphicsProxyWidget *proxy;
    int popupPrecision = 3;
    int popupDistanceThresholdPX = 12;
    std::map<QScatterSeries*, TooltipTextFunction*> popup_map;

    // toggle between two series clicking a point
    int clickableDistanceThresholdPX = 12;
    std::map<QScatterSeries*, QScatterSeries*> clickable_map;

    // prevent rubberband when rectangle is too small
    QPoint startingDragPoint;
    QRubberBand *rubberBand;

};

#endif
