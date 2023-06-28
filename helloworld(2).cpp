#include "pch.h"
#include "dbapserv.h"

void helloworld()
{
	acutPrintf(_T("\nHello world!"));

	auto pDb = acdbHostApplicationServices()->workingDatabase();

	AcDbBlockTable* pBT = nullptr;
	pDb->getBlockTable(pBT, AcDb::kForWrite);//�����ݿ��ȡ���
	if (pBT)//�ж��Ƿ�ɹ�
	{
		//������
		auto pBtr = new AcDbBlockTableRecord();
		pBtr->setName(_T("Test"));
		AcDbObjectId btrId;
		pBT->add(btrId, pBtr);

		ads_name ss;//ѡ������
		ads_point pt1;//��ȡ��
		ads_point pt2;
		if (pBtr)
		{
			acedGetPoint(NULL, _T("\n�������:"), pt1);
			acedGetCorner(pt1, _T("\n����Խǵ�:"), pt2);

			acedSSGet(_T("C"), pt1, pt2, NULL, ss);//��ѡʵ�壨�������ְ�����

			Adesk::Int32 len = 0;
			acedSSLength(ss, &len);
			acutPrintf(_T("\nѡ��ʵ����:%d"), len);
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
					acutPrintf(_T("\n��ʵ��ʧ��"));
					return;
				}
				pNewEnt = AcDbEntity::cast(pEnt->clone());
				pBtr->appendAcDbEntity(objId, pNewEnt);//�Ѷ�����ӵ�����¼��
				pEnt->close();
				pNewEnt->close();

				acutPrintf(_T("\n�Ѽ���ʵ����:%d"), i + 1);
			}
			acedSSFree(ss);

			pBtr->close();
		}
		AcDbBlockTableRecord* pBTR = nullptr;
		pBT->getAt(ACDB_MODEL_SPACE, pBTR, AcDb::kForWrite);//�ӿ���ȡģ�Ϳռ�Ŀ���¼ 
		pBT->close();//��������Ϲر�
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

	pDb->saveAs(_T("D:\\homework5.dwg"));//����ͼ��
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



