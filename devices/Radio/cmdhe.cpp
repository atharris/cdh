#include <iostream>
#include <fstream>
#include <cstring>
#include <utility>
#include <vector>
#include <string>

#include "packetgen.h"
#include "serial.h"

using namespace std;

int parsemessagetype(string mtype);

int preparesourcestream( string ifilename, ifstream& source);

int main(int argc, char* argv[]){

   string payloadfile;
   string messagetype;
   string portfilename;

   //find -i and -t options, use arguments accordingly
   for(int i=0; i<argc; i++)
   {
      if( strcmp(argv[i], "-p") == 0)
         portfilename = argv[i+1];
      if( strcmp(argv[i], "-i") == 0)
         payloadfile = argv[i+1];
      if( strcmp(argv[i], "-t") == 0)
         messagetype = argv[i+1];
   }

   int msgtype;
   bool loopflag = false;
   msgtype = parsemessagetype(messagetype);
   if(msgtype == -1){ //Uh, somethin's broke. Probably the user.
      return(1);
   } else if (msgtype == -2){ // Wait for transmit commands on stdin mode.
      msgtype = TRANSMIT_DATA;
      loopflag = true;
   }

   int fd_target;
   fd_target = initPort(portfilename.c_str(),O_WRONLY);
   if(fd_target < 0){ //initPort must have failed
      cerr << "Bad port file descriptor.  Giving up.\n";
      return(1);
   }

   int result;
   ifstream source;
   string shortcmd;
   typedef pair<string,string> action;
   vector<action> action_table; //member.first is a command name, member.second is the reply to send
   //replace this with a config file parser
   action_table.push_back( make_pair("txid","txidreply") );
   action_table.push_back( make_pair("led0","led0reply") );
   action_table.push_back( make_pair("led1","led1reply") );

   switch(msgtype){
   case NO_OP_COMMAND:
   case GET_TRANSCEIVER_CONFIG:
   case TELEMETRY_QUERY:
   case RESET_SYSTEM:
      send_command(msgtype, fd_target);
      break;
   case TRANSMIT_DATA:
      while(loopflag){
         getline(cin,shortcmd);
         if(shortcmd.length() > 4){
            cerr << "command string too long: \"" << shortcmd << "\"\n";
         } else if(shortcmd.length() < 4){
            cerr << "command string too short: \"" << shortcmd << "\"\n";
         }
         else{
            for(int i=0; i< action_table.size(); i++){
               if( action_table[i].first == shortcmd ){
                  //This entry in the action table must be the one.
                  //Send its reply text
                  payloadfile = action_table[i].second;
                  source.open(payloadfile.c_str(), ios::in);
                  if(source.good() ){
                     send_data(source, fd_target);
                  } else {
                     cerr << "Failed to open reply text file ";
                     cerr << payloadfile << endl;
                  }
                  source.close();
               }
            }
         }
      }
      if(!loopflag){
         result=preparesourcestream(payloadfile, source);
         if(result != 0){ //that would mean the source stream failed
            return(1);
         }
         send_data(source, fd_target);
         source.close();
      }
      break;
   case SET_TRANSCEIVER_CONFIG:
      result=preparesourcestream(payloadfile, source);
      if(result != 0){ //that would mean the source stream failed
         return(1);
      }
      send_config(source, fd_target);
      source.close();
      break;
   case WRITE_FLASH:
      cerr << "Command type not yet implemented.  Giving up.\n";
      return(1);
      break;
   default:
      cerr << "Bad msgtype (" << msgtype << ") from parser.  Committing suicide.\n";
      return(1);
   }
   close(fd_target);
   return(0);
}

int parsemessagetype(string mtype){
   if(mtype.length()>1){
      cerr << "Invalid message type.  Valid options are [ntcmRSF]\n";
      return(-1);
   } else if(mtype.length() == 0){
      cerr << "No message type on command line. Using interactive transmit mode\n";
      return(-2);
   }

   char mt = mtype.c_str()[0];
   switch(mt){
   case 'n': return(NO_OP_COMMAND);
             break; //heh, like it matters.  Yay habits.
   case 'R': return(RESET_SYSTEM);
             break;
   case 't': return(TRANSMIT_DATA);
             break;
   case 'c': return(GET_TRANSCEIVER_CONFIG);
             break;
   case 'S': return(SET_TRANSCEIVER_CONFIG);
             break;
   case 'm': return(TELEMETRY_QUERY);
             break;
   case 'F': return(WRITE_FLASH);
             break;
   default:
             cerr << "Invalid message type.  Valid options are [ntcmRSF]\n";
             return(-1);
   }
}

int preparesourcestream( string ifilename, ifstream& source){

   if( ifilename.compare("") == 0)
   {
      cerr << "No \"-i inputfilename\" given, using stdin\n";
      cerr << "Enter payload, ^D to end: ";
      ifilename = "/dev/stdin";
   }

   source.open(ifilename.c_str(), ios::in);

   if(source.fail())
   {
      cerr << "Failed to open source file\n";
      return(-1);
   }
   else return(0);
}
