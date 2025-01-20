#pragma once

// ���⼭ BEGIN�� ���� �ʴ������� �ٸ� ������� ������ ������
// �ٸ� ������� ��������ʴ°��� ����Ǳ� ���� ( ��ȣ���� ���� or ��������� �����ġ ������ ���� �޶����⿡ )
namespace Engine  
{
	template<typename T>
	void Safe_Delete(T& pPointer)
	{
		if (nullptr != pPointer)
		{
			delete pPointer;
			pPointer = nullptr;
		}
	}

	template<typename T>
	void Safe_Delete_Array(T& pArray)
	{
		if (nullptr != pArray)
		{
			delete[] pArray;
			pArray = nullptr;
		}
	}

	template<typename T>
	unsigned int Safe_Release(T& pInstance)
	{
		unsigned int iRefCnt = 0;

		if (nullptr != pInstance)
		{
			iRefCnt = pInstance->Release();	 // ���� ������� ��ü�� ����Ű�� ����ī��Ʈ ����

			if (0 == iRefCnt)
				pInstance = nullptr;
		}

		return iRefCnt;
	}

	template<typename T>
	unsigned int Safe_AddRef(T& pInstance)
	{
		unsigned int iRefCnt = 0;

		if (nullptr != pInstance)
			iRefCnt = pInstance->AddRef();

		return iRefCnt;
	}
}


//#include "../Public/CEvent_Manager.h"
//namespace Engine
//{
//	struct CLONE_DESC
//	{
//		_uint iLevelIndex;
//		_wstring strLayerTag;
//		_wstring strPrototypeTag;
//		void* pArg;
//	};
//
//	void Add_CloneObject_ToLayer(_uint iLevelIndex, const _wstring& strLayerTag, const _wstring& strPrototypeTag, void* pArg)
//	{
//		CEvent_Manager::tEvent evn = {};
//		evn.eEven = CEvent_Manager::EVENT_TYPE::CREATE_OBJECT;
//
//		CLONE_DESC CloneDesc{};
//
//		CloneDesc.iLevelIndex = iLevelIndex;
//		CloneDesc.strLayerTag = strLayerTag;
//		CloneDesc.strPrototypeTag = strPrototypeTag;
//		CloneDesc.pArg = pArg;
//
//		evn.pArg = &CloneDesc;
//		CEvent_Manager::GetInst()->AddEvent(evn);
//	}
//}






