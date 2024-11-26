#pragma once
#include <stdexcept>
#include <string>
class QRuntimeException : public std::runtime_error
{
public:
	/**
     * @brief Encapsulation of the error message from application on std::runtime_error.
     *
     * @param[in] code The string message describing the runtime error
     * @param[in] msg           Return value from function call that failed.
     */
    QRuntimeException(const char* code, const char * msg);

	/**
     * @brief Encapsulation of the error message from application on std::runtime_error.
     *
     * @param[in] code The string message describing the runtime error
     * @param[in] msg           Return value from function call that failed.
     */
	QRuntimeException(const std::string code, const std::string msg) :
		QRuntimeException(code.c_str(), msg.c_str()) {};

	QRuntimeException(const char* code);
	QRuntimeException(const std::string code)
		:QRuntimeException(code.c_str()) {};

	const std::string getCode() const;
	const std::string getMsg() const;
private:
	std::string code;
	std::string msg;
};