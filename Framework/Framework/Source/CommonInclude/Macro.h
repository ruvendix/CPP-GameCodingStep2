#pragma once

#define GENERATE_SINGLETON(Type)\
public:\
	static Type& GetInstance()\
	{\
		static Type inst;\
		return inst;\
	}

#define SINGLETON(Type) Type::GetInstance()

#define COM_ERROR(errCode)\
{\
	std::string strLog;\
	SINGLETON(ErrorHandler).ConvertComErrorString(errCode, strLog);\
    ::MessageBox(nullptr, strLog.c_str(), "COM ¿À·ù", MB_ICONERROR);\
}

#define ASSERT(expr) assert(expr)

#define TEST_COM(expr, hRet)\
	hRet = expr;\
	if (FAILED(hRet))\
	{\
		COM_ERROR(hRet);\
	}

#define OUTSIDE_CLASS(InsideType)\
friend class InsideType;\
std::shared_ptr<class InsideType> m_spInside;

#define INSIDE_CLASS(OutSideType, InsideType)\
public:\
	InsideType(OutSideType* pOutside)\
	{\
		m_pOutside = pOutside;\
	}\
private:\
		OutSideType* m_pOutside = nullptr;