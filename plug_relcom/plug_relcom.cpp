#include <QtGui>
#include <QMessageBox>
#include <QFileDialog>

#include "plug_relcom.h"

#include <math.h>
#include <stdlib.h>
#include "qsqltreeviewmodel.h"
#include "dialogvibsotr.h"
#include "treesqlwidget.h"
#include "dialogeditotdel.h"
#include "dialogkvp.h"
#include "treesqlotdel.h"
#include "tablesqldolgn.h"
#include "tablesqlobraz.h"
#include "dialogppo.h"
#include "dialogswitchppo.h"


RelcomPlugin::RelcomPlugin()
{
	ModulName = tr("Учет профсоюза");
	ModulIcon = QIcon(tr(":icon/icons/user.png"));

}

void RelcomPlugin::initPlug()
{
	QSettings* settings = new QSettings("Roand", "ProfiMAX");
	settings->beginGroup("plug_relcom");
	indexPPO = settings->value("PPO", "").toString();
	settings->endGroup();
	delete settings;

	if (indexPPO != QString("")) {
		QSqlQuery query;
		query.exec(QString("SELECT * FROM prof_ppo WHERE ppo_id='%1'").arg(indexPPO));

		while (query.next())
			mainWindow->setWindowTitle(tr("Профи MAX - ") + query.record().value(query.record().indexOf("name")).toString());
	}
	else {
		getSubWindowAll(tr("SwitchPPO"));
	}
}

QVariant RelcomPlugin::GetQueryResult(const QString& query)
{
	QSqlQuery q(query);
	q.exec();
	if (q.size() >= 1) {
		q.first();
		return q.value(0);
	}
	else
		return 0;
}

