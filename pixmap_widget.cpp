#include "stdafx.h"
#include "pixmap_widget.h"

namespace fv
{

pixmap_widget::pixmap_widget(QPixmap pixmap, QGraphicsItem* parent) :
	QGraphicsWidget(parent),
	pixmap_(pixmap),
	rotation_y_(0.)
{
}

pixmap_widget::~pixmap_widget()
{
}

void pixmap_widget::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	painter->drawPixmap(0, 0, size().width(), size().height(), pixmap_);	
}

qreal pixmap_widget::rotation_y() const
{
	return rotation_y_;
}

void pixmap_widget::set_rotation_y(qreal value)
{
	rotation_y_ = value;
	setTransform(QTransform().rotate(value, Qt::YAxis));
}

}