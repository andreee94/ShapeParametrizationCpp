
#ifndef CHARTVIEW_H
#define CHARTVIEW_H

#include <QtCharts/QChartView>
#include <QtWidgets/QRubberBand>

#include <QtWidgets>
#include <QtCharts>
#include <QLayoutItem>
#include <QGraphicsSimpleTextItem>

QT_CHARTS_USE_NAMESPACE

class ChartView : public QChartView
{
public:
    ChartView(QChart *chart, QWidget *parent = 0);

    using QChartView::QChartView;
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

    bool getPopupShowX() const;
    void setPopupShowX(bool value);

    bool getPopupShowY() const;
    void setPopupShowY(bool value);

    int getPopupPrecision() const;
    void setPopupPrecision(int value);

protected:
    bool viewportEvent(QEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);

    void manageTooltip(QMouseEvent * event);

private:
    bool mCanZoom = true;
    bool mCanScroll = true;
    bool mCanUseKeys = true;
    bool mShowTooltip = false;

    bool m_isTouching;
    DirectionZoom mDirectionZoom = NotZoom;
    QPointF m_lastMousePos;

    QLabel *popup;
    QGraphicsProxyWidget *proxy;
    int popupPrecision = 3;
    bool popupShowX = true;
    bool popupShowY = true;
};

#endif
