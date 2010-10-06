#include "scaledimage.h"
#include <QPainter>


ScaledImage::ScaledImage(QWidget *parent)
	: QWidget(parent)
	, m_Size(0,0)
	, m_BGColor(Qt::transparent)  // normal background (transparent)
{
	m_Image.load("off.png");
}

void ScaledImage::UpdateImage( const QString& fileName )
{
	m_Image.load(fileName);
	update();
}

void ScaledImage::SetBgColor( const QColor &color )
{
	m_BGColor = color;
	update();
}

void ScaledImage::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.fillRect( event->rect(), QBrush(m_BGColor) );
	QPoint centerPoint(0,0);
	// Scale new image
	QImage scaledImage = m_Image.scaled(m_Size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	// Calculate image center position into screen
	centerPoint.setX((m_Size.width()-scaledImage.width())/2);
	centerPoint.setY((m_Size.height()-scaledImage.height())/2);
	// Draw image
	painter.drawImage(centerPoint,scaledImage);
}

void ScaledImage::resizeEvent (QResizeEvent* event)
{
	m_Size = event->size();
	// Call base class impl
	QWidget::resizeEvent(event);
}
