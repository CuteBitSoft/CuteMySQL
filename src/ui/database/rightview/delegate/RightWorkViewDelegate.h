#pragma once
#include "ui/common/delegate/QDelegate.h"
#include "ui/common/notebook/QNotebook.h"
#include "ui/database/supplier/DatabaseSupplier.h"
#include "ui/database/rightview/page/ObjectsPage.h"
#include "ui/database/rightview/page/QueryPage.h"
#include "ui/database/rightview/page/HistoryPage.h"
#include "ui/database/rightview/page/TableStructurePage.h"

class RightWorkViewDelegate : public QDelegate<RightWorkViewDelegate, DatabaseSupplier>
{
public:
	// must call this to setup arguments
	void setup(QNotebook* tabView, std::vector<QueryPage*>& queryPagePtrs, std::vector<TableStructurePage*>& tablePagePtrs);

	void createFirstQueryPage();
	HistoryPage * createHistoryPage();
	// handle database event
	void openObjectsPage(TreeObjectType treeObjectType);
	void changeObjectsPage(TreeObjectType treeObjectType);

	// execute sql statement
	void execSelectedSql();
	void execAllSql();
	void explainSelectedSql();
private:
	QNotebook* tabView;
	ObjectsPage* objectsPage;
	HistoryPage* historyPage;
	std::vector<QueryPage*>* queryPagePtrs;
	std::vector<TableStructurePage*>* tablePagePtrs;

	void doCreateObjectsPage(TreeObjectType treeObjectType);
	void doChangeObjectsPage(TreeObjectType treeObjectType);
};