QWidget* RelcomPlugin::callReportPlugin(QString sprav)
{

	if (sprav == tr("ReportMain")) {

		ProfimaxReport* report = new ProfimaxReport();
		report->setReportName(tr("Общие сведения"));

		// -- Генератор отчета
		QSqlQuery query("SELECT name, post, adress, pred, predtel, zam, zamtel, ruk, ruktel, chprof, chpred, urface FROM prof_ppo WHERE ppo_id='" + QString("%1").arg(indexPPO) + "'");
		QStringList rep;
		while (query.next()) {
			rep << "DATA" << QDate::currentDate().toString("dd-MM-yyyy");
			rep << "NAME" << query.value(0).toString();
			rep << "INDEX" << query.value(1).toString();
			rep << "ADDRESS" << query.value(2).toString();
			rep << "PRED" << query.value(3).toString();
			rep << "PREDTEL" << query.value(4).toString();
			rep << "PREDPPO" << query.value(5).toString();
			rep << "PREDPPOTEL" << query.value(6).toString();
			rep << "RUK" << query.value(7).toString();
			rep << "RUKTEL" << query.value(8).toString();
			rep << "WORKER" << GetQueryResult("select count(*) from prof_ppo where ppo_id='" + QString("%1").arg(indexPPO) + "'").toString();
			rep << "MEMBER" << GetQueryResult("select count(*) from prof_ppo where ppo_id='" + QString("%1").arg(indexPPO) + "' and member=1").toString();
			rep << "MEMBER1" << query.value(9).toString();
			rep << "MEMBER2" << query.value(10).toString();
			if (query.value(11).toInt() == 0)
				rep << "UR" << tr("Нет");
			else
				rep << "UR" << tr("Да");
		}
		report->setInsertParam(rep);
		QString initialFile = "report/general.html";

		if (!report->load(initialFile))
			QMessageBox::critical(0, "", tr("Отчет не загрузился"));
		else
			return report;


	}

	if (sprav == tr("ReportForm1e")) {
		ProfimaxReport* report = new ProfimaxReport();

		report->setReportName(tr("Форма 1Э"));

		// -- Генератор отчета
		QStringList rep;
		QSqlQuery query("SELECT name, chprof, chpred FROM prof_ppo WHERE ppo_id='" + QString("%1").arg(indexPPO) + "'");
		query.next();
		rep << "C1" << query.value(0).toString();
		rep << "C2" << query.value(1).toString();
		rep << "C3" << query.value(2).toString();
		QString initialFile = "report/Forma1.html";
		report->setInsertParam(rep);
		if (!report->load(initialFile))
			QMessageBox::critical(0, "", tr("Отчет не загрузился"));
		else
			return report;
	}

	if (sprav == tr("ReportForm2")) {
		ProfimaxReport* report = new ProfimaxReport();
		report->setReportName(tr("Форма 2"));

		int n, n1;
		float f;
		QDate D = QDate::currentDate();
		D.addYears(-35);
		QString Strdate = "'" + D.toString("dd-MM-yyyy") + '"';
		QSqlQuery query("SELECT name, post, adress, pred, predtel FROM prof_ppo WHERE ppo_id='" + QString("%1").arg(indexPPO) + "'");
		QStringList rep;
		while (query.next()) {
			QString Preq = "SELECT COUNT(*) FROM prof_sotr WHERE ppo_id=" + QString("%1").arg(indexPPO) + " ";
			rep << "PROFNAME" << tr("Профсоюз машиностроителей РБ");
			rep << "ORGNAME" << query.value(0).toString();
			rep << "ADDRESS" << query.value(2).toString();
			rep << "PRED" << query.value(3).toString();
			rep << "PREDTEL" << query.value(4).toString();

			//------------------------------------------------------------------------
			rep << "T1_13" << GetQueryResult(Preq + " and soc_id=1").toString();
			rep << "T1_23" << GetQueryResult(Preq + " and soc_id=1 and member_id=1").toString();
			rep << "T1_33" << GetQueryResult(Preq + " and soc_id=1 and member_id=1 and admitted=1").toString();
			rep << "T1_43" << GetQueryResult(Preq + " and soc_id=4").toString();
			rep << "T1_53" << GetQueryResult(Preq + " and soc_id=4 and member_id=1").toString();
			rep << "T1_63" << GetQueryResult(Preq + " and soc_id=4 and member_id=1 and admitted=1").toString();
			rep << "T1_73" << GetQueryResult(Preq + " and soc_id=1 or Soc=4").toString();
			rep << "T1_83" << GetQueryResult(Preq + " and (soc_id=1 or Soc=4) and (member_id=1)").toString();
			n = GetQueryResult(Preq + " and soc_id=1 or soc_id=4").toInt();
			n1 = GetQueryResult(Preq + " and (soc_id=1 or soc_id=4) and (member_id=1)").toInt();
			if (n == 0)
				f = 0;
			else
				f = n1 / n * 100;
			rep << "T1_93" << QString("%1").arg(f) + "%";
			rep << "T1_103" << GetQueryResult(Preq + " and member_id=1 and soc_id=3").toString();
			rep << "T1_113" << GetQueryResult(Preq + " and member_id=1 and soc_id=2").toString();
			rep << "T1_123" << GetQueryResult(Preq + " and member_id=1").toString();
			rep << "T1_133" << GetQueryResult(Preq + " and member_id=3").toString();
			rep << "T1_143" << GetQueryResult(Preq + " and member_id=4").toString();

			//-------------------------------------------------------------------------
			rep << "T1_14" << GetQueryResult(Preq + " and soc_id=1 and category_id=1").toString();
			rep << "T1_24" << GetQueryResult(Preq + " and soc_id=1 and member_id=1 and category_id=1").toString();
			rep << "T1_34" << GetQueryResult(Preq + " and soc_id=1 and member_id=1 and admitted=1 and category_id=1").toString();
			rep << "T1_74" << GetQueryResult(Preq + " and soc_id=1 or Soc=4 and category_id=1").toString();
			rep << "T1_84" << GetQueryResult(Preq + " and (soc_id=1 or Soc=4) and (member_id=1) and category_id=1").toString();
			n = GetQueryResult(Preq + " and soc_id=1 or soc_id=4 and category_id=1").toInt();
			n1 = GetQueryResult(Preq + " and (soc_id=1 or soc_id=4) and (member_id=1) and category_id=1").toInt();
			if (n == 0)
				f = 0;
			else
				f = n1 / n * 100;
			rep << "T1_94" << QString("%1").arg(f) + "%";
			rep << "T1_104" << GetQueryResult(Preq + " and member_id=1 and soc_id=3 and category_id=1").toString();
			rep << "T1_114" << GetQueryResult(Preq + " and member_id=1 and soc_id=2 and category_id=1").toString();
			rep << "T1_124" << GetQueryResult(Preq + " and member_id=1 and category_id=1").toString();
			rep << "T1_134" << GetQueryResult(Preq + " and member_id=3 and category_id=1").toString();
			rep << "T1_144" << GetQueryResult(Preq + " and member_id=4 and category_id=1").toString();

			//-----------------------------------------------------------------------------
			rep << "T1_15" << GetQueryResult(Preq + " and soc_id=1 and sex_id=1").toString();
			rep << "T1_25" << GetQueryResult(Preq + " and soc_id=1 and member_id=1 and sex_id=1").toString();
			rep << "T1_35" << GetQueryResult(Preq + " and soc_id=1 and member_id=1 and admitted=1 and sex_id=1").toString();
			rep << "T1_45" << GetQueryResult(Preq + " and soc_id=4 and sex_id=1").toString();
			rep << "T1_55" << GetQueryResult(Preq + " and soc_id=4 and member_id=1 and sex_id=1").toString();
			rep << "T1_65" << GetQueryResult(Preq + " and soc_id=4 and member_id=1 and admitted=1 and sex_id=1").toString();
			rep << "T1_75" << GetQueryResult(Preq + " and soc_id=1 or Soc=4 and sex_id=1").toString();
			rep << "T1_85" << GetQueryResult(Preq + " and (soc_id=1 or Soc=4) and (member_id=1) and sex_id=1").toString();
			n = GetQueryResult(Preq + " and soc_id=1 or soc_id=4 and sex_id=1").toInt();
			n1 = GetQueryResult(Preq + " and (soc_id=1 or soc_id=4) and (member_id=1) and sex_id=1").toInt();
			if (n == 0)
				f = 0;
			else
				f = n1 / n * 100;
			rep << "T1_95" << QString("%1").arg(f) + "%";
			rep << "T1_105" << GetQueryResult(Preq + " and member_id=1 and soc_id=3 and sex_id=1").toString();
			rep << "T1_115" << GetQueryResult(Preq + " and member_id=1 and soc_id=2 and sex_id=1").toString();
			rep << "T1_125" << GetQueryResult(Preq + " and member_id=1 and sex_id=1").toString();
			rep << "T1_135" << GetQueryResult(Preq + " and member_id=3 and sex_id=1").toString();
			rep << "T1_145" << GetQueryResult(Preq + " and member_id=4 and sex_id=1").toString();

			//------------------------------------------------------------------------
			rep << "T1_16" << GetQueryResult(Preq + " and soc_id=1 and birthday>" + Strdate).toString();
			rep << "T1_26" << GetQueryResult(Preq + " and soc_id=1 and member_id=1 and birthday>" + Strdate).toString();
			rep << "T1_36" << GetQueryResult(Preq + " and soc_id=1 and member_id=1 and admitted=1 and birthday>" + Strdate).toString();
			rep << "T1_46" << GetQueryResult(Preq + " and soc_id=4 and birthday>" + Strdate).toString();
			rep << "T1_56" << GetQueryResult(Preq + " and soc_id=4 and member_id=1 and birthday>" + Strdate).toString();
			rep << "T1_66" << GetQueryResult(Preq + " and soc_id=4 and member_id=1 and admitted=1 and birthday>" + Strdate).toString();
			rep << "T1_76" << GetQueryResult(Preq + " and soc_id=1 or Soc=4 and birthday>" + Strdate).toString();
			rep << "T1_86" << GetQueryResult(Preq + " and (soc_id=1 or Soc=4) and (member_id=1) and birthday>" + Strdate).toString();
			n = GetQueryResult(Preq + "and soc_id=1 or soc_id=4 and birthday>" + Strdate).toInt();
			n1 = GetQueryResult(Preq + "and (soc_id=1 or soc_id=4) and (member_id=1) and birthday>" + Strdate).toInt();
			if (n == 0)
				f = 0;
			else
				f = n1 / n * 100;
			rep << "T1_96" << QString("%1").arg(f) + "%";
			rep << "T1_106" << GetQueryResult(Preq + "and member_id=1 and soc_id=3 and birthday>" + Strdate).toString();
			rep << "T1_116" << GetQueryResult(Preq + "and member_id=1 and soc_id=2 and birthday>" + Strdate).toString();
			rep << "T1_126" << GetQueryResult(Preq + "and member_id=1 and birthday>" + Strdate).toString();
			rep << "T1_136" << GetQueryResult(Preq + "and member_id=3 and birthday>" + Strdate).toString();
			rep << "T1_146" << GetQueryResult(Preq + "and member_id=4 and birthday>" + Strdate).toString();

			//------------------------------------------------------------------------
			rep << "T2_13" << GetQueryResult(Preq + "and profactiv_id=1 and member_id=1").toString();
			rep << "T2_23" << GetQueryResult(Preq + "and profactiv_id=1 and kadrsotr=1 and member_id=1").toString();
			rep << "T2_33" << GetQueryResult(Preq + "and profactiv_id=2 and member_id=1").toString();
			rep << "T2_43" << GetQueryResult(Preq + "and profactiv_id=2 and kadrsotr=1 and member_id=1").toString();
			rep << "T2_53" << GetQueryResult(Preq + "and profactiv_id=7 and member_id=1").toString();
			rep << "T2_63" << GetQueryResult(Preq + "and profactiv_id=3 and member_id=1").toString();
			rep << "T2_73" << GetQueryResult(Preq + "and profactiv_id=4 and member_id=1").toString();
			rep << "T2_83" << GetQueryResult(Preq + "and profactiv_id=4 and kadrsotr=1 and member_id=1").toString();
			rep << "T2_93" << GetQueryResult(Preq + "and profactiv_id=5 and member_id=1").toString();
			rep << "T2_103" << GetQueryResult(Preq + "and profactiv_id=6 and member_id=1").toString();

			//---------------------------------------------------------------------------
			rep << "T2_13" << GetQueryResult(Preq + "and profactiv_id=1 and member_id=1").toString();
			rep << "T2_23" << GetQueryResult(Preq + "and profactiv_id=1 and kadrsotr=1 and member_id=1").toString();
			rep << "T2_33" << GetQueryResult(Preq + "and profactiv_id=2 and member_id=1").toString();
			rep << "T2_43" << GetQueryResult(Preq + "and profactiv_id=2 and kadrsotr=1 and member_id=1").toString();
			rep << "T2_53" << GetQueryResult(Preq + "and profactiv_id=7 and member_id=1").toString();
			rep << "T2_63" << GetQueryResult(Preq + "and profactiv_id=3 and member_id=1").toString();
			rep << "T2_73" << GetQueryResult(Preq + "and profactiv_id=4 and member_id=1").toString();
			rep << "T2_83" << GetQueryResult(Preq + "and profactiv_id=4 and kadrsotr=1 and member_id=1").toString();
			rep << "T2_93" << GetQueryResult(Preq + "and profactiv_id=5 and member_id=1").toString();
			rep << "T2_103" << GetQueryResult(Preq + "and profactiv_id=6 and member_id=1").toString();

			//---------------------------------------------------------
			rep << "T2_14" << GetQueryResult(Preq + "and profactiv_id=1 and member_id=1 and category_id=1").toString();
			rep << "T2_24" << GetQueryResult(Preq + "and profactiv_id=1 and kadrsotr=1 and member_id=1 and category_id=1").toString();
			rep << "T2_34" << GetQueryResult(Preq + "and profactiv_id=2 and member_id=1 and category_id=1").toString();
			rep << "T2_44" << GetQueryResult(Preq + "and profactiv_id=2 and kadrsotr=1 and member_id=1 and category_id=1").toString();
			rep << "T2_54" << GetQueryResult(Preq + "and profactiv_id=7 and member_id=1 and category_id=1").toString();
			rep << "T2_64" << GetQueryResult(Preq + "and profactiv_id=3 and member_id=1 and category_id=1").toString();
			rep << "T2_74" << GetQueryResult(Preq + "and profactiv_id=4 and member_id=1 and category_id=1").toString();
			rep << "T2_84" << GetQueryResult(Preq + "and profactiv_id=4 and kadrsotr=1 and member_id=1 and category_id=1").toString();
			rep << "T2_94" << GetQueryResult(Preq + "and profactiv_id=5 and member_id=1 and category_id=1").toString();
			rep << "T2_104" << GetQueryResult(Preq + "and profactiv_id=6 and member_id=1 and category_id=1").toString();

			//---------------------------------------------------------
			rep << "T2_15" << GetQueryResult(Preq + "and profactiv_id=1 and member_id=1 and sex_id=2").toString();
			rep << "T2_25" << GetQueryResult(Preq + "and profactiv_id=1 and kadrsotr=1 and member_id=1 and sex_id=2").toString();
			rep << "T2_35" << GetQueryResult(Preq + "and profactiv_id=2 and member_id=1 and sex_id=2").toString();
			rep << "T2_45" << GetQueryResult(Preq + "and profactiv_id=2 and kadrsotr=1 and member_id=1 and sex_id=2").toString();
			rep << "T2_55" << GetQueryResult(Preq + "and profactiv_id=7 and member_id=1 and sex_id=2").toString();
			rep << "T2_65" << GetQueryResult(Preq + "and profactiv_id=3 and member_id=1 and sex_id=2").toString();
			rep << "T2_75" << GetQueryResult(Preq + "and profactiv_id=4 and member_id=1 and sex_id=2").toString();
			rep << "T2_85" << GetQueryResult(Preq + "and profactiv_id=4 and kadrsotr=1 and member_id=1 and sex_id=2").toString();
			rep << "T2_95" << GetQueryResult(Preq + "and profactiv_id=5 and member_id=1 and sex_id=2").toString();
			rep << "T2_105" << GetQueryResult(Preq + "and profactiv_id=6 and member_id=1 and sex_id=2").toString();

			//-----------------------------------------------------------
			rep << "T2_16" << GetQueryResult(Preq + "and profactiv_id=1 and member_id=1 and birthday>" + Strdate).toString();
			rep << "T2_26" << GetQueryResult(Preq + "and profactiv_id=1 and kadrsotr=1 and member_id=1 and birthday>" + Strdate).toString();
			rep << "T2_36" << GetQueryResult(Preq + "and profactiv_id=2 and member_id=1 and birthday>" + Strdate).toString();
			rep << "T2_46" << GetQueryResult(Preq + "and profactiv_id=2 and kadrsotr=1 and member_id=1 and birthday>" + Strdate).toString();
			rep << "T2_56" << GetQueryResult(Preq + "and profactiv_id=7 and member_id=1 and birthday>" + Strdate).toString();
			rep << "T2_66" << GetQueryResult(Preq + "and profactiv_id=3 and member_id=1 and birthday>" + Strdate).toString();
			rep << "T2_76" << GetQueryResult(Preq + "and profactiv_id=4 and member_id=1 and birthday>" + Strdate).toString();
			rep << "T2_86" << GetQueryResult(Preq + "and profactiv_id=4 and kadrsotr=1 and member_id=1 and birthday>" + Strdate).toString();
			rep << "T2_96" << GetQueryResult(Preq + "and profactiv_id=5 and member_id=1 and birthday>" + Strdate).toString();
			rep << "T2_106" << GetQueryResult(Preq + "and profactiv_id=6 and member_id=1 and birthday>" + Strdate).toString();

			rep << "T3_13" << "";
			rep << "T3_14" << "";
			rep << "T3_15" << "";
			rep << "T3_16" << "";
			rep << "T3_23" << "";
			rep << "T3_24" << "";
			rep << "T3_25" << "";
			rep << "T3_26" << "";
			rep << "T3_33" << "";
			rep << "T3_34" << "";
			rep << "T3_35" << "";
			rep << "T3_36" << "";
			rep << "T3_43" << "";
			rep << "T3_44" << "";
			rep << "T3_45" << "";
			rep << "T3_46" << "";
			rep << "T3_53" << "";
			rep << "T3_54" << "";
			rep << "T3_55" << "";
			rep << "T3_56" << "";
			rep << "T3_63" << "";
			rep << "T3_64" << "";
			rep << "T3_65" << "";
			rep << "T3_66" << "";
			rep << "T3_73" << "";
			rep << "T3_74" << "";
			rep << "T3_75" << "";
			rep << "T3_76" << "";
			rep << "T4_13" << "";
			rep << "T4_14" << "";
			rep << "T4_15" << "";
			rep << "T4_16" << "";
			rep << "T4_23" << "";
			rep << "T4_24" << "";
			rep << "T4_25" << "";
			rep << "T4_26" << "";
			rep << "T4_33" << "";
			rep << "T4_34" << "";
			rep << "T4_35" << "";
			rep << "T4_36" << "";
			rep << "T4_43" << "";
			rep << "T4_44" << "";
			rep << "T4_45" << "";
			rep << "T4_46" << "";
			rep << "T4_53" << "";
			rep << "T4_54" << "";
			rep << "T4_55" << "";
			rep << "T4_56" << "";
			rep << "T4_63" << "";
			rep << "T4_64" << "";
			rep << "T4_65" << "";
			rep << "T4_66" << "";
			rep << "T4_73" << "";
			rep << "T4_74" << "";
			rep << "T4_75" << "";
			rep << "T4_76" << "";
			rep << "T4_83" << "";
			rep << "T4_84" << "";
			rep << "T4_85" << "";
			rep << "T4_86" << "";
			rep << "T4_93" << "";
			rep << "T4_94" << "";
			rep << "T4_95" << "";
			rep << "T4_96" << "";
			rep << "T4_103" << "";
			rep << "T4_104" << "";
			rep << "T4_105" << "";
			rep << "T4_106" << "";
			rep << "T4_113" << "";
			rep << "T4_114" << "";
			rep << "T4_115" << "";
			rep << "T4_116" << "";
			rep << "T4_123" << "";
			rep << "T4_124" << "";
			rep << "T4_125" << "";
			rep << "T4_126" << "";
			rep << "T4_133" << "";
			rep << "T4_134" << "";
			rep << "T4_135" << "";
			rep << "T4_136" << "";
			rep << "T5_13" << "";
			rep << "T5_14" << "";
			rep << "T5_15" << "";
			rep << "T5_16" << "";
			rep << "T5_23" << "";
			rep << "T5_24" << "";
			rep << "T5_25" << "";
			rep << "T5_26" << "";
			rep << "T5_33" << "";
			rep << "T5_34" << "";
			rep << "T5_35" << "";
			rep << "T5_36" << "";

		}
		report->setInsertParam(rep);
		QString initialFile = "report/Forma2.html";

		if (!report->load(initialFile))
			QMessageBox::critical(0, "", tr("Отчет не загрузился"));
		else
			return report;
	}

	if (sprav == tr("ReportForm4")) {
		ProfimaxReport* report = new ProfimaxReport();
		report->setReportName(tr("Форма 4"));

		QStringList rep;
		QSqlQuery query("SELECT name, post, adress, pred, predtel FROM prof_ppo WHERE ppo_id='" + QString("%1").arg(indexPPO) + "'");
		query.next();
		rep << "PROFNAME" << tr("Профсоюз машиностроителей РБ");
		rep << "ORGNAME" << query.value(0).toString();
		rep << "ADDRESS" << query.value(2).toString();
		rep << "PRED" << query.value(3).toString();
		rep << "PREDTEL" << query.value(4).toString();

		rep << "T1_13" << "";
		rep << "T1_23" << "";
		rep << "T1_33" << "";
		rep << "T1_43" << "";
		rep << "T1_53" << "";
		rep << "T1_63" << "";
		rep << "T1_73" << "";
		rep << "T1_83" << "";
		rep << "T1_93" << "";
		rep << "T2_13" << "";
		rep << "T2_14" << "";
		rep << "T2_23" << "";
		rep << "T2_24" << "";
		rep << "T2_33" << "";
		rep << "T2_34" << "";
		rep << "T2_43" << "";
		rep << "T2_44" << "";
		rep << "T2_53" << "";
		rep << "T2_54" << "";
		rep << "T2_63" << "";
		rep << "T2_64" << "";
		rep << "T2_73" << "";
		rep << "T2_74" << "";
		rep << "T2_83" << "";
		rep << "T2_84" << "";
		rep << "T2_93" << "";
		rep << "T2_94" << "";

		QString initialFile = "report/Forma4.html";
		report->setInsertParam(rep);
		if (!report->load(initialFile))
			QMessageBox::critical(0, "", tr("Отчет не загрузился"));
		else
			return report;

	}

	return NULL;
}

