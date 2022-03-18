#pragma once

#define SAFE_DELETE(ptr)\
	if (ptr != nullptr)\
	{\
		delete ptr;\
		ptr = nullptr;\
	}

#define SAFE_DELETE_ARRAY(ptr)\
	if (ptr != nullptr)\
	{\
		delete[] ptr;\
		ptr = nullptr;\
	}

#define SAFE_RELEASE(ptr)\
	if (ptr != nullptr)\
	{\
		ptr->Release();\
		ptr = nullptr;\
	}

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

#define WSTRING_TO_STRING(str, wsz)\
	std::wstring _w##str(wsz);\
	std::string str;\
	str.assign(_w##str.begin(), _w##str.end());

#define TOGGLE_ENUM(value, Type) value = static_cast<Type>(!(static_cast<UINT>(value)))