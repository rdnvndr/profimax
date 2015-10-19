#include "dialogkvp.h"
#include <QToolTip>
#include <QMessageBox>

DialogKvp::DialogKvp(QWidget* pwgt): QDialog(pwgt)
{
    setupUi(this);
    tableViewTel->setButtoIconSize(16);
    tableViewTelP->setButtoIconSize(16);

    toolButtonFoto->setEnabled(false);
    lineEditLastName->setEnabled(false);
    lineEditFirstName->setEnabled(false);
    lineEditMidName->setEnabled(false);
    lineEditCity->setEnabled(false);
    spinBoxPost->setEnabled(false);
    lineEditStreet->setEnabled(false);
    spinBoxHouse->setEnabled(false);
    spinBoxCase->setEnabled(false);
    spinBoxFlat->setEnabled(false);

    spinBoxTabNum->setEnabled(false);
    comboBoxDolgn->setEnabled(false);

    spinBoxPaspNum->setEnabled(false);
    spinBoxPaspSer->setEnabled(false);
    dateEditPaspDate->setEnabled(false);
    lineEditPaspPlace->setEnabled(false);

    spinBoxStage->setEnabled(false);
    tableViewTel->tableViewSql->setEditTriggers(QAbstractItemView::NoEditTriggers);

    mapperSotr = new QDataWidgetMapper();
    modelSotr = new ProfimaxRelationalModel();

    mapper = new QDataWidgetMapper();
    model = new ProfimaxRelationalModel();

    connect(this->buttonBox,SIGNAL(accepted()),this,SLOT(apply()));


}