QWidget* RelcomPlugin::getSubWindowAll(QString sprav)
{

	if (sprav == tr("Sotr") || sprav == tr("Vibiv")) {
		TableSqlWidget* tableSql = new TableSqlWidget();
		tableSql->setEditStrategy(QSqlTableModel::OnFieldChange);
		tableSql->setTable(tr("prof_sotr"));

		tableSql->model->setRelation(6,  QSqlRelation("prof_sex", "sex_id", "name"));
		tableSql->model->setRelation(30, QSqlRelation("prof_dolgn", "dolgn_id", "name"));
		tableSql->model->setRelation(31, QSqlRelation("prof_otdel", "otdel_id", "name"));
		tableSql->model->setRelation(13, QSqlRelation("prof_soc", "soc_id", "name"));
		tableSql->model->setRelation(14, QSqlRelation("prof_category", "category_id", "name"));
		tableSql->model->setRelation(15, QSqlRelation("prof_cond", "cond_id", "name"));
		tableSql->model->setRelation(21, QSqlRelation("prof_invalid", "invalid_id", "name"));
		tableSql->model->setRelation(22, QSqlRelation("prof_member", "member_id", "name"));
		tableSql->model->setRelation(23, QSqlRelation("prof_edu", "edu_id", "name"));

		tableSql->model->setRelation(37, QSqlRelation("prof_elected", "elected_id", "name"));
		tableSql->model->setRelation(38, QSqlRelation("prof_profactiv", "profactiv_id", "name"));


		QStringList FieldNameBool;
		FieldNameBool << "need_home" << "trauma" << "veteran" << "profzab"
					  << "profbilet_out" << "admitted" << "kadrsotr";
		tableSql->setFieldNameBool(FieldNameBool);

		tableSql->select();

		if (sprav == tr("Vibiv")) {
			if (indexPPO != QString("00000000-0000-0000-0000-000000000000"))
				tableSql->setFilter(QString("prof_sotr.ppo_id='%1' and not (prof_sotr.member_id=1 or prof_sotr.member_id=5)").arg(indexPPO));
			else
				tableSql->setFilter(QString("not (prof_sotr.member_id=1 or prof_sotr.member_id=5)"));
		}
		else {
			if (indexPPO != QString("00000000-0000-0000-0000-000000000000"))
				tableSql->setFilter(QString("prof_sotr.ppo_id='%1' and (prof_sotr.member_id=1 or prof_sotr.member_id=5)").arg(indexPPO));
			else
				tableSql->setFilter(QString("(prof_sotr.member_id=1 or prof_sotr.member_id=5)"));
		}

		QStringList headName;
		headName << "lastname" << "firstname" << "midname" << "birthday" << "sex_id"
				 << "post" << "city" << "street" << "flat" << "house" << "case"
				 << "pasp_ser" << "pasp_num" << "pasp_date" << "pasp_place" << "inn"
				 << "tabnum" << "dolgn_id" << "otdel_id" << "soc_id" << "category_id"
				 << "cond_id" << "need_home" << "need_date" << "trauma" << "veteran"
				 << "profzab" << "invalid_id" << "member_id" << "edu_id"
				 << "org_profbilet" << "num_profbilet" << "date_in" << "profbilet_out" << "admitted"
				 << "elected_id" << "profactiv_id" << "kadrsotr" << "stage";
		tableSql->setSortColumn(headName);

		QStringList insval;
		insval << "ppo_id" << QString("%1").arg(indexPPO);
		tableSql->setInsertValue(insval);

		tableSql->setHeader("lastname", tr("Фамилия"));
		tableSql->setHeader("firstname", tr("Имя"));
		tableSql->setHeader("midname", tr("Отчество"));
		tableSql->setHeader("birthday", tr("День\nрождения"));
		tableSql->setHeader("sex_id", tr("Пол"));
		tableSql->setHeader("pasp_ser", tr("Серия\nпаспорта"));
		tableSql->setHeader("pasp_num", tr("Номер\nпаспорта"));
		tableSql->setHeader("pasp_date", tr("Дата выдачи\nпаспорта"));
		tableSql->setHeader("pasp_place", tr("Паспорт\nвыдан"));
		tableSql->setHeader("inn", tr("ИНН"));

		tableSql->setHeader("tabnum", tr("Табельный\nномер"));
		tableSql->setHeader("soc_id", tr("Cоциал.\nположение"));
		tableSql->setHeader("category_id", tr("Категория\nработника"));
		tableSql->setHeader("cond_id", tr("Условия\nтруда"));
		tableSql->setHeader("need_home", tr("Нуждается\nв жилье"));
		tableSql->setHeader("need_date", tr("Дата пред.\nжилья"));
		tableSql->setHeader("trauma", tr("Травма"));
		tableSql->setHeader("veteran", tr("Ветеран"));
		tableSql->setHeader("invalid_id", tr("Инвалид"));
		tableSql->setHeader("profzab", tr("Проф.\nзаболевание"));
		tableSql->setHeader("member_id", tr("Член\nпрофсоюза"));
		tableSql->setHeader("edu_id", tr("Образование"));

		tableSql->setHeader("post", tr("Индекс"));
		tableSql->setHeader("city", tr("Город"));
		tableSql->setHeader("street", tr("Улица"));
		tableSql->setHeader("flat", tr("Корпус"));
		tableSql->setHeader("house", tr("Дом"));
		tableSql->setHeader("case", tr("Квартира"));

		tableSql->setHeader("dolgn_id", tr("Должность"));
		tableSql->setHeader("otdel_id", tr("Отдел"));

		tableSql->setHeader("org_profbilet", tr("Орг. выдавшая\nпрофбилет"));
		tableSql->setHeader("num_profbilet", tr("Номер\nпрофбилета"));
		tableSql->setHeader("date_in", tr("Дата\nвыдачи"));
		tableSql->setHeader("profbilet_out", tr("Выдан"));
		tableSql->setHeader("admitted", tr("Принят\nвпервые"));
		tableSql->setHeader("elected_id", tr("Избран"));
		tableSql->setHeader("profactiv_id", tr("Профсоюзный\nактив"));
		tableSql->setHeader("kadrsotr", tr("Кадровый\nработник"));
		tableSql->setHeader("stage", tr("Профсоюзный\nстаж"));

		tableSql->show();

		QObject::connect(tableSql->toolButtonEdit, SIGNAL(clicked()),
						 this, SLOT(editDialogSotr()));

		QObject::connect(tableSql->toolButtonInsert, SIGNAL(clicked()),
						 this, SLOT(editDialogSotr()));

		if (sprav == tr("Vibiv")) {
			tableSql->tableViewSql->setEditTriggers(QAbstractItemView::NoEditTriggers);
			tableSql->toolButtonInsert->hide();
			tableSql->toolButtonDelete->hide();


		}
		else {
			QToolButton* toolButtonKvp = new QToolButton();
			toolButtonKvp->setObjectName(QString::fromUtf8("toolButtonЛмз"));
			QIcon iconKvp;
			iconKvp.addPixmap(QPixmap(QString::fromUtf8(":/icon/icons/kvp.png")), QIcon::Normal, QIcon::Off);
			toolButtonKvp->setIcon(iconKvp);
			toolButtonKvp->setIconSize(QSize(20, 20));
			//toolButtonVib->setCheckable(true);
			toolButtonKvp->setAutoRaise(true);
			toolButtonKvp->setToolTip(tr("Добавить в кассу взаимопомощи"));
			tableSql->horizontalLayoutViewTool->insertWidget(8, toolButtonKvp);

			QObject::connect(toolButtonKvp, SIGNAL(clicked()),
							 this, SLOT(editDialogKvp()));

			QToolButton* toolButtonVib = new QToolButton();
			toolButtonVib->setObjectName(QString::fromUtf8("toolButtonVib"));
			QIcon iconVib;
			iconVib.addPixmap(QPixmap(QString::fromUtf8(":/icon/icons/document_stop.png")), QIcon::Normal, QIcon::Off);
			toolButtonVib->setIcon(iconVib);
			toolButtonVib->setIconSize(QSize(20, 20));
			//toolButtonVib->setCheckable(true);
			toolButtonVib->setAutoRaise(true);
			toolButtonVib->setToolTip(tr("Оформить выбытие"));
			tableSql->horizontalLayoutViewTool->insertWidget(8, toolButtonVib);

			QObject::connect(toolButtonVib, SIGNAL(clicked()),
							 this, SLOT(vibDialogSotr()));

		}


		return tableSql;
	}

	if (sprav == tr("Podraz")) {
		TreeSqlOtdel* tree = new TreeSqlOtdel(indexPPO);
		return tree;
	}

	if (sprav == tr("ListPPO")) {
		TableSqlWidget* tableSql = new TableSqlWidget();
		tableSql->setTable(tr("prof_ppo"));
		tableSql->setEditStrategy(QSqlTableModel::OnFieldChange);

		QStringList FieldNameBool;
		FieldNameBool << "urface";
		tableSql->setFieldNameBool(FieldNameBool);
		tableSql->setFilter(QString("not ppo_id='00000000-0000-0000-0000-000000000000'"));
		tableSql->select();

		QStringList headName;
		headName << "name" << "code" << "post" << "adress" << "urface" << "ruk"
				 << "ruktel" << "pred" << "predtel" << "zam" << "zamtel" << "chpred"
				 << "chprof" << "bdown" << "bup" << "subject" << "otrasl" << "profname"
				 << "ur_adress" << "name_bank" << "adress_bank" << "inn"
				 << "rasch_schet" << "korr_schet" << "bik";
		tableSql->setSortColumn(headName);
		tableSql->setHeader("name", tr("Наименование\nпредприятия"));
		tableSql->setHeader("code", tr("Код"));
		tableSql->setHeader("post", tr("Индекс\nпредприятия"));
		tableSql->setHeader("adress", tr("Адрес\nпредприятия"));
		tableSql->setHeader("urface", tr("Юридич.\nлицо"));
		tableSql->setHeader("ruk", tr("Руководитель\nпредприятия"));
		tableSql->setHeader("ruktel", tr("Телефон\nруководителя"));
		tableSql->setHeader("pred", tr("Председатель\nпрофсоюза"));
		tableSql->setHeader("predtel", tr("Телефон\nпредседателя"));
		tableSql->setHeader("zam", tr("Заместитель\nпредседателя"));
		tableSql->setHeader("zamtel", tr("Тел.замест.\nпредседателя"));
		tableSql->setHeader("chprof", tr("Численность на\nпрофсоюзе"));
		tableSql->setHeader("chpred", tr("Численность на\nработодателе"));
		tableSql->setHeader("bup", tr("Интервал проф.\nбилета до"));
		tableSql->setHeader("bdown", tr("Интервал проф.\nбилета от"));
		tableSql->setHeader("subject", tr("Субъект РФ"));
		tableSql->setHeader("otrasl", tr("Отрасль"));
		tableSql->setHeader("profname", tr("Наименование\nпровсоюза"));
		tableSql->setHeader("ur_adress", tr("Юридический\nадрес"));
		tableSql->setHeader("name_bank", tr("Наименование\nбанка"));
		tableSql->setHeader("adress_bank", tr("Адрес\nбанка"));
		tableSql->setHeader("inn", tr("ИНН"));
		tableSql->setHeader("rasch_schet", tr("Рассчетный\nсчет"));
		tableSql->setHeader("korr_schet", tr("Корр.\nсчет"));
		tableSql->setHeader("bik", tr("БИК"));
		tableSql->show();

		QObject::connect(tableSql->toolButtonEdit, SIGNAL(clicked()),
						 this, SLOT(editDialogPPO()));
		QObject::connect(tableSql->toolButtonInsert, SIGNAL(clicked()),
						 this, SLOT(editDialogPPO()));

		return tableSql;
	}

	if (sprav == tr("Dolgn")) {
		TableSqlDolgn* tableSql = new TableSqlDolgn();
		tableSql->toolButtonEdit->hide();

		return  tableSql;

	}

	if (sprav == tr("SwitchPPO")) {

		DialogSwitchPPO* windowSwitchPPO = new DialogSwitchPPO(NULL);

		windowSwitchPPO->setPPO(indexPPO);
		if (windowSwitchPPO->exec() == QDialog::Accepted) {
			int idx = windowSwitchPPO->comboBoxPPO->currentIndex();
			indexPPO = windowSwitchPPO->model->record(idx).value("ppo_id").toString();
			mainWindow->setWindowTitle(tr("Профи MAX - ") + windowSwitchPPO->comboBoxPPO->currentText());

			QSettings* settings = new QSettings("Roand", "ProfiMAX");
			settings->beginGroup("plug_relcom");
			settings->setValue("PPO", indexPPO);
			settings->endGroup();
			delete settings;

		}

		delete windowSwitchPPO;
		return  NULL;

	}

	if (sprav == tr("Edu")) {
		TableSqlObraz* tableSql = new TableSqlObraz();
		tableSql->toolButtonEdit->hide();

		return  tableSql;

	}

	if (sprav == tr("kassavzaim")) {
		TableSqlWidget* tableSql = new TableSqlWidget();
		tableSql->setEditStrategy(QSqlTableModel::OnFieldChange);
		tableSql->setTable(tr("prof_kvp_view"));
		tableSql->setRelation("dolgn_id", "prof_dolgn", "dolgn_id", "name");

		tableSql->select();

		QStringList headName;
		headName << "doc_date" << "tabnum" << "lastname" << "firstname" << "midname" << "dolgn_id" << "stage"
				 << "pasp_ser" << "pasp_num" << "pasp_date" << "pasp_place"
				 << "post" << "city" << "street" << "flat" << "house" << "case"
				 << "plastname" << "pfirstname" << "pmidname" << "pdolgn" << "prabota"
				 << "pcity" <<  "ppost" << "pstreet" << "phouse" << "pcase" << "pflat"
				 << "ppasp_num" << "ppasp_ser" << "ppasp_date" << "ppasp_place"
				 << "pdog_num" << "pdog_date"
				 << "doc_date" << "bank" << "dogssoud_num" << "dogssoud_date"
				 << "protokol_num" << "protokol_date" << "schet" << "nazn"
				 << "text_summa" << "text_srok" << "begin_date" << "otkaz"
				 << "summa" << "srok";
		tableSql->setSortColumn(headName);

		tableSql->setHeader("lastname", tr("Фамилия"));
		tableSql->setHeader("firstname", tr("Имя"));
		tableSql->setHeader("midname", tr("Отчество"));

		tableSql->setHeader("tabnum", tr("Табельный\nномер"));
		tableSql->setHeader("dolgn_id", tr("Должность"));
		tableSql->setHeader("stage", tr("Профсоюзный\nстаж"));

		tableSql->setHeader("pasp_ser", tr("Серия\nпаспорта"));
		tableSql->setHeader("pasp_num", tr("Номер\nпаспорта"));
		tableSql->setHeader("pasp_date", tr("Дата выдачи\nпаспорта"));
		tableSql->setHeader("pasp_place", tr("Паспорт\nвыдан"));

		tableSql->setHeader("post", tr("Индекс"));
		tableSql->setHeader("city", tr("Город"));
		tableSql->setHeader("street", tr("Улица"));
		tableSql->setHeader("flat", tr("Корпус"));
		tableSql->setHeader("house", tr("Дом"));
		tableSql->setHeader("case", tr("Квартира"));

		tableSql->setHeader("plastname", tr("Фамилия\nпоручителя"));
		tableSql->setHeader("pfirstname", tr("Имя\nпоручителя"));
		tableSql->setHeader("pmidname", tr("Отчество\nпоручителя"));

		tableSql->setHeader("pcity", tr("Город\nпоручителя"));
		tableSql->setHeader("ppost", tr("Индекс\nпоручителя"));
		tableSql->setHeader("pstreet", tr("Улица\nпоручителя"));
		tableSql->setHeader("phouse", tr("Дом\nпоручителя"));
		tableSql->setHeader("pcase", tr("квартира\nпоручителя"));
		tableSql->setHeader("pflat", tr("Корпус\nпоручителя"));

		tableSql->setHeader("ppasp_num", tr("Серия паспорта\nпоручителя"));
		tableSql->setHeader("ppasp_ser", tr("Номер паспорта\nпоручителя"));
		tableSql->setHeader("ppasp_date", tr("Дата выдачи\n паспорта поручителю"));
		tableSql->setHeader("ppasp_place", tr("Пвспорт выдан\nпоручителю"));

		tableSql->setHeader("pdolgn", tr("Должность\nпоручителя"));
		tableSql->setHeader("prabota", tr("Место работы\nпоручителя"));
		tableSql->setHeader("pdog_num", tr("Номер договора\nпоручительства"));
		tableSql->setHeader("pdog_date", tr("Дата договора\nпоручительства"));

		tableSql->setHeader("doc_date", tr("Дата\nдокумента"));
		tableSql->setHeader("bank", tr("Наименование\nбанка"));
		tableSql->setHeader("dogssoud_num", tr("Номер договора\nссуды"));
		tableSql->setHeader("dogssoud_date", tr("Дата договора\nссуды"));
		tableSql->setHeader("protokol_num", tr("Номер\nпротокола"));
		tableSql->setHeader("protokol_date", tr("Дата\nпротокола"));
		tableSql->setHeader("schet", tr("Счет"));
		tableSql->setHeader("nazn", tr("Назначение\nссуды"));
		tableSql->setHeader("text_summa", tr("Сумма ссуды\nтекстом"));
		tableSql->setHeader("text_srok", tr("Срок ссуды\nтекстом"));
		tableSql->setHeader("begin_date", tr("Месяц\nотсчета"));
		tableSql->setHeader("otkaz", tr("Причина\nотказа"));
		tableSql->setHeader("summa", tr("Сумма\nссуды"));
		tableSql->setHeader("srok", tr("Срок\nссуды"));

		tableSql->toolButtonInsert->hide();
		//tableSql->toolButtonDelete->hide();

		QObject::connect(tableSql->toolButtonEdit, SIGNAL(clicked()),
						 this, SLOT(editDialogKvp()));


		tableSql->show();

		return tableSql;

	}

	if (sprav == tr("ImportXLS")) {

		QString fileName = QFileDialog::getOpenFileName(tabWidgetMDI , tr("Импорт из Excel"), "", tr("Excel Files (*.xls)"));
		if (fileName == tr(""))
			return NULL;
        FILE* fileCSV = fopen(fileName.toLatin1().data(),"r");
		while (!feof(fileCSV)){
			// Чтение данных и запись в БД
			fscanf(fileCSV,"%*s");
		}
		fclose(fileCSV);

		return NULL;
	}

	if (sprav == tr("ExportXLS")) {

		QString fileName = QFileDialog:: getSaveFileName(tabWidgetMDI, tr("Экспорт в Excel"), "", tr("Excel Files (*.xls)"));
		if (fileName == tr(""))
			return NULL;

        FILE* fileCSV = fopen(fileName.toLatin1().data(),"w");
		QSqlQuery query("SELECT * FROM prof_sotr");
		while (query.next()) {
			QString lastname = query.value(query.record().indexOf("lastname")).toString();
			QString firstname = query.value(query.record().indexOf("firstname")).toString();
			QString midname = query.value(query.record().indexOf("midname")).toString();
			QString birthday = query.value(query.record().indexOf("birthday")).toDate().toString("dd.MM.yyyy");
/*			QString sex_id = query.value(query.record().indexOf("lastname")).toString();

			QString post = query.value(query.record().indexOf("lastname")).toInt();
			QString city = query.value(query.record().indexOf("lastname")).toString();
			QString street = query.value(query.record().indexOf("lastname")).toString();
			QString house = query.value(query.record().indexOf("lastname")).toString();
			QString ccase = query.value(query.record().indexOf("case")).toString();
			QString flat = query.value(query.record().indexOf("flat")).toString();

			QString dolgn_id = query.value(query.record().indexOf("lastname")).toInt();
			QString tabnum = query.value(query.record().indexOf("lastname")).toInt();
			QString rabtel = query.value(query.record().indexOf("lastname")).toInt();
			QString domtel = query.value(query.record().indexOf("lastname")).toInt();
			QString cond_id = query.value(query.record().indexOf("lastname")).toInt();
			QString edu_id = query.value(query.record().indexOf("lastname")).toInt();
			QString soc_id = query.value(query.record().indexOf("lastname")).toInt();

			QString pasp_ser = query.value(query.record().indexOf("lastname")).toInt();
			QString pasp_num = query.value(query.record().indexOf("lastname")).toInt();
			QString pasp_place = query.value(query.record().indexOf("lastname")).toInt();
			QString pasp_date = query.value(query.record().indexOf("lastname")).toInt();
			QString inn = query.value(query.record().indexOf("lastname")).toInt();

			QString trauma = query.value(query.record().indexOf("lastname")).toInt();
			QString profzab = query.value(query.record().indexOf("lastname")).toInt();
			QString kadrsotr = query.value(query.record().indexOf("lastname")).toInt();
			QString veteran = query.value(query.record().indexOf("lastname")).toInt();
			QString invalid_id = query.value(query.record().indexOf("lastname")).toInt();
			QString member_id = query.value(query.record().indexOf("lastname")).toInt();
			QString org_profbilet = query.value(query.record().indexOf("lastname")).toInt();
			QString num_profbilet = query.value(query.record().indexOf("lastname")).toInt();
			QString date_in = query.value(query.record().indexOf("lastname")).toInt();

			QString profbilet_out = query.value(query.record().indexOf("lastname")).toInt();
			QString admitted = query.value(query.record().indexOf("lastname")).toInt();
			QString elected_id = query.value(query.record().indexOf("lastname")).toInt();
			QString profactiv_id = query.value(query.record().indexOf("lastname")).toInt();
			QString child_count = query.value(query.record().indexOf("lastname")).toInt();

			QString child1 = query.value(query.record().indexOf("lastname")).toInt();
			QString child2 = query.value(query.record().indexOf("lastname")).toInt();
			QString child3 = query.value(query.record().indexOf("lastname")).toInt();
			QString child4 = query.value(query.record().indexOf("lastname")).toInt();
			QString child5 = query.value(query.record().indexOf("lastname")).toInt();

			QString need_home = query.value(query.record().indexOf("lastname")).toInt();
			QString category_id = query.value(query.record().indexOf("lastname")).toInt();
			QString date_rab_1_5 = query.value(query.record().indexOf("lastname")).toInt();
			QString dolgn_rab_1_5 = query.value(query.record().indexOf("lastname")).toInt();
			QString podraz_rab_1_5 = query.value(query.record().indexOf("lastname")).toInt();

			QString member_id = query.value(query.record().indexOf("lastname")).toInt();
			QString date_in = query.value(query.record().indexOf("lastname")).toInt();
*/
			fprintf(fileCSV,"%s;%s;%s;%s \n",
                    lastname.toLatin1().data(),
                    firstname.toLatin1().data(),
                    midname.toLatin1().data(),
                    birthday.toLatin1().data());
		}
		fclose(fileCSV);

		return NULL;
	}

	return callReportPlugin(sprav);
}

