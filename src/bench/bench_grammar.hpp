/*=============================================================================
    Written by Carolina P. Nogueira (2016)
=============================================================================*/
#pragma once

#include <boost/phoenix.hpp>
#include "skip_parser.hpp"

namespace bench_format
{   
    struct bench_node_type
    {
        std::string label;                        // Unique ID for a node
        std::string gate_type;                    // Type of the gate (ex. AND, NOT, OR)
        std::vector<std::string> input_node_list; // list containing all the inputs of the respective gate
    };
}

/* Let fusion know about the structure */
BOOST_FUSION_ADAPT_STRUCT(
    bench_format::bench_node_type,
    (std::string, label)
    (std::string, gate_type)
    (std::vector<std::string>, input_node_list)
)     
    
namespace bench_format
{     
    /*
     *
     *  bench_format parser
     *
     */
    namespace qi = boost::spirit::qi;
    namespace ascii = boost::spirit::ascii;
    namespace phx = boost::phoenix;

    typedef skip_p::skip_grammar<boost::spirit::istream_iterator> const_skipper;
           
    template <typename Iterator>
    struct bench_parser : qi::grammar<Iterator, bench_format::bench_node_type(), const_skipper>
    {
        /* Node */
        bench_format::bench_node_type bench_node;
                
        bench_parser() : bench_parser::base_type(start,"bench_grammar")
        {
            using qi::_1;
            using qi::_val;
            using phx::at_c;
        
            using qi::lit;
            using ascii::char_;
            using qi::lexeme;
            using qi::eol;
            using qi::string;
            using qi::hold;

            /* Node IDs */
            lhs_id %= lexeme[+(char_("a-zA-Z_0-9.") - '=')  ] >> lit("=");
            rhs_id %= lit("(") >> lexeme[+(char_("a-zA-Z_0-9.") - ','- ')') ] >> !lit(',') >> lit(")");

            /* Input Operators */
            single_op   %= lit("(") >> lexeme[+(char_("a-zA-Z_0-9.") - ','- ')' ) ] >> !lit(',') >> lit(")");
            multiple_op %= lit("(") >> +(lexeme[+(char_("a-zA-Z_0-9.") - ',') ] >> lit(",")) >> lexeme[+(char_("a-zA-Z_0-9.") - ')')] >> lit(")");
            
            /* Gate Types */
            single_input_gate_t   %= string("NOT") | string("BUFF") | string("DFF") ;
            multiple_input_gate_t %= string("AND") | string("OR") | string("NAND")| string("NOR") | string("XOR");
            
            /* Primary inputs and outputs */
            primary_input  = string("INPUT")  [bind(&bench_format::bench_node_type::gate_type, _val)=_1] >> rhs_id [bind(&bench_format::bench_node_type::label, _val)=_1] >> +eol;
            primary_output = string("OUTPUT") [bind(&bench_format::bench_node_type::gate_type, _val)=_1] >> rhs_id [bind(&bench_format::bench_node_type::label, _val)=_1] >> +eol;

            /* Gate Rules */
            single_input_gate   %= lhs_id  >> single_input_gate_t   [at_c<1>(_val) = _1] >> single_op >> +eol;
            multiple_input_gate %= lhs_id  >> multiple_input_gate_t [at_c<1>(_val) = _1] >> multiple_op >> +eol;

            /* Starting Rule */
            start %= hold[primary_input] | hold[primary_output] | hold[single_input_gate] | hold[multiple_input_gate];
        }

        /* Node IDs */
        qi::rule<Iterator, std::string(), const_skipper> lhs_id; 
        qi::rule<Iterator, std::string(), const_skipper> rhs_id ;
        
        /* Input Operators */
        qi::rule<Iterator, std::vector<std::string>(), const_skipper> single_op;
        qi::rule<Iterator, std::vector<std::string>(), const_skipper> multiple_op;
        
        /* Gate Types */
        qi::rule<Iterator, std::string(), const_skipper> single_input_gate_t;
        qi::rule<Iterator, std::string(), const_skipper> multiple_input_gate_t;

        /* Primary inputs and outputs */        
        qi::rule<Iterator, bench_format::bench_node_type(), const_skipper> primary_input;
        qi::rule<Iterator, bench_format::bench_node_type(), const_skipper> primary_output;
        qi::rule<Iterator, bench_format::bench_node_type(), const_skipper> single_input_gate;
        qi::rule<Iterator, bench_format::bench_node_type(), const_skipper> multiple_input_gate;

        /* Starting Rule */
        qi::rule<Iterator, bench_format::bench_node_type(), const_skipper> start;      
    };
}
