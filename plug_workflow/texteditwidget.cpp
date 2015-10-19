#include "texteditwidget.h"
#include <QtGui>
#include <QtCore>
#include <QtPrintSupport/QPrintPreviewDialog>
#include <QFileDialog>

TextEditWidget::TextEditWidget(QWidget* pwgt){
	setupUi(this);


	connect(textEdit, SIGNAL(currentCharFormatChanged(const QTextCharFormat &)),
			this, SLOT(currentCharFormatChanged(const QTextCharFormat &)));
	connect(textEdit, SIGNAL(cursorPositionChanged()),
			this, SLOT(cursorPositionChanged()));

	fontChanged(textEdit->font());
	alignmentChanged(textEdit->alignment());

	connect(toolButtonBold , SIGNAL(clicked()), this, SLOT(textBold()));
	connect(toolButtonItalic, SIGNAL(clicked()), this, SLOT(textItalic()));
	connect(toolButtonUnderline, SIGNAL(clicked()), this, SLOT(textUnderline()));

	connect(buttonGroupAlign , SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(textAlign(QAbstractButton *)));

	connect(comboFont, SIGNAL(activated(const QString &)),
			this, SLOT(textFamily(const QString &)));

	QFontDatabase db;
	foreach(int size, db.standardSizes())
		comboSize->addItem(QString::number(size));

	connect(comboSize, SIGNAL(activated(const QString &)),
			this, SLOT(textSize(const QString &)));

	comboSize->setCurrentIndex(comboSize->findText(QString::number(QApplication::font()													   .pointSize())));

	connect(comboStyle, SIGNAL(activated(int)),
			this, SLOT(textStyle(int)));
}

void TextEditWidget::setReadOnly(bool b){
	textEdit->setReadOnly(b);
	comboStyle->setDisabled(b);
	comboFont->setDisabled(b);
	comboSize->setDisabled(b);
	toolButtonBold->setDisabled(b);
	toolButtonItalic->setDisabled(b);
	toolButtonUnderline->setDisabled(b);
	toolButtonAlignCenter->setDisabled(b);
	toolButtonAlignLeft->setDisabled(b);
	toolButtonAlignRight->setDisabled(b);
	toolButtonAlignJustify->setDisabled(b);

}

void TextEditWidget::setHtml(const QString& text)
{
	textEdit->setHtml(text);
}

void TextEditWidget::textBold()
{
	QTextCharFormat fmt;
	fmt.setFontWeight(toolButtonBold->isChecked() ? QFont::Bold : QFont::Normal);
	mergeFormatOnWordOrSelection(fmt);
}

void TextEditWidget::textUnderline()
{
	QTextCharFormat fmt;
	fmt.setFontUnderline(toolButtonUnderline->isChecked());
	mergeFormatOnWordOrSelection(fmt);
}

void TextEditWidget::textItalic()
{
	QTextCharFormat fmt;
	fmt.setFontItalic(toolButtonItalic->isChecked());
	mergeFormatOnWordOrSelection(fmt);
}

void TextEditWidget::textFamily(const QString &f)
{
	QTextCharFormat fmt;
	fmt.setFontFamily(f);
	mergeFormatOnWordOrSelection(fmt);
}

void TextEditWidget::textSize(const QString &p)
{
	qreal pointSize = p.toFloat();
	if (p.toFloat() > 0) {
		QTextCharFormat fmt;
		fmt.setFontPointSize(pointSize);
		mergeFormatOnWordOrSelection(fmt);
	}
}

