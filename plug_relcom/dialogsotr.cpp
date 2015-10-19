/*
 *      mainwindow.cpp
 *
 *      Copyright 2009 Rodionov Andrey <andrey@roand.ru>
 *
 *
 */
#include <QMessageBox>
#include <QFileDialog>

#include "dialogsotr.h"
#include "treesqlotdel.h"
#include "tablesqldolgn.h"
#include "tablesqlobraz.h"
#include "profimaxdelegate.h"

DialogSotr::DialogSotr(QWidget* pwgt) : QDialog(pwgt)
{
	setupUi(this);
	mapper = new QDataWidgetMapper(this);

	connect(this->buttonBox, SIGNAL(accepted()), this, SLOT(apply()));
	connect(this->comboBoxMember, SIGNAL(currentIndexChanged(int)), this, SLOT(changeMember(int)));
	connect(this->comboBoxProfActiv, SIGNAL(currentIndexChanged(int)), this, SLOT(changeProfActiv(int)));
	connect(this->comboBoxSoc, SIGNAL(currentIndexChanged(int)), this, SLOT(changeSoc(int)));
	connect(this->toolButtonFoto, SIGNAL(clicked()), this, SLOT(onChooseFoto()));
}

void DialogSotr::changeProfActiv(int ci)
{
	//QMessageBox::critical(0,"",QString("%1").arg(ci));
	bool fl = false;
	if (ci != 0) {
		fl = true;
	}

	if (comboBoxElected->currentIndex() < 0)
		comboBoxElected->setCurrentIndex(0);

	comboBoxElected->setEnabled(fl);
}

void DialogSotr::changeSoc(int ci)
{
	//QMessageBox::critical(0,"",QString("%1").arg(ci));
	bool fl = true;
	if (ci != 0) {
		fl = false;
	}

	if (comboBoxSoc->currentIndex() > 0) {
		if (comboBoxDolgn->currentIndex() < 0)
			comboBoxDolgn->setCurrentIndex(0);
		if (comboBoxOtdel->currentIndex() < 0)
			comboBoxOtdel->setCurrentIndex(0);
		if (comboBoxCategory->currentIndex() < 0)
			comboBoxCategory->setCurrentIndex(0);
		if (comboBoxCond->currentIndex() < 0)
			comboBoxCond->setCurrentIndex(0);

		if (spinBoxTabNum->value() < 1)
			spinBoxTabNum->setValue(1);

	}

	comboBoxDolgn->setEnabled(fl);
	spinBoxTabNum->setEnabled(fl);
	comboBoxOtdel->setEnabled(fl);
	comboBoxCategory->setEnabled(fl);
	comboBoxCond->setEnabled(fl);
}

void DialogSotr::changeMember(int ci)
{
	//QMessageBox::critical(0,"",QString("%1").arg(ci));
	bool fl = true;
	if (ci != 0) {
		fl = false;
	}

	if (comboBoxMember->currentIndex() == 1) {
		lineEditOrhProfbilet->setText(QString(" "));
		comboBoxElected->setCurrentIndex(0);
		comboBoxProfActiv->setCurrentIndex(0);
		//comboBoxMember->setCurrentIndex(1);
	}

	lineEditOrhProfbilet->setEnabled(fl);
	spinBoxNumProfbilet->setEnabled(fl);
	//dateEditDateIn->setEnabled(fl);
	checkBoxProfbiletOut->setEnabled(fl);
	checkBoxAdmitted->setEnabled(fl);
	comboBoxElected->setEnabled(fl);
	comboBoxProfActiv->setEnabled(fl);
	checkBoxKadrSotr->setEnabled(fl);
	spinBoxStage->setEnabled(fl);
}

