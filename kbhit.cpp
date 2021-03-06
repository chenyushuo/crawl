#include "kbhit.h"

#include <ctime>

void InitKeyboard(){
    tcgetattr(0,&initial_settings);
    new_settings = initial_settings;
    new_settings.c_lflag &= ~ICANON;
    new_settings.c_lflag &= ~ECHO;
    new_settings.c_lflag &= ~ISIG;
    new_settings.c_cc[VMIN] = 1;
    new_settings.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &new_settings);
}
void CloseKeyboard(){
    tcsetattr(0, TCSANOW, &initial_settings);
}

static int kbhit(){
    char ch;
    int nread;
    if(peek_character != -1)
        return 1;
    new_settings.c_cc[VMIN] = 0;
    tcsetattr(0, TCSANOW, &new_settings);
    nread = read(0, &ch, 1);
    new_settings.c_cc[VMIN] = 1;
    tcsetattr(0, TCSANOW, &new_settings);
	if(nread == 1) {
		peek_character = ch;
		return 1;
	}
	return 0;
}

static int readch(){
    char ch;
    int nread = -1;
    if(peek_character != -1){
        ch = peek_character;
        peek_character = -1;
        for (;nread != 0;){
        	new_settings.c_cc[VMIN] = 0;
			tcsetattr(0, TCSANOW, &new_settings);
			nread = read(0, &ch, 1);
			new_settings.c_cc[VMIN] = 1;
			tcsetattr(0, TCSANOW, &new_settings);
        }
        return ch;
    }
    //nread = read(0,&ch,1);
    for (;nread != 0;){
    	new_settings.c_cc[VMIN] = 0;
		tcsetattr(0, TCSANOW, &new_settings);
		nread = read(0, &ch, 1);
		new_settings.c_cc[VMIN] = 1;
		tcsetattr(0, TCSANOW, &new_settings);
    }
    return ch;
}

void CheckKeyboard(){
	if (kbhit()){
		char ch = readch();
		if (ch == 'p'){
			fprintf(stderr, "\033[31m");
			fprintf(stderr, "press p to continue, press q to quit\n");
			fprintf(stderr, "\033[37m");
			ch = '\0';
			while (ch != 'p' && ch != 'q'){
				usleep(CLOCKS_PER_SEC * 0.05);
				if (kbhit())
					ch = readch();
			}
			if (ch == 'q'){
				CloseKeyboard();
				exit(0);
			}
		}
	}
}