void DialogKvp::setModel(ProfimaxRelationalModel* modd) {



	modelSotr->setTable("prof_sotr");
	modelSotr->setRelation(30, QSqlRelation("prof_dolgn", "dolgn_id", "name"));
	modelSotr->select();

	QSqlTableModel* relModel = modelSotr->relationModel(modelSotr->fieldIndex("dolgn_id"));
	comboBoxDolgn->setModel(relModel);
	comboBoxDolgn->setModelColumn(relModel->fieldIndex("name"));

	mapperSotr->setModel(modelSotr);
	mapperSotr->setItemDelegate(new QSqlRelationalDelegate());

	// Main Page
	//mapperSotr->addMapping(toolButtonFoto, modelSotr->fieldIndex("foto"));
	mapperSotr->addMapping(lineEditLastName, modelSotr->fieldIndex("lastname"));
	mapperSotr->addMapping(lineEditFirstName, modelSotr->fieldIndex("firstname"));
	mapperSotr->addMapping(lineEditMidName, modelSotr->fieldIndex("midname"));

	mapperSotr->addMapping(lineEditCity, modelSotr->fieldIndex("city"));
	mapperSotr->addMapping(spinBoxPost, modelSotr->fieldIndex("post"));
	mapperSotr->addMapping(lineEditStreet, modelSotr->fieldIndex("street"));
	mapperSotr->addMapping(spinBoxHouse, modelSotr->fieldIndex("house"));
	mapperSotr->addMapping(spinBoxCase, modelSotr->fieldIndex("case"));
	mapperSotr->addMapping(spinBoxFlat, modelSotr->fieldIndex("flat"));

	mapperSotr->addMapping(spinBoxTabNum, modelSotr->fieldIndex("tabnum"));
	mapperSotr->addMapping(comboBoxDolgn, modelSotr->fieldIndex("dolgn_id"));
	//mapperSotr->addMapping(comboBoxOtdel->comboBox, modelSotr->fieldIndex("otdel_id"));

	mapperSotr->addMapping(spinBoxPaspNum, modelSotr->fieldIndex("pasp_num"));
	mapperSotr->addMapping(spinBoxPaspSer, modelSotr->fieldIndex("pasp_ser"));
	mapperSotr->addMapping(dateEditPaspDate, modelSotr->fieldIndex("pasp_date"));
	mapperSotr->addMapping(lineEditPaspPlace, modelSotr->fieldIndex("pasp_place"));
	mapperSotr->addMapping(spinBoxStage, modelSotr->fieldIndex("stage"));

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
	headName << "teltype_id"<< "nomer";
	tableViewTel->setSortColumn(headName);
	tableViewTel->setHeader("teltype_id", tr("Тип"));
	tableViewTel->setHeader("nomer", tr("Ноиер"));
	tableViewTel->show();


	//---------- ПОРУЧИТЕЛЬ ----------
	if (model->tableName()!="prof_kvp"){
		modelins = new ProfimaxRelationalModel();
		modelins->setTable("prof_kvp_view");
		modelins->select();
		modelins->setEditStrategy(QSqlTableModel::OnRowChange);
		model = modelins;
	} else {
	    model = modd;
	}

	mapper->setModel(model);
	mapper->setItemDelegate(new QSqlRelationalDelegate());

	mapper->addMapping(lineEditLastNameP, model->fieldIndex("plastname"));
	mapper->addMapping(lineEditFirstNameP, model->fieldIndex("pfirstname"));
	mapper->addMapping(lineEditMidNameP, model->fieldIndex("pmidname"));

	mapper->addMapping(lineEditCityP, model->fieldIndex("pcity"));
	mapper->addMapping(spinBoxPostP, model->fieldIndex("ppost"));
	mapper->addMapping(lineEditStreetP, model->fieldIndex("pstreet"));
	mapper->addMapping(spinBoxHouseP, model->fieldIndex("phouse"));
	mapper->addMapping(spinBoxCaseP, model->fieldIndex("pcase"));
	mapper->addMapping(spinBoxFlatP, model->fieldIndex("pflat"));

	mapper->addMapping(spinBoxPaspNumP, model->fieldIndex("ppasp_num"));
	mapper->addMapping(spinBoxPaspSerP, model->fieldIndex("ppasp_ser"));
	mapper->addMapping(dateEditPaspDateP, model->fieldIndex("ppasp_date"));
	mapper->addMapping(lineEditPaspPlaceP, model->fieldIndex("ppasp_place"));

	mapper->addMapping(lineEditDolgmP, model->fieldIndex("pdolgn"));
	mapper->addMapping(lineEditRabotaP, model->fieldIndex("prabota"));
	mapper->addMapping(lineEditDogNumP, model->fieldIndex("pdog_num"));
	mapper->addMapping(dateEditDogDateP, model->fieldIndex("pdog_date"));

	// TableView для телефона
	tableViewTelP->setTable("prof_kvptel");
	tableViewTelP->model->setEditStrategy(QSqlTableModel::OnManualSubmit);
	tableViewTelP->setButtoIconSize(16);
	tableViewTelP->toolButtonFilter->hide();
	tableViewTelP->toolButtonEdit->hide();
	tableViewTelP->lineToolView2->hide();
	tableViewTelP->setMargin(2);
	tableViewTelP->model->setRelation(1,  QSqlRelation("prof_teltype", "teltype_id", "name"));
	tableViewTelP->select();

	headName << "teltype_id"<< "nomer";
	tableViewTelP->setSortColumn(headName);
	tableViewTelP->setHeader("teltype_id", tr("Тип"));
	tableViewTelP->setHeader("nomer", tr("Ноиер"));
	tableViewTelP->show();



	mapper->addMapping(dateEditDocDate, model->fieldIndex("doc_date"));
	mapper->addMapping(lineEditBank, model->fieldIndex("bank"));
	mapper->addMapping(lineEditDogNumS, model->fieldIndex("dogssoud_num"));
	mapper->addMapping(dateEditDugDateS, model->fieldIndex("dogssoud_date"));
	mapper->addMapping(lineEditProtokolNum, model->fieldIndex("protokol_num"));
	mapper->addMapping(dateEditProtokolDate, model->fieldIndex("protokol_date"));
	mapper->addMapping(lineEditSchetNum, model->fieldIndex("schet"));
	mapper->addMapping(lineEditNazS, model->fieldIndex("nazn"));
	mapper->addMapping(lineEditSummaS, model->fieldIndex("text_summa"));
	mapper->addMapping(lineEditSrokS, model->fieldIndex("text_srok"));
	mapper->addMapping(dateEditMonthS, model->fieldIndex("begin_date"));
	mapper->addMapping(lineEditOtkazS, model->fieldIndex("otkaz"));
	mapper->addMapping(spinBoxSummaS, model->fieldIndex("summa"));
	mapper->addMapping(spinBoxSrokS, model->fieldIndex("srok"));

	model = modd;


}

void DialogKvp::apply(){

	if (checkInput(lineEditLastNameP,tr("Введите пожалуйста\nфамилию поручителя"),1)&&
		checkInput(lineEditFirstNameP,tr("Введите пожалуйста\nимя поручителя"),1)&&
		checkInput(lineEditMidNameP,tr("Введите пожалуйста\nотчество поручителя"),1)&&

		checkInput(spinBoxPaspSerP,tr("Введите пожалуйста\nсерию паспорта поручителя"),1)&&
		checkInput(spinBoxPaspNumP,tr("Введите пожалуйста\nномер паспорта поручителя"),1)&&
		checkInput(lineEditPaspPlaceP,tr("Введите пожалуйста\nкем выдан паспорт сотруднику"),1)&&

		checkInput(spinBoxPostP,tr("Введите пожалуйста\nиндекс поручителя"),1)&&
		checkInput(lineEditCityP,tr("Введите пожалуйста\nгород поручителя"),1)&&
		checkInput(lineEditStreetP,tr("Введите пожалуйста\nулицу поручителя"),1)&&
		checkInput(spinBoxHouseP,tr("Введите пожалуйста\nдом поручителя"),1)&&

		checkInput(lineEditDolgmP,tr("Введите пожалуйста\nдолжность поручителя"),1)&&
		checkInput(lineEditRabotaP,tr("Введите пожалуйста\n место работы поручителя"),1)&&
		checkInput(lineEditDogNumP,tr("Введите пожалуйста\n номер договора"),1)&&

		checkInput(lineEditBank,tr("Введите пожалуйста\nнаименование банка"),2)&&
		checkInput(lineEditDogNumS,tr("Введите пожалуйста\nномер договора"),2)&&
		checkInput(lineEditProtokolNum,tr("Введите пожалуйста\nномер протокола"),2)&&
		checkInput(lineEditSchetNum,tr("Введите пожалуйста\nномер счета"),2)&&
		checkInput(lineEditNazS,tr("Введите пожалуйста\nназначение суммы"),2)&&
		checkInput(spinBoxSummaS,tr("Введите пожалуйста\nсумму числом"),2)&&
		checkInput(lineEditSummaS,tr("Введите пожалуйста\nсумму текстом"),2)&&
		checkInput(spinBoxSrokS,tr("Введите пожалуйста\nсрок числом"),2)&&
		checkInput(lineEditSrokS,tr("Введите пожалуйста\nсрок текстом"),2)

		){
			accept();

		}
}

