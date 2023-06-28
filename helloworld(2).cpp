#include "pch.h"
#include "dbapserv.h"

void helloworld()
{
	acutPrintf(_T("\nHello world!"));

	auto pDb = acdbHostApplicationServices()->workingDatabase();

	AcDbBlockTable* pBT = nullptr;
	pDb->getBlockTable(pBT, AcDb::kForWrite);//从数据库获取块表
	if (pBT)//判断是否成功
	{
		//创建块
		auto pBtr = new AcDbBlockTableRecord();
		pBtr->setName(_T("Test"));
		AcDbObjectId btrId;
		pBT->add(btrId, pBtr);

		ads_name ss;//选择集名称
		ads_point pt1;//获取点
		ads_point pt2;
		if (pBtr)
		{
			acedGetPoint(NULL, _T("\n输入起点:"), pt1);
			acedGetCorner(pt1, _T("\n输入对角点:"), pt2);

			acedSSGet(_T("C"), pt1, pt2, NULL, ss);//框选实体（包括部分包含）

			Adesk::Int32 len = 0;
			acedSSLength(ss, &len);
			acutPrintf(_T("\n选中实体数:%d"), len);
			for (auto i = 0; i < len; i++) {
				ads_name en;
				acedSSName(ss, i, en);
				AcDbObjectId objId;
				acdbGetObjectId(objId, en);

				AcDbEntity* pEnt = nullptr;
				AcDbEntity *pNewEnt = nullptr;
				//acdbOpenAcDbEntity(pEnt, objId, AcDb::kForRead);
				if (Acad::ErrorStatus::eOk != acdbOpenObject(pEnt, objId, AcDb::OpenMode::kForWrite))
				{
					acutPrintf(_T("\n打开实体失败"));
					return;
				}
				pNewEnt = AcDbEntity::cast(pEnt->clone());
				pBtr->appendAcDbEntity(objId, pNewEnt);//把对象添加到块表记录里
				pEnt->close();
				pNewEnt->close();

				acutPrintf(_T("\n已加入实体数:%d"), i + 1);
			}
			acedSSFree(ss);

			pBtr->close();
		}
		AcDbBlockTableRecord* pBTR = nullptr;
		pBT->getAt(ACDB_MODEL_SPACE, pBTR, AcDb::kForWrite);//从块表获取模型空间的块表记录 
		pBT->close();//用完后马上关闭
		if (pBTR)
		{
			AcDbObjectId refId;
			auto pRef = new AcDbBlockReference(AcGePoint3d::kOrigin, btrId);
			pBTR->appendAcDbEntity(refId,pRef);
			pRef->close();
			ads_name enname;
			acdbGetAdsName(enname, refId);
			acedCommand(RTSTR, _T("Xclip"), 
				RTENAME, enname, 
				RTSTR, _T(""), 
				RTSTR, _T(""), 
				RTSTR, _T("R"), 
				RT3DPOINT, pt1, 
				RT3DPOINT, pt2, 
				RTNONE);

			AcDbEntity* pEnt = nullptr;
			acdbOpenAcDbEntity(pEnt, refId, AcDb::kForWrite);
			AcGeMatrix3d Mat;
			Mat.setToScaling(3, AcGePoint3d::kOrigin);
			pEnt->transformBy(Mat);
			pEnt->close();

			pBTR->close();
		}

	}

	pDb->saveAs(_T("D:\\homework5.dwg"));//保存图像
}




int showhello(struct resbuf *rb)
{
	ads_printf(_T("hello"));
	ads_retvoid();
	return RTNORM;
}

int showhellocmd(struct resbuf *rb)
{
	ads_printf(_T("hello"));
	ads_retvoid();
	return RTNORM;
}



