#include "stdafx.h"
#include <cmath>
#include "stepped_animation.h"

namespace fv
{

stepped_animation::stepped_animation(QObject* parent) :
	QObject(parent),
	animation_(this, "animation_value"),
	running_(false),
	value_(0.),
	forward_(true)
{
	animation_.setDuration(200);
	animation_.setEasingCurve(QEasingCurve::Linear);	
}

stepped_animation::~stepped_animation()
{
}

void stepped_animation::start(bool forward)
{
	Q_ASSERT(!running_);
	running_ = true;
	forward_ = forward;
	value_ = 0.;
	animation_.setLoopCount(-1);
	animation_.setStartValue(value_);
	animation_.setEndValue(forward ? 1 : -1);
	animation_.start();
}

void stepped_animation::stop()
{
	running_ = false;
	// Do not really stop here, wait until cycle finishes
}

void stepped_animation::interrupt()
{
	running_ = false;
	set_animation_value(0.);
	animation_.stop();
}

bool stepped_animation::stopped() const
{
	return animation_.state() == QAbstractAnimation::Stopped;
}

bool stepped_animation::forward() const
{
	Q_ASSERT(!stopped());
	return forward_;
}

qreal stepped_animation::animation_value() const
{
	return value_;
}

void stepped_animation::set_animation_value(qreal value)
{
	if (std::abs(value) < std::abs(value_))
	{
		// Next loop started
		// We don't use currentLoopChanged animation's signal because it generates this signal only
		// after first value of the next cycle already emitted
				
		emit step(forward_);
		if (!running_)
		{
			animation_.stop();
			return;
		}
	}

	value_ = value;
	emit value_changed(value_);
}

}