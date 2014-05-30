#include "Lexer.h"
#include <iostream>
#include <string>


void read_temp(Lexer lexer);
void read_eps(Lexer lexer);
void read_magnetometer();
void read_gyro();
void read_gps();
void send_file(Lexer lexer);
void get_file(Lexer lexer);
void command_magnetorquer(Lexer lexer);
void command_rxn(Lexer lexer);
void get_telemetry();
void determine_constellation();	


int main(){

  Lexer lexer;
  Token tok;
  std::string line;

  while(std::cin){
    std::cout << "Enter FlatSat Command :> ";
    getline(std::cin,line);
    lexer.set_input(line);

    if(lexer.has_more_token()){
      tok = lexer.next_token();
      
      if(tok.value.compare("temp")==0){
	read_temp(lexer);
	continue;
      }

      if(tok.value.compare("eps")==0){
	read_eps(lexer);
	continue;
      }

      if(tok.value.compare("magnetometer")==0){
	read_magnetometer();
	continue;
      }

      if(tok.value.compare("gyro")==0){
	read_gyro();
	continue;
      }

      if(tok.value.compare("gps")==0){
	read_gps();
	continue;
      }

      if(tok.value.compare("send")==0){
	send_file(lexer);
	continue;
      }

      if(tok.value.compare("get")==0){
	get_file(lexer);
	continue;
      }

      if(tok.value.compare("magnetorquer")==0){
	command_magnetorquer(lexer);
	continue;
      }

      if(tok.value.compare("rxn")==0){
	command_rxn(lexer);
	continue;
      }

      if(tok.value.compare("telem")==0){
	get_telemetry();
	continue;
      }

      if(tok.value.compare("constellation")==0){
	determine_constellation();
	continue;
      }
    }
  }


}


void read_temp(Lexer lexer){

  if(!lexer.has_more_token()){
    std::cout << "temp command entered, need temp # to read from" << std::endl;
  }
  else{
    Token tok = lexer.next_token();
    if(tok.type != INTEGER)
      std::cout << "Need a number corresponding to an rtd to measure" << std::endl;
  }

}


void read_eps(Lexer lexer){

}


void read_magnetometer(){

}


void read_gyro(){

}


void read_gps(){

}


void send_file(Lexer lexer){

}


void get_file(Lexer lexer){

}


void command_magnetorquer(Lexer lexer){

}


void command_rxn(Lexer lexer){

}


void get_telemetry(){

}


void determine_constellation(){

}
