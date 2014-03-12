#include <iostream>
#include <cctype>   // for isalpha(), isalnum(), isdigit()                                                                     

#include "Lexer.h"
using namespace std; // BAD practice                                                                                           

/**                                                                                                                            
 * -----------------------------------------------------------------------------                                               
 * the constructor for Lexer, takes a string (defaulted to be empty)                                                           
 * -----------------------------------------------------------------------------                                               
 */
Lexer::Lexer(string str)
  : input_str(str), cur_pos(0)
{
  // under C++11, there's a better way to do this                                                                            
  separators.insert(' ');
  separators.insert('\t');
  separators.insert('\r');
  separators.insert('\n');
}

/**                                                                                                                            
 * -----------------------------------------------------------------------------                                               
 *  scan and return the next token                                                                                             
 *  cur_pos then points to one position right past the token                                                                   
 *  the token type is set to ERRTOK on error                                                                                   
 * -----------------------------------------------------------------------------                                               
 */
Token Lexer::next_token()
{
  Token ret(ENDTOK, ""); // the default token is the ENDTOK                                                                  
  size_t last;

  if (has_more_token()) { // cur_pos is at next non-separator char                                                           
    last = cur_pos++;   // hence, input_str[last] is a non-separator                                                       
    if (isdigit(input_str[last])) {
      // scanning for INTEGER                                                                                            
      move_through_digits();
      ret.type = INTEGER;
      ret.value = input_str.substr(last, cur_pos-last);
    } else if (isalpha(input_str[last]) || input_str[last] == '_') {
      // sanning for IDENT, can start with alpha or _                                                                    
      while ( cur_pos < input_str.length() &&
	      (isalnum(input_str[cur_pos]) ||
	       input_str[cur_pos] == '_') )
	cur_pos++;
      ret.type  = IDENT;
      ret.value = input_str.substr(last, cur_pos-last);
    } else {
      // unrecognized character, just an error token here                                                                
      ret.type = ERRTOK;
      ret.value = input_str[last];
    }
  } // end if (has more token())                                                                                             
  return ret;
}

/**                                                                                                                            
 * -----------------------------------------------------------------------------                                               
 *  return a vector of all tokens left                                                                                         
 *  we will not restart after tokenizing                                                                                       
 * -----------------------------------------------------------------------------                                               
 */
vector<Token> Lexer::tokenize()
{
  vector<Token> ret;
  Token tok;
  while (has_more_token()) {
    tok = next_token();
    ret.push_back(tok);
  }
  return ret;
}

/**                                                                                                                            
 * -----------------------------------------------------------------------------                                               
 *  set a new input string, restart                                                                                            
 * -----------------------------------------------------------------------------                                               
 */
void Lexer::set_input(string str)
{
  input_str = str;
  restart();
}

/**                                                                                                                            
 * -----------------------------------------------------------------------------                                               
 *  returns true if there's more token down the stream from the current pos                                                    
 *  false otherwise                                                                                                            
 *  - cur_pos is moved to the first non-separator character                                                                    
 *    or to the end of input                                                                                                   
 * -----------------------------------------------------------------------------                                               
 */
bool Lexer::has_more_token()
{
  while (cur_pos < input_str.length() &&
	 separators.find(input_str[cur_pos]) != separators.end()) {
    cur_pos++;
  }
  return (cur_pos < input_str.length());
}

/**                                                                                                                            
 * -----------------------------------------------------------------------------                                               
 *  keep increasing cur_pos until the end of input_str or a non-digit is seen                                                  
 * -----------------------------------------------------------------------------                                               
 */
void Lexer::move_through_digits()
{
  while (cur_pos < input_str.length() && isdigit(input_str[cur_pos]))
    cur_pos++;
}

/**                                                                                                                            
 * -----------------------------------------------------------------------------                                               
 *  restart from the beginning                                                                                                 
 * -----------------------------------------------------------------------------                                               
 */
void Lexer::restart()
{
  cur_pos = 0;
}
