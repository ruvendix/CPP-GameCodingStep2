#pragma once

#define GENERATE_SINGLETON(Type)\
public:\
	static std::shared_ptr<Type> GetInstance()\
	{\
		static std::shared_ptr<Type> spInst = std::make_shared<Type>();\
		return spInst;\
	}

#define GENERATE_SMART_POINTER(Type) using SPtr = std::shared_ptr<Type>

#define SINGLETON(Type) Type::GetInstance()

#define COM_ERROR(errCode)\
{\
	std::string strLog;\
	SINGLETON(ErrorHandler)->ConvertComErrorString(errCode, strLog);\
    ::MessageBox(nullptr, strLog.c_str(), "COM ¿À·ù", MB_ICONERROR);\
}

#define CUSTOM_ERROR(strErr) SINGLETON(ErrorHandler)->ShowCustomError(strErr)

#define ASSERT(expr) assert(expr)

#define TEST_COM(expr, hRet)\
	hRet = expr;\
	if (FAILED(hRet))\
	{\
		COM_ERROR(hRet);\
	}

#define BIND_INPUT_LISTENER(virtualKeyCode, inputState, listener) InputEvent{ virtualKeyCode, inputState }, std::bind(&listener, this)