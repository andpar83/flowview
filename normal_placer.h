#pragma once
#include "placer.h"

namespace fv
{

//! Class for placing widgets in starting position
class normal_placer : public placer
{
public:
	explicit normal_placer(const QList<pixmap_widget*>& icons);
	virtual ~normal_placer();

	virtual int shift_index() const;

private:
	virtual QSizeF calculate_icon_size() const;
	virtual qreal get_zvalue(int icon_num, qreal shift_value) const;
	virtual QPointF get_pos(int icon_num, qreal shift_value) const;
	virtual qreal get_scale(int icon_num, qreal shift_value) const;
	virtual qreal get_opacity(int icon_num, qreal shift_value) const;
	virtual qreal get_rotate_y(int icon_num, qreal shift_value) const;
};

}