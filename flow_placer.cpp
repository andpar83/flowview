#include "stdafx.h"
#include <cmath>
#include <limits>
#include "pixmap_widget.h"
#include "flow_placer.h"

namespace fv
{

flow_placer::flow_placer(const QList<pixmap_widget*>& icons) : placer(icons)
{
}

flow_placer::~flow_placer()
{
}

int flow_placer::shift_index() const
{
	int result_index = -1;
	qreal zvalue = std::numeric_limits<qreal>::max();
	for (int i = 0; i < icons().count(); ++i)
	{
		if (icons()[i]->zValue() < zvalue)
		{
			result_index = i;
			zvalue = icons()[i]->zValue();
		}
	}
	Q_ASSERT(result_index != -1);
	return (icons().count() - result_index) % icons().count();
}

QSizeF flow_placer::calculate_icon_size() const
{
	auto sz = 2 * std::min(placement_rect().width(), placement_rect().height()) / 3;
	return QSizeF(sz, sz);
}

qreal flow_placer::get_zvalue(int icon_num, qreal shift_value) const
{
	return get_koef(icon_num, shift_value);
}

QPointF flow_placer::get_pos(int icon_num, qreal shift_value) const
{
	auto a = get_icon_angle(icon_num, shift_value);
	auto x = std::min(placement_rect().height(), placement_rect().width()) * cos(a);
	auto y = std::min(placement_rect().height(), placement_rect().width()) * sin(a);
	return QPointF(x, y);
}

qreal flow_placer::get_scale(int icon_num, qreal shift_value) const
{
	auto k = get_koef(icon_num, shift_value);
	return 0.5 * k + 0.5;
}

qreal flow_placer::get_opacity(int icon_num, qreal shift_value) const
{
	auto k = get_koef(icon_num, shift_value);
	if (k <= 0.2)
		return 5. * k;
	else if (k >= 0.8)
		return 5. * (1. - k);
	return 1.;
}

qreal flow_placer::get_rotate_y(int icon_num, qreal shift_value) const
{
	return -20.;
}

qreal flow_placer::get_koef(int icon_num, qreal shift_value) const
{
	qreal k = (icon_num + shift_value) / icons().count();
	if (k < 0)
		k += 1.;	
	else if (k > qreal(icons().count() - 1) / icons().count())
		k -= 1.;
	return k;
}

qreal flow_placer::get_icon_angle(int icon_num, qreal shift_value) const
{
	static const int start_angle = -100;
	static const int span_angle = 30;
	auto k = get_koef(icon_num, shift_value);
	return (start_angle + k * span_angle) * 4. * std::acos(0.) / 360.;
}

}