bool DialogSotr::checkInput(QWidget* widget, QString mes, int tab)
{

	if (qobject_cast<QCBoxButton*>(widget) != NULL) {
		if (qobject_cast<QCBoxButton*>(widget)->currentIndex() >= 0)
			return true;
		else {
			tabWidget->setCurrentIndex(tab);
			qobject_cast<QCBoxButton*>(widget)->comboBox->setFocus();
			//widget->setFocus();
			QToolTip::showText(widget->mapToGlobal(QPoint(0, 5)), mes);
			return false;
		}
	}
	if (qobject_cast<QComboBox*>(widget) != NULL) {
		if (qobject_cast<QComboBox*>(widget)->currentIndex() >= 0)
			return true;
		else {
			tabWidget->setCurrentIndex(tab);
			widget->setFocus();
			QToolTip::showText(widget->mapToGlobal(QPoint(0, 5)), mes);
			return false;
		}
	}
	else if (qobject_cast<QLineEdit*>(widget) != NULL) {
		if (qobject_cast<QLineEdit*>(widget)->text() != QString(""))
			return true;
		else {
			tabWidget->setCurrentIndex(tab);
			widget->setFocus();
			QToolTip::showText(widget->mapToGlobal(QPoint(0, 5)), mes);
			return false;
		}
	}
	else if (qobject_cast<QSpinBox*>(widget) != NULL) {
		if (qobject_cast<QSpinBox*>(widget)->value() != 0)
			return true;
		else {
			tabWidget->setCurrentIndex(tab);
			widget->setFocus();
			QToolTip::showText(widget->mapToGlobal(QPoint(0, 5)), mes);
			return false;
		}
	}
	else
		return true;


}

void DialogSotr::apply()
{

	if (checkInput(lineEditLastName, tr("Введите пожалуйста\nфамилию сотрудника"), 0) &&
		checkInput(lineEditFirstName, tr("Введите пожалуйста\nимя сотрудника"), 0) &&
		checkInput(lineEditMidName, tr("Введите пожалуйста\nотчество сотрудника"), 0) &&
		checkInput(comboBoxSex, tr("Введите пожалуйста\nдолжность сотрудника"), 0) &&
		checkInput(spinBoxPost, tr("Введите пожалуйста\nиндекс сотрудника"), 0) &&
		checkInput(lineEditCity, tr("Введите пожалуйста\nгород сотрудника"), 0) &&
		checkInput(lineEditStreet, tr("Введите пожалуйста\nулицу сотрудника"), 0) &&
		checkInput(spinBoxHouse, tr("Введите пожалуйста\nдом сотрудника"), 0) &&

		checkInput(spinBoxTabNum, tr("Введите пожалуйста\nтабельный сотрудника"), 1) &&
		checkInput(comboBoxDolgn, tr("Введите пожалуйста\nдолжность сотрудника"), 1) &&
		checkInput(comboBoxOtdel, tr("Введите пожалуйста\nподразделение сотрудника"), 1) &&

		checkInput(comboBoxEdu, tr("Введите пожалуйста\nобразование сотрудника"), 1) &&
		checkInput(comboBoxCategory, tr("Введите пожалуйста\nкатегорию сотрудника"), 1) &&
		checkInput(comboBoxSoc, tr("Введите пожалуйста\nусловия соц. положение сотрудника"), 1) &&
		checkInput(comboBoxCond, tr("Введите пожалуйста\nусловия труда сотрудника"), 1) &&

		checkInput(spinBoxPaspSer, tr("Введите пожалуйста\nсерию паспорта сотрудника"), 2) &&
		checkInput(spinBoxPaspNum, tr("Введите пожалуйста\nномер паспорта сотрудника"), 2) &&
		checkInput(lineEditPaspPlace, tr("Введите пожалуйста\nкем выдан паспорт сотруднику"), 2) &&
		checkInput(spinBoxINN, tr("Введите пожалуйста\nИНН сотрудника"), 2) &&
		checkInput(comboBoxInvalid, tr("Введите пожалуйста\nинвалидность сотрудника"), 2) &&
		checkInput(comboBoxMember, tr("Введите пожалуйста\nвхождение сотрудника в профсоюз"), 3) &&
		checkInput(lineEditOrhProfbilet, tr("Введите пожалуйста\nорган. выдавшую профбилет"), 3) &&
		checkInput(comboBoxProfActiv, tr("Введите пожалуйста\nпрофсоюзный актив"), 3) &&
		checkInput(comboBoxElected, tr("Введите пожалуйста\nизбрание"), 3)
	   ) {
		accept();

	}
}

