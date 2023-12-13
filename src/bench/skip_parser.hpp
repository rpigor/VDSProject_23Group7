/*=============================================================================
    Written by Carolina P. Nogueira (2016)
=============================================================================*/
#pragma once
#include <boost/spirit/include/qi.hpp>

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

namespace skip_p
{
	/* Skip parser definition */
	template <typename Iterator>
	struct skip_grammar : qi::grammar<Iterator>
	{
		skip_grammar() : skip_grammar::base_type(skip_, "Skip Grammar")
		{
			using qi::eol;
			using qi::lit;
			using namespace ascii;
			using qi::blank;
			
			/* Skip all spaces and comments */
            skip_ = line_comment_ | blank;
            
            /* comments (i.e. # everything else should be ignored) */
            line_comment_ = lit("#") >> *(char_ - eol) >> +eol;

		}
        /*
         *
         * Declaring alle rules used in this parser
         * The prototype is:
         * qi::rule<Iteratortype, returntype(parametertypes), qi::local<local_variables>, skip_parser_type> rule_name;
         *
         */
		qi::rule<Iterator> skip_;
		qi::rule<Iterator> line_comment_;
	};
}

