#pragma once

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>

class RangeSlider : public QWidget
{
    Q_OBJECT

public:
    RangeSlider(QWidget* aParent = Q_NULLPTR);

    QSize minimumSizeHint() const override;

    double GetMinimun() const;
    void SetMinimum(double aMinimum);

    double GetMaximun() const;
    void SetMaximum(double aMaximum);

    double GetLowerValue() const;
    void SetLowerValue(double aLowerValue);

    double GetUpperValue() const;
    void SetUpperValue(double aUpperValue);

    void SetRange(double aMinimum, double aMaximum);

protected:
    void paintEvent(QPaintEvent* aEvent) override;
    void mousePressEvent(QMouseEvent* aEvent) override;
    void mouseMoveEvent(QMouseEvent* aEvent) override;
    void mouseReleaseEvent(QMouseEvent* aEvent) override;
    void changeEvent(QEvent* aEvent) override;

    QRectF firstHandleRect() const;
    QRectF secondHandleRect() const;
    QRectF handleRect(int aValue) const;

signals:
    void lowerValueChanged(double aLowerValue);
    void upperValueChanged(double aUpperValue);
    void rangeChanged(double aMin, double aMax);

public slots:
    void setLowerValue(double aLowerValue);
    void setUpperValue(double aUpperValue);
    void setMinimum(double aMinimum);
    void setMaximum(double aMaximum);

private:
    Q_DISABLE_COPY(RangeSlider)
    float currentPercentage();
    int validWidth() const;

    double mMinimum;
    double mMaximum;
    double mLowerValue;
    double mUpperValue;
    bool mFirstHandlePressed;
    bool mSecondHandlePressed;
    double mInterval;
    double mDelta;
    QColor mBackgroudColorEnabled;
    QColor mBackgroudColorDisabled;
    QColor mBackgroudColor;
};