void DialogSotr::setModel(ProfimaxRelationalModel* modd)
{

	model = modd;

	tabWidget->setCurrentIndex(0);

	// Main Page
	QSqlTableModel* relModel = model->relationModel(model->fieldIndex("sex_id"));
	comboBoxSex->setModel(relModel);
	comboBoxSex->setModelColumn(relModel->fieldIndex("name"));

	// Page Место работы
	relModel = model->relationModel(model->fieldIndex("category_id"));
	comboBoxCategory->setModel(relModel);
	comboBoxCategory->setModelColumn(relModel->fieldIndex("name"));
	relModel = model->relationModel(model->fieldIndex("cond_id"));
	comboBoxCond->setModel(relModel);
	comboBoxCond->setModelColumn(relModel->fieldIndex("name"));
	relModel = model->relationModel(model->fieldIndex("soc_id"));
	comboBoxSoc->setModel(relModel);
	comboBoxSoc->setModelColumn(relModel->fieldIndex("name"));
	relModel = model->relationModel(model->fieldIndex("edu_id"));
	comboBoxEdu->setModel(relModel);
	comboBoxEdu->setModelColumn(relModel->fieldIndex("name"));

	// Page Специальный
	relModel = model->relationModel(model->fieldIndex("dolgn_id"));
	comboBoxDolgn->setModel(relModel);
	comboBoxDolgn->setModelColumn(relModel->fieldIndex("name"));
	relModel = model->relationModel(model->fieldIndex("otdel_id"));
	comboBoxOtdel->setModel(relModel);
	comboBoxOtdel->setModelColumn(relModel->fieldIndex("name"));
	relModel = model->relationModel(model->fieldIndex("invalid_id"));
	comboBoxInvalid->setModel(relModel);
	comboBoxInvalid->setModelColumn(relModel->fieldIndex("name"));

	// Page Профсоюз
	relModel = model->relationModel(model->fieldIndex("member_id"));
	relModel->setFilter("member_id=1 or member_id=5");
	comboBoxMember->setModel(relModel);
	comboBoxMember->setModelColumn(relModel->fieldIndex("name"));
	relModel = model->relationModel(model->fieldIndex("elected_id"));
	comboBoxElected->setModel(relModel);
	comboBoxElected->setModelColumn(relModel->fieldIndex("name"));
	relModel = model->relationModel(model->fieldIndex("profactiv_id"));
	comboBoxProfActiv->setModel(relModel);
	comboBoxProfActiv->setModelColumn(relModel->fieldIndex("name"));


	mapper->setModel(model);
	mapper->setItemDelegate(new ProfimaxDelegate());
	//mapper->setItemDelegate(new QSqlRelationalDelegate());

	// Main Page
	//mapper->addMapping(toolButtonFoto, model->fieldIndex("foto"));
	mapper->addMapping(lineEditLastName, model->fieldIndex("lastname"));
	mapper->addMapping(lineEditFirstName, model->fieldIndex("firstname"));
	mapper->addMapping(lineEditMidName, model->fieldIndex("midname"));
	mapper->addMapping(comboBoxSex, model->fieldIndex("sex_id"));
	mapper->addMapping(dateEditBirthday, model->fieldIndex("birthday"));
	mapper->addMapping(lineEditCity, model->fieldIndex("city"));
	mapper->addMapping(spinBoxPost, model->fieldIndex("post"));
	mapper->addMapping(lineEditStreet, model->fieldIndex("street"));
	mapper->addMapping(spinBoxHouse, model->fieldIndex("house"));
	mapper->addMapping(spinBoxCase, model->fieldIndex("case"));
	mapper->addMapping(spinBoxFlat, model->fieldIndex("flat"));

	// Page Место работы
	mapper->addMapping(spinBoxTabNum, model->fieldIndex("tabnum"));
	mapper->addMapping(comboBoxDolgn->comboBox, model->fieldIndex("dolgn_id"));
	mapper->addMapping(comboBoxOtdel->comboBox, model->fieldIndex("otdel_id"));
	mapper->addMapping(comboBoxCategory, model->fieldIndex("category_id"));
	mapper->addMapping(comboBoxCond, model->fieldIndex("cond_id"));
	mapper->addMapping(comboBoxSoc, model->fieldIndex("soc_id"));
	mapper->addMapping(comboBoxEdu->comboBox, model->fieldIndex("edu_id"));

	// TableView для телефона
	tableViewTel->setTable("prof_tel");
	tableViewTel->model->setEditStrategy(QSqlTableModel::OnManualSubmit);
	tableViewTel->setButtoIconSize(16);
	tableViewTel->toolButtonFilter->hide();
	tableViewTel->toolButtonEdit->hide();
	tableViewTel->lineToolView2->hide();
	tableViewTel->setMargin(2);
	tableViewTel->model->setRelation(1,  QSqlRelation("prof_teltype", "teltype_id", "name"));
	tableViewTel->select();

	QStringList headName;
	headName << "teltype_id" << "nomer";
	tableViewTel->setSortColumn(headName);
	tableViewTel->setHeader("teltype_id", tr("Тип"));
	tableViewTel->setHeader("nomer", tr("Ноиер"));
	tableViewTel->tableViewSql->setColumnWidth(1, 70);
	tableViewTel->show();


	// TableView для работы старой
	tableViewPrevWork->setTable("prof_prework");
	tableViewPrevWork->model->setEditStrategy(QSqlTableModel::OnManualSubmit);
	tableViewPrevWork->setButtoIconSize(16);
	tableViewPrevWork->toolButtonFilter->hide();
	tableViewPrevWork->toolButtonEdit->hide();
	tableViewPrevWork->lineToolView2->hide();
	tableViewPrevWork->setMargin(2);

	tableViewPrevWork->model->setRelation(3, QSqlRelation("prof_dolgn", "dolgn_id", "name"));
	tableViewPrevWork->model->setRelation(4, QSqlRelation("prof_otdel", "otdel_id", "name"));

	tableViewPrevWork->select();

	headName << "date" << "dolgn_id" << "otdel_id";
	tableViewPrevWork->setSortColumn(headName);
	tableViewPrevWork->setHeader("date", tr("Дата"));
	tableViewPrevWork->setHeader("dolgn_id", tr("Должность"));
	tableViewPrevWork->setHeader("otdel_id", tr("Отдел"));
	tableViewPrevWork->show();

	// TableView для дат рождения детей
	tableViewChild->setTable(tr("prof_child"));
	tableViewChild->model->setEditStrategy(QSqlTableModel::OnManualSubmit);
	tableViewChild->setButtoIconSize(16);
	tableViewChild->toolButtonFilter->hide();
	tableViewChild->toolButtonEdit->hide();
	tableViewChild->lineToolView2->hide();
	tableViewChild->setMargin(2);
	tableViewChild->select();
	headName << "date";
	tableViewChild->setSortColumn(headName);
	tableViewChild->setHeader("date", tr("Дата"));
	tableViewChild->show();

	// Page Специальный
	mapper->addMapping(spinBoxPaspNum, model->fieldIndex("pasp_num"));
	mapper->addMapping(spinBoxPaspSer, model->fieldIndex("pasp_ser"));
	mapper->addMapping(dateEditPaspDate, model->fieldIndex("pasp_date"));
	mapper->addMapping(lineEditPaspPlace, model->fieldIndex("pasp_place"));
	mapper->addMapping(spinBoxINN, model->fieldIndex("inn"));
	mapper->addMapping(checkBoxNeedHome, model->fieldIndex("need_home"));
	mapper->addMapping(dateEditNeedDate, model->fieldIndex("need_date"));
	mapper->addMapping(checkBoxVeteran, model->fieldIndex("veteran"));
	mapper->addMapping(checkBoxTrauma, model->fieldIndex("trauma"));
	mapper->addMapping(checkBoxProvZab, model->fieldIndex("profzab"));
	mapper->addMapping(comboBoxInvalid, model->fieldIndex("invalid_id"));

	// Page Профсоюз
	mapper->addMapping(comboBoxMember, model->fieldIndex("member_id"));
	mapper->addMapping(lineEditOrhProfbilet, model->fieldIndex("org_profbilet"));
	mapper->addMapping(spinBoxNumProfbilet, model->fieldIndex("num_profbilet"));
	mapper->addMapping(dateEditDateIn, model->fieldIndex("date_in"));
	mapper->addMapping(checkBoxProfbiletOut, model->fieldIndex("profbilet_out"));
	mapper->addMapping(checkBoxAdmitted, model->fieldIndex("admitted"));
	mapper->addMapping(spinBoxStage, model->fieldIndex("stage"));
	mapper->addMapping(comboBoxElected, model->fieldIndex("elected_id"));
	mapper->addMapping(comboBoxProfActiv, model->fieldIndex("profactiv_id"));
	mapper->addMapping(checkBoxKadrSotr, model->fieldIndex("kadrsotr"));
	//mapper->toFirst();
	connect(comboBoxOtdel, SIGNAL(clickChoose()), this, SLOT(onChooseOtdel()));
	connect(comboBoxDolgn, SIGNAL(clickChoose()), this, SLOT(onChooseDolgn()));
	connect(comboBoxEdu, SIGNAL(clickChoose()), this, SLOT(onChooseEdu()));
}

