#include "keyboard.h"
#include "servo.h"
#include "uart.h"

////////////////////////////////////////////////////
char cOdebranyZnak;
int main (){
 enum eRecieverStatus x ;
		 char tab[5];
	while(1){
	
		
		Reciever_PutCharacterToBuffer ('k');
		Reciever_PutCharacterToBuffer ('o');
		Reciever_PutCharacterToBuffer ('d');
		Reciever_PutCharacterToBuffer ('\0');

		x = eReciever_GetStatus();
		Reciever_GetStringCopy(tab);			
		
		
		Reciever_PutCharacterToBuffer ('k');
		Reciever_PutCharacterToBuffer ('o');
		Reciever_PutCharacterToBuffer ('d');
		Reciever_PutCharacterToBuffer ('1');
		Reciever_PutCharacterToBuffer ('\0');

		x = eReciever_GetStatus();
		Reciever_GetStringCopy(tab);		
		

		}
	}

