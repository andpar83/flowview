#pragma once
#include <QGraphicsWidget>
#include <QPixmap>

namespace fv
{

//! Class for painting pixmap on the scene
class pixmap_widget : public QGraphicsWidget
{
	Q_OBJECT
	Q_PROPERTY(qreal zvalue READ zValue WRITE setZValue)
	Q_PROPERTY(qreal rotation_y READ rotation_y WRITE set_rotation_y)

public:
	pixmap_widget(QPixmap pixmap, QGraphicsItem* parent = nullptr);
	virtual ~pixmap_widget();

	//! Get rotation against y-axis for pixmap
	qreal rotation_y() const;

	//! Set rotation against y-axis for pixmap
	void set_rotation_y(qreal value);

protected:
	virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

private:
	QPixmap	pixmap_;
	qreal	rotation_y_;
};

}