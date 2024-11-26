#include "QRuntimeException.h"
#include "utils/StringUtil.h"
#include "core/common/Lang.h"

QRuntimeException::QRuntimeException(const char* code, const char * msg): 
	std::runtime_error(msg)
{
	this->code = code;
	this->msg = msg;
}

QRuntimeException::QRuntimeException(const char* code):
	std::runtime_error(ERR(code))
{

}

const std::string QRuntimeException::getCode() const
{
	return code;
}

const std::string QRuntimeException::getMsg() const
{
	if (!code.empty() && msg.empty()) {
		return ERR(code);
	}
	return msg;
}

