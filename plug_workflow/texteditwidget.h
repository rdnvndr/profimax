#ifndef TEXTEDITWIDGET_H
#define TEXTEDITWIDGET_H

#include <QtPrintSupport/QPrinter>

#include "ui_texteditwidget.h"

class TextEditWidget : public QWidget, public Ui::TextEditWidget{
		Q_OBJECT
	public:
                TextEditWidget(QWidget* pwgt = 0);
		void setReadOnly(bool b);
		void setHtml(const QString& text);
		void print(QPrinter*) const;
		void previewPrint();
		void printPdf();

	private slots:
		void textBold();
		void textUnderline();
		void textItalic();
		void textFamily(const QString &f);
		void textSize(const QString &p);
		void textStyle(int styleIndex);
		void textAlign(QAbstractButton *a);

		void currentCharFormatChanged(const QTextCharFormat &format);
		void cursorPositionChanged();
		void printPreview(QPrinter *printer);

	private:
		void mergeFormatOnWordOrSelection(const QTextCharFormat &format);
		void fontChanged(const QFont &f);
		void alignmentChanged(Qt::Alignment a);

};

#endif // TEXTEDITWIDGET_H