void DialogSotr::onChooseOtdel()
{
	TreeSqlOtdel* tree = new TreeSqlOtdel(ppo);
	QDialog* dialog = new QDialog(this);
	dialog->setWindowTitle(tr("Подразделения"));

	QVBoxLayout* layout = new QVBoxLayout();
	layout->addWidget(tree);

	tree->layout()->setSpacing(0);
	tree->layout()->setMargin(0);

	QDialogButtonBox* buttonbox = new QDialogButtonBox(QDialogButtonBox::Ok
		| QDialogButtonBox::Cancel);

	connect(buttonbox, SIGNAL(accepted()), dialog, SLOT(accept()));
	connect(buttonbox, SIGNAL(rejected()), dialog, SLOT(reject()));

	layout->addWidget(buttonbox);

	//layout->setSpacing(0);
	layout->setMargin(6);
	dialog->setLayout(layout);

	if (dialog->exec() == QDialog::Accepted) {

		model->relationModel(model->fieldIndex("otdel_id"))->select();
	}

	//return tree;
}

void DialogSotr::onChooseEdu()
{
	TableSqlObraz* tableSql = new TableSqlObraz();
	tableSql->toolButtonEdit->hide();
	QDialog* dialog = new QDialog(this);
	dialog->setWindowTitle(tr("Образование"));
	QVBoxLayout* layout = new QVBoxLayout();
	layout->addWidget(tableSql);
	tableSql->layout()->setSpacing(0);
	tableSql->layout()->setMargin(0);
	QDialogButtonBox* buttonbox = new QDialogButtonBox(QDialogButtonBox::Ok
		| QDialogButtonBox::Cancel);

	connect(buttonbox, SIGNAL(accepted()), dialog, SLOT(accept()));
	connect(buttonbox, SIGNAL(rejected()), dialog, SLOT(reject()));
	layout->addWidget(buttonbox);
	layout->setSpacing(6);
	layout->setMargin(6);
	dialog->setLayout(layout);

	if (dialog->exec() == QDialog::Accepted) {

		model->relationModel(model->fieldIndex("edu_id"))->select();
	}

}