void DialogKvp::setCurrentIndex(int index){

	mapidx = index;
	if (mapidx==-1)
		mapidx=0;

	QString curidx = model->record(mapidx).value("sotr_id").toString();
	QString kvpidx = model->record(mapidx).value("kvp_id").toString();

	if (kvpidx==QString("")){
		QSqlQuery query("select uuid_generate()");
		query.next();
		kvpidx = query.value(0).toString();

		model = modelins;

		mapidx=0;

		model->insertRow(mapidx);

		QSqlRecord rec = model->record(mapidx);
		rec.setValue("sotr_id",curidx);
		rec.setValue("kvp_id",kvpidx);
		model->setRecord(mapidx,rec);
	}

	mapper->setCurrentIndex(mapidx);

	tableViewTelP->model->setFilter(QString("kvp_id='%1'").arg(kvpidx));
	QStringList insval;
	insval <<"kvp_id"<<QString("%1").arg(kvpidx);
	tableViewTelP->setInsertValue(insval);

	modelSotr->setFilter(QString("sotr_id='%1'").arg(curidx));
	tableViewTel->model->setFilter(QString("sotr_id='%1'").arg(curidx));
	mapperSotr->toFirst();

	QPixmap pixmap;
	QByteArray bytes = modelSotr->record(0).value("foto").toByteArray();
	if (bytes.count()>0){
	    pixmap.loadFromData(bytes);
	    QIcon icon(pixmap);

	    toolButtonFoto->setIcon(icon);
	}

}

bool DialogKvp::checkInput(QWidget* widget, QString mes,int tab){

	if (qobject_cast<QCBoxButton *>(widget)!=NULL){
		if (qobject_cast<QCBoxButton *>(widget)->currentIndex()>=0)
			return true;
		else{
			tabWidget->setCurrentIndex(tab);
			qobject_cast<QCBoxButton *>(widget)->comboBox->setFocus();
			//widget->setFocus();
			QToolTip::showText(widget->mapToGlobal(QPoint(0,5)),mes);
			return false;
		}
	}
	if (qobject_cast<QComboBox *>(widget)!=NULL){
		if (qobject_cast<QComboBox *>(widget)->currentIndex()>=0)
			return true;
		else{
			tabWidget->setCurrentIndex(tab);
			widget->setFocus();
			QToolTip::showText(widget->mapToGlobal(QPoint(0,5)),mes);
			return false;
		}
	}else if (qobject_cast<QLineEdit *>(widget)!=NULL){
		if (qobject_cast<QLineEdit *>(widget)->text()!=QString(""))
			return true;
		else{
			tabWidget->setCurrentIndex(tab);
			widget->setFocus();
			QToolTip::showText(widget->mapToGlobal(QPoint(0,5)),mes);
			return false;
		}
	}else if (qobject_cast<QSpinBox *>(widget)!=NULL){
		if (qobject_cast<QSpinBox *>(widget)->value()!=0)
			return true;
		else{
			tabWidget->setCurrentIndex(tab);
			widget->setFocus();
			QToolTip::showText(widget->mapToGlobal(QPoint(0,5)),mes);
			return false;
		}
	}else
		return true;


}

void DialogKvp::submitAll(){
	// &&tableViewTel->model->submit()&&tableViewChild->model->submit()

	if (mapper->submit()){
		model->submitAll();
		tableViewTelP->model->submitAll();
	}
	else{
		QMessageBox::critical(0,"",QString("%1").arg(model->lastError().text()));
		//revertAll();
		QMessageBox::critical(0,"",tr("Ошибка ввода данных"));
	}
}

void DialogKvp::revertAll(){
	model->revertAll();
	tableViewTelP->model->revertAll();
}