void RelcomPlugin::editDialogPPO()
{

	TableSqlWidget* tableSql = qobject_cast<TableSqlWidget*>(sender()->parent());
	tableSql->setEditStrategy(QSqlTableModel::OnRowChange);
	DialogPPO* windowPPO = new DialogPPO(tabWidgetMDI);
	windowPPO->setModel(tableSql->model);

	int row = tableSql->tableViewSql->currentIndex().row();
	if (sender()->objectName() != QString("toolButtonEdit"))
		row--;
	if (row < 0)
		row = 0;
	windowPPO->mapper->setCurrentIndex(row);

	windowPPO->exec();
	delete windowPPO;
	tableSql->model->submit();
	tableSql->setEditStrategy(QSqlTableModel::OnFieldChange);
}

void RelcomPlugin::editDialogSotr()
{

	TableSqlWidget* tableSql = qobject_cast<TableSqlWidget*>(sender()->parent());

	tableSql->setEditStrategy(QSqlTableModel::OnRowChange);

	int row = tableSql->tableViewSql->currentIndex().row();
	if (sender()->objectName() != QString("toolButtonEdit")) {
		row--;
		if (indexPPO == QString("00000000-0000-0000-0000-000000000000")) {
			QMessageBox::critical(0, tr("Ошибка вставки записи"), tr("Не выбран ППО"));
			tableSql->model->revertAll();
			tableSql->setEditStrategy(QSqlTableModel::OnFieldChange);

			return;
		}
	}
	if (row < 0)
		row = 0;


	DialogSotr* windowSotr = new DialogSotr(tabWidgetMDI);
	windowSotr->ppo = indexPPO;
	windowSotr->setModel(tableSql->model);
	windowSotr->setCurrentIndex(row);
	if (tableSql->tableViewSql->editTriggers() == QAbstractItemView::NoEditTriggers)
		windowSotr->setEditable(false);


	if (windowSotr->exec() == QDialog::Accepted) {
		windowSotr->setEditable(true);
		windowSotr->submitAll();
	}
	else {
		windowSotr->revertAll();
	}
	tableSql->setEditStrategy(QSqlTableModel::OnFieldChange);

	delete windowSotr;
}

