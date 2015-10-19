#include <QWidget>

#include "flowlayout.h"


FlowLayout::FlowLayout(QWidget *parent, int margin, int hSpacing, int vSpacing)
	: QLayout(parent), m_hSpace(hSpacing), m_vSpace(vSpacing)
{
	setMargin(margin);
	refreshmar=true;
}

FlowLayout::FlowLayout(int margin, int hSpacing, int vSpacing)
	: m_hSpace(hSpacing), m_vSpace(vSpacing)
{
	setMargin(margin);
}

FlowLayout::~FlowLayout()
{
	QLayoutItem *item;
	while ((item = takeAt(0)))
		delete item;
}

void FlowLayout::addItem(QLayoutItem *item)
{
	itemList.append(item);
}

int FlowLayout::horizontalSpacing() const
{
	if (m_hSpace >= 0) {
		return m_hSpace;
	} else {
		return smartSpacing(QStyle::PM_LayoutHorizontalSpacing);
	}
}

int FlowLayout::verticalSpacing() const
{
	if (m_vSpace >= 0) {
		return m_vSpace;
	} else {
		return smartSpacing(QStyle::PM_LayoutVerticalSpacing);
	}
}

int FlowLayout::count() const
{
	return itemList.size();
}

QLayoutItem *FlowLayout::itemAt(int index) const
{
	return itemList.value(index);
}

QLayoutItem *FlowLayout::takeAt(int index)
{
	if (index >= 0 && index < itemList.size())
		return itemList.takeAt(index);
	else
		return 0;
}

Qt::Orientations FlowLayout::expandingDirections() const
{
	return 0;
}

bool FlowLayout::hasHeightForWidth() const
{
	return true;
}

int FlowLayout::heightForWidth(int width) const
{
	int height = doLayout(QRect(0, 0, width, 0), true);
	return height;
}

void FlowLayout::setGeometry(const QRect &rect)
{
	QLayout::setGeometry(rect);
	doLayout(rect, false);
}

QSize FlowLayout::sizeHint() const
{
	return minimumSize();
}

QSize FlowLayout::minimumSize() const
{
	QSize size;
	QLayoutItem *item;
	foreach (item, itemList)
		size = size.expandedTo(item->minimumSize());

	size += QSize(2*margin(), 2*margin());
	return size;
}

int FlowLayout::doLayout(const QRect &rect, bool testOnly) const
{
	int left, top, right, bottom;
	getContentsMargins(&left, &top, &right, &bottom);

	QLayoutItem *item;
	int lsize = 0;
	foreach (item, itemList) {
		QWidget *wid = item->widget();
		if ((rect.width()-left-right)<lsize+wid->geometry().width())
			break;
		lsize=lsize+wid->geometry().width();
	}

	if (rect.width()>lsize)
		lsize = (rect.width()-lsize-left-right)/2;
	else
		lsize=0;
	QRect effectiveRect = rect.adjusted(+left+lsize, +top, -right, -bottom);
	int x = effectiveRect.x();
	int y = effectiveRect.y();
	int lineHeight = 0;

	foreach (item, itemList) {
		QWidget *wid = item->widget();
		int spaceX = horizontalSpacing();
		if (spaceX == -1)
			spaceX = wid->style()->layoutSpacing(
				QSizePolicy::PushButton, QSizePolicy::PushButton, Qt::Horizontal);
		int spaceY = verticalSpacing();
		if (spaceY == -1)
			spaceY = wid->style()->layoutSpacing(
				QSizePolicy::PushButton, QSizePolicy::PushButton, Qt::Vertical);
		int nextX = x + item->sizeHint().width() + spaceX;
		if (nextX - spaceX > effectiveRect.right() && lineHeight > 0) {
			x = effectiveRect.x();
			y = y + lineHeight + spaceY;
			nextX = x + item->sizeHint().width() + spaceX;
			lineHeight = 0;
		}

		if (!testOnly)
			item->setGeometry(QRect(QPoint(x, y), item->sizeHint()));

		x = nextX;
		lineHeight = qMax(lineHeight, item->sizeHint().height());
	}
	return y + lineHeight - rect.y() + bottom;

}

int FlowLayout::smartSpacing(QStyle::PixelMetric pm) const
{
	QObject *parent = this->parent();
	if (!parent) {
		return -1;
	} else if (parent->isWidgetType()) {
		QWidget *pw = static_cast<QWidget *>(parent);
		return pw->style()->pixelMetric(pm, 0, pw);
	} else {
		return static_cast<QLayout *>(parent)->spacing();
	}
}

void FlowLayout::addWidget(QWidget *item)
{
	item->hide();
	QLayout::addWidget(item);
	item->show();
	if (refreshmar){
		this->setMargin(this->margin()+1);
		refreshmar = false;
	}else{
		this->setMargin(this->margin()-1);
		refreshmar = true;
	}

}
