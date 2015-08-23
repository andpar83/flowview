#pragma once
#include <QPropertyAnimation>

namespace fv
{

//! Wrapper for animation which allows to run infinite loop of changing values from 0. to 1. or from 0. to -1.
class stepped_animation : public QObject
{
	Q_OBJECT
	Q_PROPERTY(qreal animation_value READ animation_value WRITE set_animation_value)

public:
	stepped_animation(QObject* parent = nullptr);
	virtual ~stepped_animation();

	//! Run animation with specified direction
	void start(bool forward);

	//! Delayed stop until current cycle finishes
	void stop();

	//! Instant stop of animation
	void interrupt();

	//! Is animation stopped
	bool stopped() const;

	//! Get directino of running animation
	bool forward() const;

	//! Get current animation value
	qreal animation_value() const;

	//! Set current animation value
	void set_animation_value(qreal value);

signals:
	//! Signal which is emitted every time current animation value is changed
	void value_changed(qreal value);

	//! Signal which is emitted every time current cycle finishes
	void step(bool forward);

private:
	QPropertyAnimation	animation_;
	bool				running_;
	qreal				value_;
	bool				forward_;
};

}