#include "stdafx.h"
#include <cmath>
#include <functional>
#include <algorithm>
#include "pixmap_widget.h"
#include "placer.h"

namespace fv
{

placer::placer(const QList<pixmap_widget*>& icons) :
	icons_(icons),
	index_(0),
	shift_value_(0.),
	dx_(0.),
	dy_(0.)
{
	Q_ASSERT(!icons_.empty());
}

placer::~placer()
{
}

void placer::set_placement_rect(const QRect& rect)
{
	Q_ASSERT(rect.isValid());
	rect_ = rect;
	icon_size_ = calculate_icon_size();
	init_ajustment();	
}

void placer::init_ajustment()
{
	auto min_x = std::numeric_limits<qreal>::max();
	auto max_x = -std::numeric_limits<qreal>::max();
	auto min_y = std::numeric_limits<qreal>::max();
	auto max_y = -std::numeric_limits<qreal>::max();

	for (int i = 0; i < icons_.size(); ++i)
	{
		auto pos = get_pos(current_index(i), 0.);
		min_x = std::min(min_x, pos.x());
		max_x = std::max(max_x, pos.x() + icon_size_.width());
		min_y = std::min(min_y, pos.y());
		max_y = std::max(max_y, pos.y() + icon_size_.height());
	}

	dx_ = (rect_.x() - min_x) + (rect_.width() - (max_x - min_x)) / 2;
	dy_ = (rect_.y() - min_y) + (rect_.height() - (max_y - min_y)) / 2;	
}

void placer::place()
{
	place(shift_value_);
}

void placer::place(qreal shift_value)
{
	Q_ASSERT(-1. <= shift_value && shift_value <= 1.);
	shift_value_ = shift_value;

	using std::placeholders::_1;
	std::for_each(icons_.begin(), icons_.end(), std::bind(&pixmap_widget::resize, _1, std::cref(icon_size_)));

	for (int i = 0; i < icons_.size(); ++i)
	{
		auto icon = icons_[i];
		auto index = current_index(i);
		
		icon->setZValue(get_zvalue(index, shift_value));
		icon->setPos(get_pos(index, shift_value) + QPointF(dx_, dy_));
		icon->setOpacity(get_opacity(index, shift_value));
		icon->setScale(get_scale(index, shift_value));
		icon->set_rotation_y(get_rotate_y(index, shift_value));
	}
}

int placer::current_index(int i) const
{
	Q_ASSERT(!icons_.empty());
	return (i + index_) % icons_.count();
}

void placer::step(bool forward)
{
	shift_value_ = 0.;
	Q_ASSERT(!icons_.empty());
	index_ = (index_ + (forward ? 1 : -1) + icons_.count()) % icons_.count();
}

void placer::set_shift_index(int index)
{
	shift_value_ = 0.;
	index_ = index;
}

qreal placer::zvalue(int index) const
{
	return get_zvalue(current_index(index), shift_value_);
}

QPointF placer::position(int index) const
{
	return get_pos(current_index(index), shift_value_) + QPointF(dx_, dy_);
}

qreal placer::opacity(int index) const
{
	return get_opacity(current_index(index), shift_value_);
}

qreal placer::scale(int index) const
{
	return get_scale(current_index(index), shift_value_);
}

qreal placer::rotate_y(int index) const
{
	return get_rotate_y(current_index(index), shift_value_);
}

}