void DialogSotr::onChooseDolgn()
{
	TableSqlDolgn* tableSql = new TableSqlDolgn();
	tableSql->toolButtonEdit->hide();
	QDialog* dialog = new QDialog(this);
	dialog->setWindowTitle(tr("Должности"));
	QVBoxLayout* layout = new QVBoxLayout();
	layout->addWidget(tableSql);
	tableSql->layout()->setSpacing(0);
	tableSql->layout()->setMargin(0);
	QDialogButtonBox* buttonbox = new QDialogButtonBox(QDialogButtonBox::Ok
		| QDialogButtonBox::Cancel);

	connect(buttonbox, SIGNAL(accepted()), dialog, SLOT(accept()));
	connect(buttonbox, SIGNAL(rejected()), dialog, SLOT(reject()));
	layout->addWidget(buttonbox);
	layout->setSpacing(6);
	layout->setMargin(6);
	dialog->setLayout(layout);

	if (dialog->exec() == QDialog::Accepted) {

		model->relationModel(model->fieldIndex("dolgn_id"))->select();
	}

}

void DialogSotr::submitAll()
{
	// &&tableViewTel->model->submit()&&tableViewChild->model->submit()

	if (mapper->submit()) {
		model->submitAll();

		tableViewTel->model->submitAll();
		tableViewPrevWork->model->submitAll();
		tableViewChild->model->submitAll();
	}
	else {
		QMessageBox::critical(0, "", QString("%1").arg(model->lastError().text()));
		//revertAll();
		QMessageBox::critical(0, "", tr("Ошибка ввода данных"));
	}
}

void DialogSotr::revertAll()
{
	model->revertAll();
	tableViewTel->model->revertAll();
	tableViewPrevWork->model->revertAll();
	tableViewChild->model->revertAll();
}

