#include "stdafx.h"
#include <algorithm>
#include "pixmap_widget.h"
#include "normal_placer.h"
#include "flow_placer.h"
#include "flow_view.h"

namespace fv
{

flow_view::flow_view(QWidget* parent) : QGraphicsView(parent)
{
	setWindowTitle("Flow View");
	setMinimumSize(200, 100);
	resize(600, 400);
	setBackgroundBrush(Qt::black);
	setRenderHint(QPainter::Antialiasing, true);
	setFrameStyle(QFrame::NoFrame);
	setScene(new QGraphicsScene(this));

	load_scene();
	Q_ASSERT(!icons_.empty());
	normal_placer_.reset(new normal_placer(icons_));
	flow_placer_.reset(new flow_placer(icons_));
	placer_ = normal_placer_.get();
	placer_->place();

	auto cr = connect(&flow_animation_, SIGNAL(value_changed(qreal)), this, SLOT(animation_value_changed(qreal)));
	Q_ASSERT(cr && "Failed to connect signal");

	cr = connect(&flow_animation_, SIGNAL(step(bool)), this, SLOT(step(bool)));
	Q_ASSERT(cr && "Failed to connect signal");
}

flow_view::~flow_view()
{
}

void flow_view::load_scene()
{
	QStringList names;
	names << ":/images/desktop.png";
	names << ":/images/hub.png";
	names << ":/images/lan.png";
	names << ":/images/laptop.png";
	names << ":/images/object.png";
	names << ":/images/pc.png";
	names << ":/images/phone.png";
	names << ":/images/printer.png";
    names << ":/images/socket.png";

	for (int i = 0; i < names.count(); ++i)
	{
		auto icon = new pixmap_widget(QPixmap(names[i]));
		scene()->addItem(icon);
		icons_ << icon;
	}
}

void flow_view::step(bool forward)
{
	normal_placer_->step(forward);
	flow_placer_->step(forward);	
	scene()->update();
}

void flow_view::resizeEvent(QResizeEvent* e)
{
	QGraphicsView::resizeEvent(e);
	scene()->setSceneRect(0, 0, width(), height());
	normal_placer_->set_placement_rect(rect());
	flow_placer_->set_placement_rect(rect());
	placer_->place();
	scene()->update();
}

bool flow_view::process_ctrl(QKeyEvent* e)
{
	auto ctrl = e->modifiers().testFlag(Qt::ControlModifier);
	bool changed = false;
	if (ctrl && placer_ == normal_placer_.get())
	{
		changed = true;
		placer_ = flow_placer_.get();
	}
	else if (!ctrl && placer_ == flow_placer_.get())
	{
		changed = true;
		flow_animation_.interrupt();
		placer_ = normal_placer_.get();
	}
	return changed;
}

void flow_view::keyPressEvent(QKeyEvent* e)
{
	auto changed = process_ctrl(e);
	if (changed)
		run_mode_animation();

	if (placer_ == normal_placer_.get())
		return;

	switch (e->key())
	{
	case Qt::Key_Left:
		run_flow(false);
		break;
	case  Qt::Key_Right:
		run_flow(true);
		break;
	}
}

void flow_view::keyReleaseEvent(QKeyEvent* e)
{
	auto changed = process_ctrl(e);

	if (!flow_animation_.stopped())
	{
		if (placer_ == normal_placer_.get())
			flow_animation_.interrupt();
		else if (e->key() == Qt::Key_Left && !flow_animation_.forward() || e->key() == Qt::Key_Right && flow_animation_.forward())
			flow_animation_.stop();
	}

	if (changed)
		run_mode_animation();
}

void flow_view::animation_value_changed(qreal value)
{
	placer_->place(value);
	scene()->update();
}

void flow_view::place()
{
	placer_->place();
	scene()->update();
}

void flow_view::run_flow(bool forward)
{
	Q_ASSERT(placer_ == flow_placer_.get());
	if (mode_animation_.state() != QAbstractAnimation::Stopped)
		return;
	
	if (!flow_animation_.stopped() && forward != flow_animation_.forward())
		flow_animation_.interrupt();
	if (flow_animation_.stopped())
		flow_animation_.start(forward);
}

QPropertyAnimation* flow_view::add_mode_animation(pixmap_widget* icon, const QByteArray& property_name)
{
	static const int duration = 300;
	auto* animation = new QPropertyAnimation(icon, property_name);
	animation->setDuration(duration);
	mode_animation_.addAnimation(animation);
	return animation;
}

void flow_view::run_mode_animation()
{
	Q_ASSERT(flow_animation_.stopped());
	mode_animation_.clear();
	placer_->set_shift_index(placer_ == normal_placer_.get() ? flow_placer_->shift_index() : normal_placer_->shift_index());

	for (int i = 0; i < icons_.size(); ++i)
	{
		auto icon = icons_[i];
		add_mode_animation(icon, "geometry")->setEndValue(QRectF(placer_->position(i), placer_->icon_size()));
		add_mode_animation(icon, "opacity")->setEndValue(placer_->opacity(i));
		add_mode_animation(icon, "scale")->setEndValue(placer_->scale(i));
		add_mode_animation(icon, "zvalue")->setEndValue(placer_->zvalue(i));
		add_mode_animation(icon, "rotation_y")->setEndValue(placer_->rotate_y(i));
	}

	mode_animation_.start();
}

}