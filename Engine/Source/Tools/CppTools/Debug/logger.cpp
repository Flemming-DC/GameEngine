#include <string>
#include "logger.h"
#include "ErrorChecker.h" 
#include "StringTools.h"

namespace logger
{

	
	std::string TryCallToString(const std::type_info& type_, ...)
	{
		RaiseError(type_.name(), " has no to_string() method, nor does there exist a logger::to_string() overload with ", 
			type_.name(), " as an input.");
	}
}