void TextEditWidget::textStyle(int styleIndex)
{
	QTextCursor cursor = textEdit->textCursor();

	if (styleIndex != 0) {
		QTextListFormat::Style style = QTextListFormat::ListDisc;

		switch (styleIndex) {
			default:
			case 1:
				style = QTextListFormat::ListDisc;
				break;
			case 2:
				style = QTextListFormat::ListCircle;
				break;
			case 3:
				style = QTextListFormat::ListSquare;
				break;
			case 4:
				style = QTextListFormat::ListDecimal;
				break;
			case 5:
				style = QTextListFormat::ListLowerAlpha;
				break;
			case 6:
				style = QTextListFormat::ListUpperAlpha;
				break;
		}

		cursor.beginEditBlock();

		QTextBlockFormat blockFmt = cursor.blockFormat();

		QTextListFormat listFmt;

		if (cursor.currentList()) {
			listFmt = cursor.currentList()->format();
		} else {
			listFmt.setIndent(blockFmt.indent() + 1);
			blockFmt.setIndent(0);
			cursor.setBlockFormat(blockFmt);
		}

		listFmt.setStyle(style);

		cursor.createList(listFmt);

		cursor.endEditBlock();
	} else {
		// ####
		QTextBlockFormat bfmt;
		bfmt.setObjectIndex(-1);
		cursor.mergeBlockFormat(bfmt);
	}
}
void TextEditWidget::textAlign(QAbstractButton *a)
{
	if (a == toolButtonAlignLeft)
		textEdit->setAlignment(Qt::AlignLeft | Qt::AlignAbsolute);
	else if (a == toolButtonAlignCenter)
		textEdit->setAlignment(Qt::AlignHCenter);
	else if (a == toolButtonAlignRight)
		textEdit->setAlignment(Qt::AlignRight | Qt::AlignAbsolute);
	else if (a == toolButtonAlignJustify)
		textEdit->setAlignment(Qt::AlignJustify);
}

void TextEditWidget::currentCharFormatChanged(const QTextCharFormat &format)
{
	fontChanged(format.font());
}

void TextEditWidget::cursorPositionChanged()
{
	alignmentChanged(textEdit->alignment());
}


void TextEditWidget::mergeFormatOnWordOrSelection(const QTextCharFormat &format)
{
	QTextCursor cursor = textEdit->textCursor();
	if (!cursor.hasSelection())
		cursor.select(QTextCursor::WordUnderCursor);
	cursor.mergeCharFormat(format);
	textEdit->mergeCurrentCharFormat(format);
}

void TextEditWidget::fontChanged(const QFont &f)
{
	comboFont->setCurrentIndex(comboFont->findText(QFontInfo(f).family()));
	comboSize->setCurrentIndex(comboSize->findText(QString::number(f.pointSize())));
	toolButtonBold->setChecked(f.bold());
	toolButtonItalic->setChecked(f.italic());
	toolButtonUnderline->setChecked(f.underline());
}


void TextEditWidget::alignmentChanged(Qt::Alignment a)
{
	if (a & Qt::AlignLeft) {
		toolButtonAlignLeft->setChecked(true);
	} else if (a & Qt::AlignHCenter) {
		toolButtonAlignCenter->setChecked(true);
	} else if (a & Qt::AlignRight) {
		toolButtonAlignRight->setChecked(true);
	} else if (a & Qt::AlignJustify) {
		toolButtonAlignJustify->setChecked(true);
	}
}

void TextEditWidget::print(QPrinter* printer) const {
    textEdit->print(printer);
}

void TextEditWidget::previewPrint()
{
#ifndef QT_NO_PRINTER
	QPrinter printer(QPrinter::HighResolution);
	QPrintPreviewDialog preview(&printer, this);

	if (!printer.isValid())
	  printer.setOutputFormat(QPrinter::PdfFormat);

	connect(&preview, SIGNAL(paintRequested(QPrinter *)), SLOT(printPreview(QPrinter *)));
	preview.exec();
#endif
}

void TextEditWidget::printPreview(QPrinter *printer)
{
#ifdef QT_NO_PRINTER
	Q_UNUSED(printer);
#else
	textEdit->print(printer);
#endif
}

void TextEditWidget::printPdf(){
#ifndef QT_NO_PRINTER

    QString fileName = QFileDialog::getSaveFileName(this, "Export PDF",
												    QString(), "*.pdf");
	if (!fileName.isEmpty()) {
		if (QFileInfo(fileName).suffix().isEmpty())
			fileName.append(".pdf");
		QPrinter printer(QPrinter::HighResolution);
		printer.setOutputFormat(QPrinter::PdfFormat);
		printer.setOutputFileName(fileName);
		textEdit->print(&printer);
	}
#endif
}
