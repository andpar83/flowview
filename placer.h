#pragma once
#include <QRect>
#include <QSize>
#include <QPoint>
#include <QList>

namespace fv
{

class pixmap_widget;

//! Base abstract class for placing list of widgets on the scene with some support for rotating elements on the scene
class placer
{
public:
	explicit placer(const QList<pixmap_widget*>& icons);
	virtual ~placer();

	//! Set bounding rectange where widgets should be placed
	void set_placement_rect(const QRect& rect);

	//! Place elements on the scene with current shift-value	
	void place();

	//! Place elements on the scene with specified shift-value
	void place(qreal shift_value);

	//! Move elements to next or previous position
	void step(bool forward);

	//! Set index for the first element in the list of elements
	void set_shift_index(int index);

	//! Get current index for the first element
	virtual int shift_index() const = 0;

	//! Get size of widget from list
	const QSizeF& icon_size() const { return icon_size_; }

	//! Get z-value for widget with specified index
	qreal zvalue(int index) const;

	//! Get position for widget with specified index
	QPointF position(int index) const;

	//! Get scale for widget with specified index
	qreal scale(int index) const;

	//! Get opacity for widget with specified index
	qreal opacity(int index) const;

	//! Get rotation against y-axis for widget with specified index
	qreal rotate_y(int index) const;

protected:
	int index() const { return index_; }
	int current_index(int i) const;
	const QRect& placement_rect() const { return rect_; }
	const QList<pixmap_widget*>& icons() const { return icons_; }

private:
	virtual QSizeF calculate_icon_size() const = 0;	
	virtual qreal get_zvalue(int icon_num, qreal shift_value) const = 0;
	virtual QPointF get_pos(int icon_num, qreal shift_value) const = 0;
	virtual qreal get_scale(int icon_num, qreal shift_value) const = 0;
	virtual qreal get_opacity(int icon_num, qreal shift_value) const = 0;
	virtual qreal get_rotate_y(int icon_num, qreal shift_value) const = 0;

private:
	void init_ajustment();

private:
	QRect							rect_;
	QSizeF							icon_size_;
	const QList<pixmap_widget*>&	icons_;
	int								index_;

	/**
	 *	Shift-value is value from -1. to 1. which points to position between current position of
	 *	the element and previous (next one)
	 */
	qreal							shift_value_;
	qreal							dx_;
	qreal							dy_;
};

}