#include <string>
#include "logger.h"
#include "ErrorChecker.h" 
#include "StringTools.h"

namespace logger
{

	
	std::string TryCallToString(const std::type_info& type_, ...)
	{
		RaiseError(type_.name(), " has no 'std::string to_string() const' method, nor does there exist a ", 
			"'std::string logger::to_string()' overload with ", type_.name(), " as an input.");
		return "";
	}
}