void DialogSotr::setCurrentIndex(int index)
{

	mapidx = index;
	if (mapidx == -1)
		mapidx = 0;
	mapper->setCurrentIndex(index);

	QString curidx = model->record(mapidx).value("sotr_id").toString();
	if (curidx == QString("")) {
		QSqlQuery query("select uuid_generate()");
		query.next();
		curidx = query.value(0).toString();
		QSqlRecord rec = model->record(mapidx);
		rec.setValue("sotr_id", curidx);
		model->setRecord(mapidx, rec);
	}

	tableViewTel->model->setFilter(QString("sotr_id='%1'").arg(curidx));
	tableViewPrevWork->model->setFilter(QString("sotr_id='%1'").arg(curidx));
	tableViewChild->model->setFilter(QString("sotr_id='%1'").arg(curidx));

	QStringList insval;
	insval << "sotr_id" << QString("%1").arg(curidx);
	tableViewTel->setInsertValue(insval);
	tableViewPrevWork->setInsertValue(insval);
	tableViewChild->setInsertValue(insval);

	QPixmap pixmap;
	QByteArray bytes = model->record(mapidx).value("foto").toByteArray();
	if (bytes.count() > 0) {
		pixmap.loadFromData(bytes);
		QIcon icon(pixmap);

		toolButtonFoto->setIcon(icon);
	}

}

void DialogSotr::setEditable(bool fl)
{


	// Main Page
	lineEditLastName->setEnabled(fl);
	lineEditFirstName->setEnabled(fl);
	lineEditMidName->setEnabled(fl);
	comboBoxSex->setEnabled(fl);
	dateEditBirthday->setEnabled(fl);
	lineEditCity->setEnabled(fl);
	spinBoxPost->setEnabled(fl);
	lineEditStreet->setEnabled(fl);
	spinBoxHouse->setEnabled(fl);
	spinBoxCase->setEnabled(fl);
	spinBoxFlat->setEnabled(fl);
	toolButtonFoto->setEnabled(fl);

	// Page Место работы
	spinBoxTabNum->setEnabled(fl);
	comboBoxDolgn->setEnabled(fl);
	comboBoxOtdel->setEnabled(fl);
	comboBoxCategory->setEnabled(fl);
	comboBoxCond->setEnabled(fl);
	comboBoxSoc->setEnabled(fl);
	comboBoxEdu->setEnabled(fl);

	// TableView для телефона
	tableViewTel->tableViewSql->setEditTriggers(QAbstractItemView::NoEditTriggers);
	tableViewTel->toolButtonInsert->hide();
	tableViewTel->toolButtonDelete->hide();

	tableViewPrevWork->tableViewSql->setEditTriggers(QAbstractItemView::NoEditTriggers);
	tableViewPrevWork->toolButtonInsert->hide();
	tableViewPrevWork->toolButtonDelete->hide();

	tableViewChild->tableViewSql->setEditTriggers(QAbstractItemView::NoEditTriggers);
	tableViewChild->toolButtonInsert->hide();
	tableViewChild->toolButtonDelete->hide();

	// Page Специальный
	spinBoxPaspNum->setEnabled(fl);
	spinBoxPaspSer->setEnabled(fl);
	dateEditPaspDate->setEnabled(fl);
	lineEditPaspPlace->setEnabled(fl);
	spinBoxINN->setEnabled(fl);
	checkBoxNeedHome->setEnabled(fl);
	dateEditNeedDate->setEnabled(fl);
	checkBoxVeteran->setEnabled(fl);
	checkBoxTrauma->setEnabled(fl);
	checkBoxProvZab->setEnabled(fl);
	comboBoxInvalid->setEnabled(fl);

	// Page Профсоюз
	comboBoxMember->setEnabled(fl);
	lineEditOrhProfbilet->setEnabled(fl);
	spinBoxNumProfbilet->setEnabled(fl);
	dateEditDateIn->setEnabled(fl);
	checkBoxProfbiletOut->setEnabled(fl);
	checkBoxAdmitted->setEnabled(fl);
	comboBoxElected->setEnabled(fl);
	comboBoxProfActiv->setEnabled(fl);
	checkBoxKadrSotr->setEnabled(fl);
	spinBoxStage->setEnabled(fl);

}

void DialogSotr::onChooseFoto()
{
	QString fileName = "";
	fileName = QFileDialog::getOpenFileName(this,
											tr("Открытие фотографии"), ".", tr("Файлы фотографий(*.png *.jpg *.bmp)"));

	if (fileName != "") {
		toolButtonFoto->setIcon(QIcon(fileName));
		QByteArray bytes;
		QBuffer buffer(&bytes);
		buffer.open(QIODevice::WriteOnly);
		toolButtonFoto->icon().pixmap(140, 180).save(&buffer, "PNG");
		QSqlRecord record = model->record(mapidx);
		record.setValue("foto", bytes);
		model->setRecord(mapidx, record);
	}
}
