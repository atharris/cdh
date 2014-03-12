// ============================================================================                                                
// Lexer.h                                                                                                                     
// ~~~~~~~                                                                                                                     
// Hung Q. Ngo                                                                                                                 
// - a simple lexical analyzer                                                                                                 
// - 2 types of tokens are accepted: IDENT and INTEGER                                                                         
// ============================================================================                                                

#ifndef LEXER_H_
#define LEXER_H_

#include <string>
#include <vector>
#include <set>

enum token_types_t {
  IDENT,   // a sequence of alphanumeric characters and _, starting with alpha                                               
  INTEGER, // a series of digits                                                                                             
  ENDTOK,   // end of string/file, no more token                                                                             
  ERRTOK    // unrecognized token                                                                                            
};

struct Token {
  token_types_t type;
  std::string   value;
  Token(token_types_t tt=ENDTOK, std::string val="")
  : type(tt), value(val) {}
};

/**                                                                                                                            
 * -----------------------------------------------------------------------------                                               
 * the Lexer class:                                                                                                            
 * - take a string to be scanned                                                                                               
 * - scan for tokens and return one at a time                                                                                  
 * -----------------------------------------------------------------------------                                               
 */
class Lexer {
 public:
  // constructor                                                                                                             
  Lexer(std::string str="");

  // a couple of modifiers                                                                                                   
  void set_input(std::string); // set a new input,                                                                           
  void restart();              // move cursor to the beginning, restart                                                      

  Token next_token();            // returns the next token                                                                   
  std::vector<Token> tokenize(); // returns the rest of the tokens                                                           
  bool has_more_token();         // are there more token(s)?                                                                 

 private:
  std::string    input_str;  // the input string to be scanned                                                               
  size_t         cur_pos;    // current position in the input string                                                         
  std::set<char> separators; // set of separators                                                                            

  void move_through_digits(); // move cur_pos until the end or non-digit                                                     
};

#endif