void RelcomPlugin::editDialogKvp()
{


	TableSqlWidget* tableSql = qobject_cast<TableSqlWidget*>(sender()->parent());

	tableSql->setEditStrategy(QSqlTableModel::OnRowChange);

	int row = tableSql->tableViewSql->currentIndex().row();
	if (row < 0)
		row = 0;

	DialogKvp* windowKvp = new DialogKvp(tabWidgetMDI);

	windowKvp->setModel(tableSql->model);
	windowKvp->setCurrentIndex(row);



	if (windowKvp->exec() == QDialog::Accepted) {
		windowKvp->submitAll();
	}
	else {
		windowKvp->revertAll();
	}
	tableSql->setEditStrategy(QSqlTableModel::OnFieldChange);

	delete windowKvp;
}

void RelcomPlugin::vibDialogSotr()
{

	TableSqlWidget* tableSql = qobject_cast<TableSqlWidget*>(sender()->parent());

	tableSql->setEditStrategy(QSqlTableModel::OnRowChange);
	int row = tableSql->tableViewSql->currentIndex().row();
	if (row < 0)
		row = 0;

	DialogVibSotr* windowSotr = new DialogVibSotr(tabWidgetMDI);
	windowSotr->setModel(tableSql->model);
	windowSotr->setCurrentIndex(row);

	if (windowSotr->exec() == QDialog::Accepted) {
		windowSotr->submitAll();
	}
	else {
		windowSotr->revertAll();
	}
	tableSql->setEditStrategy(QSqlTableModel::OnFieldChange);

	delete windowSotr;
}

