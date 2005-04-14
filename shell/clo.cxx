/*
 * This file was automaticly generated by clo++ version 0.6.4 (REL-0_6_4)
 * http://pmade.org/pjones/software/clo++/
 */

/** @file
 * This file contains the implementation of the parser class.
 * You can control the name of this source file using the
 * cxx_source_file variable.
**/

#include "clo.h"
#include <string.h>
#include <cstdlib>
#include <cctype>

namespace {
    const char const_usage[] =
"  -d, --dumpstack    dump the stack after each step\n"
"  -i, --interactive  start interactive envirment\n";

    const char const_help_comment[] =
"use the -h option for help";

    const char* expand_long_name (const std::string &name);
}

//#########################################################################
clo::parser::parser (void)
    : state_(state_option)
{
    std::memset(&locations_, 0, sizeof(locations_));
}
//#########################################################################
void clo::parser::parse (int argc, char *argv[], bool call_finalize) {
    for (int i=1; i<argc; ++i) parse_element(argv[i], i, source_cl);
    if (call_finalize) finalize();
}
//#########################################################################
void clo::parser::finalize (void) {
    if (state_ == state_value) {
	switch (openum_) {
	    case option_dumpstack:
		throw option_error("missing value for 'dumpstack' option");
	    case option_interactive:
		throw option_error("missing value for 'interactive' option");
	}

    }

}
//#########################################################################
void clo::parser::parse_element (const char *element, int position, opsource source) {
    std::size_t length = std::strlen(element);

    switch (state_) {
	case state_consume:
	    non_options_.push_back(element);
	    break;
	case state_option:
	    if (length >= 2 && element[0] == '-' && element[1] == '-') {
		if (length == 2) { state_ = state_consume; return; }
		element += 2;
		const char *value = element;
		while (*value != 0 && *value != '=') ++value;
		if (*value == '=') {
		    std::string selement(element, value - element), svalue(++value);

		    parse_long_option(selement.c_str(), position, source);
		    if (state_ != state_value) {
			std::string error("the '"); error += element; error += "' option does not take a value";
			throw option_error(error);
		    }

		    parse_value(svalue.c_str());
		    state_ = state_option;
		} else {
		    parse_long_option(element, position, source);
		}
	    } else if (length >= 2 && element[0] == '-') {
		++element;

		if (length > 2) {
		    while (*element != 0) {
			parse_short_option(*element, position, source);
			++element;
	    
			if (state_ == state_value && *element != 0) {
			    parse_value(element);
			    state_ = state_option;
			    break;
			}
		    }
		} else {
		    parse_short_option(*element, position, source);
		}
	    } else {
		non_options_.push_back(element);
	    }
	    break;
	case state_value:
	    parse_value(element);
	    state_ = state_option;
	    break;
    }
}
//#########################################################################
void clo::parser::parse_short_option (char option, int position, opsource source) {
    switch (option) {
    	case 'd':
    	    source = source; // kill compiler unused variable warning
    	    openum_ = option_dumpstack;
    	    options_.dumpstack = !options_.dumpstack;
    	    locations_.dumpstack = position;
    	    return;
    	case 'i':
    	    source = source; // kill compiler unused variable warning
    	    openum_ = option_interactive;
    	    options_.interactive = !options_.interactive;
    	    locations_.interactive = position;
    	    return;
    	case 'h':
    	    if (source != source_cl) break;
    	    throw autoexcept(autothrow_help, const_usage);
    	case '?':
    	    if (source != source_cl) break;
    	    throw autoexcept(autothrow_help, const_usage);
    }

    std::string error("unknown option: '"); error += option; error += "'";
    throw option_error(error);
}
//#########################################################################
void clo::parser::parse_long_option (const char *option, int position, opsource source) {
    option = expand_long_name(option);

	    if (std::strcmp(option, "dumpstack") == 0) {
		source = source; // kill compiler unused variable warning
		openum_ = option_dumpstack;
		locations_.dumpstack = position;
		options_.dumpstack = !options_.dumpstack;
		return;
	    } else if (std::strcmp(option, "interactive") == 0) {
		source = source; // kill compiler unused variable warning
		openum_ = option_interactive;
		locations_.interactive = position;
		options_.interactive = !options_.interactive;
		return;
	    } else if (source == source_cl && std::strcmp(option, "help") == 0) {
		throw autoexcept(autothrow_help, const_usage);
	    }

    std::string error("unknown option '"); error += option; error += "'";
    throw option_error(error);
}
//#########################################################################
void clo::parser::parse_value (const char *value) {
}
//#########################################################################
const char* clo::option_error::get_help_comment (void) const {
    return const_help_comment;
}
//#########################################################################
namespace {
    const char* expand_long_name (const std::string &name) {
	std::string::size_type name_size = name.size();
	std::vector<const char*> matches;

        if (name_size <= 9 && name.compare(0, name_size, "dumpstack", name_size) == 0)
        	matches.push_back("dumpstack");

        if (name_size <= 11 && name.compare(0, name_size, "interactive", name_size) == 0)
        	matches.push_back("interactive");

        if (name_size <= 4 && name.compare(0, name_size, "help", name_size) == 0)
        	matches.push_back("help");


	if (matches.empty()) {
	    std::string error("unknown option '"); error += name; error += "'";
	    throw clo::option_error(error);
	}

	if (matches.size() == 1) {
	    return matches[0];
	}

	std::string error("the option name '"); error += name; error += "' is ambiguous";
	throw clo::option_error(error);
    }
} // end anonymous namespace
