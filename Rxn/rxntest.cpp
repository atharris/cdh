#include "Rxn.h"
#include "Lexer.h"

int main(int argc, char** argv){
  
  char* path = argv[1];
  Rxn* rxn = new Rxn(path);
  rxn->start_telemetry();
  std::string line;
  Lexer lexer;
  Token tok;
  while(std::cin){
    std::cout << "> ";
    getline(std::cin,line);
    lexer.set_input(line);
    if(!lexer.has_more_token())
      continue;
    tok = lexer.next_token();
    if(tok.value.compare("telem")==0){
      rxn->print_telemetry();
    }
    if(tok.value.compare("xspeed")==0){
      tok = lexer.next_token();
      if(tok.type != INTEGER)
	std::cerr << "Need for speed.";
      else{
	uint16_t speed = atoi(tok.value.c_str());
	rxn->write_x_speed(speed);
      }
    }
    if(tok.value.compare("xclear")==0){
      rxn->clear_x();
    }


  }
  

  return 0;

}