QList<QTreeWidgetItem*> RelcomPlugin::getSpravItems()
{
	QList<QTreeWidgetItem*> items;

	QTreeWidgetItem* item = new QTreeWidgetItem();
	QFont font = item->font(0);
	font.setBold(true);
	item->setFont(0, font);
	item->setText(0, tr("Общее"));
	items.append(item);

	QTreeWidgetItem* item1 = new QTreeWidgetItem(item);
	item1->setIcon(0, QIcon(tr(":icon/icons/user.png")));
	item1->setText(0, tr("Сотрудники"));
	setWidgetName(item1, tr("Sotr"));

	item1 = new QTreeWidgetItem(item);
	item1->setIcon(0, QIcon(tr(":icon/icons/userdel.png")));
	item1->setText(0, tr("Выбывшие"));
	setWidgetName(item1, tr("Vibiv"));

	item1 = new QTreeWidgetItem(item);
	item1->setIcon(0, QIcon(tr(":icon/icons/kvp.png")));
	item1->setText(0, tr("Члены КВП"));
	setWidgetName(item1, tr("kassavzaim"));

	item1 = new QTreeWidgetItem(item);
	item1->setIcon(0, QIcon(tr(":icon/icons/ppo_mini.png")));
	item1->setText(0, tr("Выбор ППО"));
	setWidgetName(item1, tr("SwitchPPO"));

	item = new QTreeWidgetItem();
	item->setText(0, tr("Справочники"));
	item->setFont(0, font);
	items.append(item);

	item1 = new QTreeWidgetItem(item);
	item1->setIcon(0, QIcon(tr(":icon/icons/group.png")));
	item1->setText(0, tr("Подразделения"));
	setWidgetName(item1, tr("Podraz"));

	item1 = new QTreeWidgetItem(item);
	item1->setIcon(0, QIcon(tr(":icon/icons/brief.png")));
	item1->setText(0, tr("Должности"));
	setWidgetName(item1, tr("Dolgn"));

	item1 = new QTreeWidgetItem(item);
	item1->setIcon(0, QIcon(tr(":icon/icons/science.png")));
	item1->setText(0, tr("Образование"));
	setWidgetName(item1, tr("Edu"));

	item1 = new QTreeWidgetItem(item);
	item1->setIcon(0, QIcon(tr(":icon/icons/ppo_mini.png")));
	item1->setText(0, tr("Список ППО"));
	setWidgetName(item1, tr("ListPPO"));

	item = new QTreeWidgetItem();
	item->setText(0, tr("Отчеты"));
	item->setFont(0, font);
	items.append(item);

	item1 = new QTreeWidgetItem(item);
	item1->setIcon(0, QIcon(tr(":icon/icons/document.png")));
	item1->setText(0, tr("Общие сведения"));
	setWidgetName(item1, tr("ReportMain"));

	item1 = new QTreeWidgetItem(item);
	item1->setIcon(0, QIcon(tr(":icon/icons/document.png")));
	item1->setText(0, tr("Форма 1Э"));
	setWidgetName(item1, tr("ReportForm1e"));

	item1 = new QTreeWidgetItem(item);
	item1->setIcon(0, QIcon(tr(":icon/icons/document.png")));
	item1->setText(0, tr("Форма 2"));
	setWidgetName(item1, tr("ReportForm2"));

	item1 = new QTreeWidgetItem(item);
	item1->setIcon(0, QIcon(tr(":icon/icons/document.png")));
	item1->setText(0, tr("Форма 4"));
	setWidgetName(item1, tr("ReportForm4"));

	item = new QTreeWidgetItem();
	item->setText(0, tr("Отчеты по КВП"));
	item->setFont(0, font);
	items.append(item);

	item1 = new QTreeWidgetItem(item);
	item1->setIcon(0, QIcon(tr(":icon/icons/document.png")));
	item1->setText(0, tr("Заявление 1"));
	setWidgetName(item1, tr("report_zayav1"));

	item1 = new QTreeWidgetItem(item);
	item1->setIcon(0, QIcon(tr(":icon/icons/document.png")));
	item1->setText(0, tr("Заявление 2"));
	setWidgetName(item1, tr("report_zayav2"));

	item1 = new QTreeWidgetItem(item);
	item1->setIcon(0, QIcon(tr(":icon/icons/document.png")));
	item1->setText(0, tr("Договор на ссуду"));
	setWidgetName(item1, tr("report_dogssoud"));

	item1 = new QTreeWidgetItem(item);
	item1->setIcon(0, QIcon(tr(":icon/icons/document.png")));
	item1->setText(0, tr("Договор поруч."));
	setWidgetName(item1, tr("report_dogporuch"));

	item1 = new QTreeWidgetItem(item);
	item1->setIcon(0, QIcon(tr(":icon/icons/document.png")));
	item1->setText(0, tr("Список членов"));
	setWidgetName(item1, tr("report_listmemver"));

	return items;

}

QList<QTreeWidgetItem*> RelcomPlugin::getAdminItems()
{
	QList<QTreeWidgetItem*> items;

	QTreeWidgetItem* item = new QTreeWidgetItem();
	item->setIcon(0, QIcon(tr(":icon/icons/document.png")));
	item->setText(0, tr("Импорт из Excel"));
	setWidgetName(item, tr("ImportXLS"));
	items.append(item);

	item = new QTreeWidgetItem();
	item->setIcon(0, QIcon(tr(":icon/icons/document.png")));
	item->setText(0, tr("Экспорт в Excel"));
	setWidgetName(item, tr("ExportXLS"));
	items.append(item);

	return items;

}
#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(plug_relcom, RelcomPlugin)
#endif
