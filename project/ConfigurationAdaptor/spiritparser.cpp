/*! \file spiritparser.cpp
 * \brief The internal parser to read configurations (or sub models) from FAMILIAR
 * at runtime (selected/deselected features)
 *
 * It uses spirit/phoenix/qi C++ parsing modules
 */

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/bind.hpp>
#include <boost/fusion/adapted/std_pair.hpp>
//#include <boost/spirit/include/phoenix_object.hpp>
//#include <boost/fusion/include/adapt_struct.hpp>
//#include <boost/fusion/include/io.hpp>
//#include "boost/tuple/tuple.hpp"
//using namespace boost::phoenix::arg_names;

#include <vector>
#include <utility>
#include <iostream>
#include <string>
//#include <locale>
using namespace std;
using namespace boost::phoenix;

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
using qi::char_;
using qi::lit;
using qi::lexeme;
//qi::alpha>>*qi::alnum;

using ascii::space;

// For tests
/*
void print_vector(const vector<string>& v)
{
    for(vector<string>::const_iterator it= v.begin(); it != v.end();++it)
	cerr << *it << ' ';
    cerr << endl;
}
void print_result(const pair<vector<string>, vector<string> >& res)
{
	cerr << "selected : ";
	print_vector(res.first);

	cerr << "deselected : ";
        print_vector(res.second);
}
*/

// The configuration parser, a structure
template <typename Iterator>
struct config_parser : qi::grammar<Iterator,pair<vector<string>,vector<string> >(), ascii::space_type>
{
    config_parser() : config_parser::base_type(start)
    {
	// The grammar
        start %= boost::spirit::omit[ident] >> ':'
            >> lit("(CONFIGURATION)")
	    >> lit("selected:") >>
            '[' >> list_ident >> ']' //[&print_vector]
	    >> lit("deselected:")
            >> '[' >> list_ident >> ']'
            ;

	list_ident = -(ident %',') ;

	ident%= lexeme[+char_("a-zA-Z_0-9")];
    }
    // Spirit rule definitions
    qi::rule<Iterator,pair<vector<string>, vector<string> >(),
	ascii::space_type> start; // return value is a pair of vectors
    qi::rule<Iterator,vector<string>(),ascii::space_type> list_ident;
    // return value is  a vector of strings
    qi::rule<Iterator,string(),ascii::space_type> ident; // return value is a string
};


class Not_Parsed {}; // Exception

// Decode a configuration received from FAMILIAR (complete(config) or not(=sub model))
pair<vector<string>, vector<string> >
receive_from_Familiar(string input)
{
    config_parser<string::iterator> cp; // create a parser

    pair<vector<string>, vector<string> > result;
    // Spirit will fill the 2 vectors with the 2 list_ident in the grammar
    // thanks to the return values of the rules

    bool status =  qi::phrase_parse(input.begin(), input.end(), cp, space, result);
    if (status)
    {
	cerr << "Configuration parsed successfully" << endl;
	//print_result(result);
	return result;
    }

    else cout << "Parse error in configuration" << endl; throw Not_Parsed();
  }


/*
int main()
{
    locale loc;
    cout << loc.name() << endl;

    pair<vector<string>, vector<string> > result;

    string input = "toto : (CONFIGURATION) selected: [ ah bk1 ] deselected: [ c b_2 ]";

    config_parser<string::iterator> cp;

    bool status =  qi::phrase_parse(input.begin(), input.end(), cp, space, result);
    if (status)
    {
	cout << "OK" << endl;
	print_result(result);
    }

    else cout << "NO" << endl;
    return 0;
  }
*/
