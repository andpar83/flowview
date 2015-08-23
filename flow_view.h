#pragma once
#include <memory>
#include <QList>
#include <QGraphicsView>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include "stepped_animation.h"

namespace fv
{

class pixmap_widget;
class placer;

//! Window for showing widgets on the scene
class flow_view : public QGraphicsView
{
	Q_OBJECT

public:
	flow_view(QWidget* parent = nullptr);
	virtual ~flow_view();

protected:
	virtual void resizeEvent(QResizeEvent* e);
	virtual void keyPressEvent(QKeyEvent* e);
	virtual void keyReleaseEvent(QKeyEvent* e);

private slots:
	void animation_value_changed(qreal value);
	void step(bool forward);

private:
	void load_scene();
	void place();
	void run_flow(bool forward);
	void run_mode_animation();
	bool process_ctrl(QKeyEvent* e);
	QPropertyAnimation* add_mode_animation(pixmap_widget* icon, const QByteArray& property_name);

private:
	QList<pixmap_widget*>	icons_;
	QParallelAnimationGroup	mode_animation_;
	stepped_animation		flow_animation_;

	std::unique_ptr<placer>	normal_placer_;
	std::unique_ptr<placer>	flow_placer_;
	placer*					placer_;
};

}