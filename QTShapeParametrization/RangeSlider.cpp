
#include "RangeSlider.h"

#include <Utils.h>

namespace
{

const int scHandleSideLength = 13;
const int scSliderBarHeight = 5;
const int scMargin = 1;

}


RangeSlider::RangeSlider(QWidget* aParent)
    : QWidget(aParent),
      mMinimum(0),
      mMaximum(100),
      mLowerValue(0),
      mUpperValue(100),
      mFirstHandlePressed(false),
      mSecondHandlePressed(false),
      mInterval(mMaximum - mMinimum),
      mBackgroudColorEnabled(QColor(0x1E, 0x90, 0xFF)),
      mBackgroudColorDisabled(Qt::darkGray),
      mBackgroudColor(mBackgroudColorEnabled)
{
    setMouseTracking(true);
}

void RangeSlider::paintEvent(QPaintEvent* aEvent)
{
    Q_UNUSED(aEvent);
    QPainter painter(this);

    // Background
    QRectF backgroundRect = QRectF(scMargin, (height() - scSliderBarHeight) / 2, width() - scMargin * 2, scSliderBarHeight);
    QPen pen(Qt::gray, 0.8);
    painter.setPen(pen);
    painter.setRenderHint(QPainter::Qt4CompatiblePainting);
    QBrush backgroundBrush(QColor(0xD0, 0xD0, 0xD0));
    painter.setBrush(backgroundBrush);
    painter.drawRoundedRect(backgroundRect, 1, 1);

    // First value handle rect
    pen.setColor(Qt::darkGray);
    pen.setWidth(0.5);
    painter.setPen(pen);
    painter.setRenderHint(QPainter::Antialiasing);
    QBrush handleBrush(QColor(0xFA, 0xFA, 0xFA));
    painter.setBrush(handleBrush);
    QRectF leftHandleRect = firstHandleRect();
    painter.drawRoundedRect(leftHandleRect, 2, 2);

    // Second value handle rect
    QRectF rightHandleRect = secondHandleRect();
    painter.drawRoundedRect(rightHandleRect, 2, 2);

    // Handles
    painter.setRenderHint(QPainter::Antialiasing, false);
    QRectF selectedRect(backgroundRect);
    selectedRect.setLeft(leftHandleRect.right() + 0.5);
    selectedRect.setRight(rightHandleRect.left() - 0.5);
    QBrush selectedBrush(mBackgroudColor);
    painter.setBrush(selectedBrush);
    painter.drawRect(selectedRect);
}

QRectF RangeSlider::firstHandleRect() const
{
    double percentage = (mLowerValue - mMinimum) * 1.0 / mInterval;
    return handleRect(percentage * validWidth() + scMargin);
}

QRectF RangeSlider::secondHandleRect() const
{
    double percentage = (mUpperValue - mMinimum) * 1.0 / mInterval;
    return handleRect(percentage * validWidth() + scMargin + scHandleSideLength);
}

QRectF RangeSlider::handleRect(int aValue) const
{
    return QRect(aValue, (height()-scHandleSideLength) / 2, scHandleSideLength, scHandleSideLength);
}

void RangeSlider::mousePressEvent(QMouseEvent* aEvent)
{
    if(aEvent->buttons() & Qt::LeftButton)
    {
        mLowerValueOLD = mLowerValue;
        mUpperValueOLD = mUpperValue;
        mSecondHandlePressed = secondHandleRect().contains(aEvent->pos());
        mFirstHandlePressed = !mSecondHandlePressed && firstHandleRect().contains(aEvent->pos());
        if(mFirstHandlePressed)
        {
            mDelta = aEvent->pos().x() - (firstHandleRect().x() + scHandleSideLength / 2);
        }
        else if(mSecondHandlePressed)
        {
            mDelta = aEvent->pos().x() - (secondHandleRect().x() + scHandleSideLength / 2);
        }
        if(aEvent->pos().y() >= 2
           && aEvent->pos().y() <= height()- 2)
        {
            double step = mInterval / 10; // < 1 ? 1 : mInterval / 10;
            if(aEvent->pos().x() < firstHandleRect().x())
            {
                setLowerValue(mLowerValue - step);
            }
            else if(aEvent->pos().x() > firstHandleRect().x() + scHandleSideLength
                    && aEvent->pos().x() < secondHandleRect().x())
            {
                if(aEvent->pos().x() - (firstHandleRect().x() + scHandleSideLength) <
                   (secondHandleRect().x() - (firstHandleRect().x() + scHandleSideLength)) / 2)
                {
                    if(mLowerValue + step < mUpperValue)
                    {
                        setLowerValue(mLowerValue + step);
                    }
                    else
                    {
                        setLowerValue(mUpperValue);
                    }
                }
                else
                {
                    if(mUpperValue - step > mLowerValue)
                    {
                        setUpperValue(mUpperValue - step);
                    }
                    else
                    {
                        setUpperValue(mLowerValue);
                    }
                }
            }
            else if(aEvent->pos().x() > secondHandleRect().x() + scHandleSideLength)
            {
                setUpperValue(mUpperValue + step);
            }
        }
    }
}

