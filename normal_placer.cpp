#include "stdafx.h"
#include "normal_placer.h"

namespace fv
{

normal_placer::normal_placer(const QList<pixmap_widget*>& icons) : placer(icons)
{
}

normal_placer::~normal_placer()
{
}

int normal_placer::shift_index() const
{
	return index();
}

QSizeF normal_placer::calculate_icon_size() const
{
	auto sz = placement_rect().width() / icons().count();
	return QSizeF(sz, sz);
}

qreal normal_placer::get_zvalue(int icon_num, qreal shift_value) const
{
	return icon_num;
}

QPointF normal_placer::get_pos(int icon_num, qreal shift_value) const
{
	return QPointF(icon_num * icon_size().width(), 0);
}

qreal normal_placer::get_scale(int icon_num, qreal shift_value) const
{
	return 1.;
}

qreal normal_placer::get_opacity(int icon_num, qreal shift_value) const
{
	return 1.;
}

qreal normal_placer::get_rotate_y(int icon_num, qreal shift_value) const
{
	return 0.;
}

}