void RangeSlider::mouseMoveEvent(QMouseEvent* aEvent)
{
    if(aEvent->buttons() & Qt::LeftButton)
    {
        if(mFirstHandlePressed)
        {
            if(aEvent->pos().x() - mDelta + scHandleSideLength / 2 <= secondHandleRect().x())
            {
                setLowerValue((aEvent->pos().x() - mDelta - scMargin - scHandleSideLength / 2) * 1.0 / validWidth() * mInterval + mMinimum);
            }
            else
            {
                setLowerValue(mUpperValue);
            }
        }
        else if(mSecondHandlePressed)
        {
            if(firstHandleRect().x() + scHandleSideLength * 1.5 <= aEvent->pos().x() - mDelta)
            {
                setUpperValue((aEvent->pos().x() - mDelta - scMargin - scHandleSideLength / 2 - scHandleSideLength) * 1.0 / validWidth() * mInterval + mMinimum);
            }
            else
            {
                setUpperValue(mLowerValue);
            }
        }
    }
}

void RangeSlider::mouseReleaseEvent(QMouseEvent* aEvent)
{
    Q_UNUSED(aEvent);
    if (Utils::eq(mLowerValueOLD, mLowerValue))
        setLowerValue(mLowerValue, true);
    if (Utils::eq(mUpperValueOLD, mUpperValue))
        setUpperValue(mUpperValue, true);
    mFirstHandlePressed = false;
    mSecondHandlePressed = false;
}

void RangeSlider::changeEvent(QEvent* aEvent)
{
    if(aEvent->type() == QEvent::EnabledChange)
    {
        if(isEnabled())
        {
            mBackgroudColor = mBackgroudColorEnabled;
        }
        else
        {
            mBackgroudColor = mBackgroudColorDisabled;
        }
        update();
    }
}

QSize RangeSlider::minimumSizeHint() const
{
    return QSize(scHandleSideLength * 2 + scMargin * 2, scHandleSideLength);
}

double RangeSlider::GetMinimun() const
{
    return mMinimum;
}

void RangeSlider::SetMinimum(double aMinimum)
{
    setMinimum(aMinimum);
}

double RangeSlider::GetMaximun() const
{
    return mMaximum;
}

void RangeSlider::SetMaximum(double aMaximum)
{
    setMaximum(aMaximum);
}

double RangeSlider::GetLowerValue() const
{
    return mLowerValue;
}

void RangeSlider::SetLowerValue(double aLowerValue)
{
    setLowerValue(aLowerValue);
}

double RangeSlider::GetUpperValue() const
{
    return mUpperValue;
}

void RangeSlider::SetUpperValue(double aUpperValue)
{
    setUpperValue(aUpperValue);
}

void RangeSlider::setLowerValue(double aLowerValue, bool isFinished)
{
    if(aLowerValue > mMaximum)
    {
        aLowerValue = mMaximum;
    }

    if(aLowerValue < mMinimum)
    {
        aLowerValue = mMinimum;
    }

    mLowerValue = aLowerValue;
    emit lowerValueChanged(mLowerValue, isFinished);

    update();
}

void RangeSlider::setUpperValue(double aUpperValue, bool isFinished)
{
    if(aUpperValue > mMaximum)
    {
        aUpperValue = mMaximum;
    }

    if(aUpperValue < mMinimum)
    {
        aUpperValue = mMinimum;
    }

    mUpperValue = aUpperValue;
    emit upperValueChanged(mUpperValue, isFinished);

    update();
}

void RangeSlider::setMinimum(double aMinimum)
{
    if(aMinimum <= mMaximum)
    {
        mMinimum = aMinimum;
    }
    else
    {
        double oldMax = mMaximum;
        mMinimum = oldMax;
        mMaximum = aMinimum;
    }
    mInterval = mMaximum - mMinimum;
    update();

    if (mLowerValue < mMinimum)
        setLowerValue(mMinimum);
    if (mUpperValue > mMaximum)
        setUpperValue(mMaximum);

    emit rangeChanged(mMinimum, mMaximum);
}

void RangeSlider::setMaximum(double aMaximum)
{
    if(aMaximum >= mMinimum)
    {
        mMaximum = aMaximum;
    }
    else
    {
        double oldMin = mMinimum;
        mMaximum = oldMin;
        mMinimum = aMaximum;
    }
    mInterval = mMaximum - mMinimum;
    update();

    if (mLowerValue < mMinimum)
        setLowerValue(mMinimum);
    if (mUpperValue > mMaximum)
        setUpperValue(mMaximum);

    emit rangeChanged(mMinimum, mMaximum);
}

int RangeSlider::validWidth() const
{
    return width() - scMargin * 2 - scHandleSideLength * 2;
}

void RangeSlider::SetRange(double aMinimum, double mMaximum)
{
    setMinimum(aMinimum);
    setMaximum(mMaximum);
}

