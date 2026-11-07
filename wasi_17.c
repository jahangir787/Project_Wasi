#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <sys/ioctl.h>
#include <dirent.h>

#define BUFFER_MAX 4

char buffer[BUFFER_MAX] = {0};
int buffer_size = 0;

void tap_key(int fd, int keycode) ;

void init_buffer() {
	/*
		replace all the values in the buffer array with null character,i.e, int val zero
	*/
    for(int i = 0; i < BUFFER_MAX; i++) {
        buffer[i] = 0;
    }
    buffer_size = 0; // this means no data in the buffer right now
}

void enqueue(char ch) {
	int i = 0;
	if(buffer_size >= BUFFER_MAX){// the buffer is full
		// shift all the characters in the buffer by 1 place to the left, which will truncate the oldest char in the buffer, and make an empty place
		for(i=0; i < BUFFER_MAX-1; i++){
			buffer[i] = buffer[i+1];		
		}
		// make the last position in the buffer empty
		buffer[i] = 0; // assigned null character; int val of null character is zero
		// decrease buffer_size by 1
		buffer_size = buffer_size - 1;
	}// end if(buffer_size >= (BUFFER_MAX-1))
	// insert the last data pressed in the buffer
	buffer[buffer_size] = ch; // current position is the present last position in the buffer
	// after the insertion, increase buffer_size by 1
	buffer_size = buffer_size + 1;
}// end function enqueue
/*
void dequeue(){
	int i = 0;
	printf("\ndequeue() function called.\n");
	if(buffer_size >= 8){
		for(i=0; i<4; i++){
			buffer[i] = buffer[i+3];
		}
		buffer_size = i;
		for(; i<= BUFFER_MAX; i ++){
			buffer[i] = '\0';
		}
	}
}
*/

void print_buffer(){
	int i=0;
	printf("\n\nbuffer_size = %d\n\n", buffer_size);
	for(i = 0; i < buffer_size; i++){
		printf("buffer[%d]=%c | buffer[%d]=%d\n", i, buffer[i], i, buffer[i]);
	}printf("\n\n");
}

char pick_last() {
    if (buffer_size <= 0) return 0;
    int last_position = buffer_size - 1;
    return buffer[last_position];
}

// Forward declaration for helper
void tap_key(int fd, int keycode);

long int find_bangla_char(int fd) {
    char ch;
    long int bn_char;
    ch = pick_last();
    printf("\nLast character picked: char = %c, int = %d\n", ch, ch);
    switch(ch){
        case '0': bn_char = 0x9e6; init_buffer(); break;
        case '1': bn_char = 0x9e7; init_buffer(); break;
        case '2': bn_char = 0x9e8; init_buffer(); break;
        case '3': bn_char = 0x9e9; init_buffer(); break;
        case '4': bn_char = 0x9ea; init_buffer(); break;
        case '5': bn_char = 0x9eb; init_buffer(); break;
        case '6': bn_char = 0x9ec; init_buffer(); break;
        case '7': bn_char = 0x9ed; init_buffer(); break;
        case '8': bn_char = 0x9ee; init_buffer(); break;
        case '9': bn_char = 0x9ef; init_buffer(); break;
        case 'a':
            if (buffer_size == 1) bn_char = 0x0986; // আ
            else bn_char = 0x09be; // া
            break;
        case 'b': bn_char = 0x09ac; break; // ব
        case 'c': bn_char = 0x099a; break; // চ
        case 'd': bn_char = 0x09a6; break; // দ
        case 'e':
            if (buffer_size == 1) bn_char = 0x098f; // এ
            else bn_char = 0x09c7; // ে
            break;
        case 'f': bn_char = 0x09ab; break; // ফ
        case 'g':
            if (buffer_size >= 2 && buffer[buffer_size-2] == 'N') {
                bn_char = 0x0999;
				buffer[buffer_size-1] = '\0';
				tap_key(fd, KEY_BACKSPACE); // deletes the previous one glyph				
				buffer_size--;
				break;
            } else if (buffer_size >= 2 && buffer[buffer_size-2] == 'n') {
                bn_char = 0x0982; buffer[buffer_size-1] = '\0'; buffer_size--; 
				tap_key(fd, KEY_BACKSPACE); // deletes the previous one glyph				
				break;
            } else { bn_char = 0x0997; break; }
        case 'h':
            if (buffer_size >= 2 && buffer[buffer_size-2] == 'k') {
                bn_char = 0x0996;
				buffer[buffer_size-1] = '\0';
				buffer_size--;
				tap_key(fd, KEY_BACKSPACE); // deletes the previous one glyph				
				break;
            } else if (buffer_size >= 2 && buffer[buffer_size-2] == 'g') {
                bn_char = 0x0998; buffer[buffer_size-1] = '\0'; buffer_size--; 
				tap_key(fd, KEY_BACKSPACE); // deletes the previous one glyph							
				break;
            } else if (buffer_size >= 2 && buffer[buffer_size-2] == 'c') {
                bn_char = 0x099b; buffer[buffer_size-1] = '\0'; buffer_size--;
				tap_key(fd, KEY_BACKSPACE); // deletes the previous one glyph								
				break;
            } else if (buffer_size >= 2 && buffer[buffer_size-2] == 'j') {
                bn_char = 0x099d; buffer[buffer_size-1] = '\0'; buffer_size--; 
				tap_key(fd, KEY_BACKSPACE);
				break;
            } else if (buffer_size >= 2 && buffer[buffer_size-2] == 'T') {
                bn_char = 0x09a0; buffer[buffer_size-1] = '\0'; buffer_size--; 
				tap_key(fd, KEY_BACKSPACE);			
				break;
            } else if (buffer_size >= 2 && buffer[buffer_size-2] == 'D') {
                bn_char = 0x09a2; buffer[buffer_size-1] = '\0'; buffer_size--; 
				tap_key(fd, KEY_BACKSPACE);					
				break;
            } else if (buffer_size >= 2 && buffer[buffer_size-2] == 't') {
                bn_char = 0x09a5; buffer[buffer_size-1] = '\0'; buffer_size--; 
				tap_key(fd, KEY_BACKSPACE);
				break;
            } else if (buffer_size >= 2 && buffer[buffer_size-2] == 'd') {
                bn_char = 0x09a7; buffer[buffer_size-1] = '\0'; buffer_size--; 
				tap_key(fd, KEY_BACKSPACE);	
				break;
            } else if (buffer_size >= 2 && buffer[buffer_size-2] == 'S') {
                bn_char = 0x09b7; buffer[buffer_size-1] = '\0'; buffer_size--; 
				tap_key(fd, KEY_BACKSPACE);		
				break;
            } else if (buffer_size >= 2 && buffer[buffer_size-2] == 'R') {
                bn_char = 0x09dd; buffer[buffer_size-1] = '\0'; buffer_size--;
				tap_key(fd, KEY_BACKSPACE);
				 break;
            } else { bn_char = 0x09b9; break; }
        case 'i':
            if (buffer_size >= 3 && buffer[buffer_size-2] == 'r' && buffer[buffer_size-3] == 'r'){
                if (buffer_size == 3) {
                    bn_char = 0x098b; // ঋ (sorborno ri at beginning of word)
                } else {
                    bn_char = 0x09c3; // ৃ (ri-kar not at beginning of word)
                }
                // Delete both 'ra' characters printed by the 'rr' sequence
                tap_key(fd, KEY_BACKSPACE);
                tap_key(fd, KEY_BACKSPACE);
                init_buffer();
                break;
            } else if (buffer_size >= 3 && buffer[buffer_size-2] == 'r' && buffer[buffer_size-3] == 'R'){
                bn_char = 0x09dd; buffer[buffer_size-1] = '\0'; buffer_size-=2; break;
            } else if (buffer_size >= 2 && buffer[buffer_size-2] == 'o'){
                bn_char = 0x09c8; buffer[buffer_size-1] = '\0'; buffer_size--; break;
            } else if (buffer_size == 2 && buffer[buffer_size-2] == 'o'){
                bn_char = 0x0990; buffer[buffer_size-1] = '\0'; buffer_size--; break;
            } else if (buffer_size >= 2){ bn_char = 0x09bf; break;
            } else { bn_char = 0x0987; break; }
        case 'j': bn_char = 0x099c; break;
        case 'k': bn_char = 0x0995; break;
        case 'l': bn_char = 0x09b2; break;
        case 'm': bn_char = 0x09ae; break;
        case 'n': bn_char = 0x09a8; break;
        case 'o': bn_char = 0x0985; break;
        case 'p': bn_char = 0x09aa; break;
        case 'r': bn_char = 0x09b0; break; // র
        case 's': bn_char = 0x09b8; break;
        case 't': bn_char = 0x09a4; break;
        case 'u':
            if (buffer_size >= 2){ bn_char = 0x09c1; break; }
            else { bn_char = 0x0989; break; }
        case 'v': bn_char = 0x09ad; break;
        case 'y': bn_char = 0x09df; break;
        case 'z': bn_char = 0x09af; break;
        case 'D': bn_char = 0x09a1; break;
        case 'G':
            if (buffer_size >= 2 && buffer[buffer_size-2] == 'N'){
                bn_char = 0x099e; break;
            } else { bn_char = 'G'; init_buffer(); break; }
        case 'H': bn_char = 0x09cd; break;
        case 'I':
            if (buffer_size >= 2){ bn_char = 0x09c0; break; }
            else { bn_char = 0x0988; break; }
        case 'N': bn_char = 0x09a3; break;
        case 'O':
            if (buffer_size >= 2){ bn_char = 0x09cb; break; }
            else{ bn_char = 0x0993; break; }
        case 'R': bn_char = 0x09dc; break;
        case 'S': bn_char = 0x09b6; break;
        case 'T': bn_char = 0x099f; break;
        case 'U':
            if (buffer_size >= 2){ 
					bn_char = 0x09c2;
					break;
			}else{ 
							bn_char = 0x098a; 
							break;
			}
        case ':': bn_char = 0x0983; break;
        case '^': bn_char = 0x0981; break;
        case '`':
            if (buffer_size >= 2 && buffer[buffer_size-2] == 't'){
                bn_char = 0x09ce; 
				buffer[buffer_size-1] = '\0';
				buffer_size--; 
				tap_key(fd, KEY_BACKSPACE); // deletes the previous one glyph				
				break;
            } else { goto default_case; }
		case '.':
			if(buffer_size>=3 && buffer[buffer_size-2]=='.' && buffer[buffer_size-3]=='.'){
				bn_char = '.';
				//tap_key(fd, KEY_BACKSPACE);
				tap_key(fd, KEY_BACKSPACE);
				init_buffer();
				break;			
			}else if(buffer_size>=2 && buffer[buffer_size-2]=='.'){
				bn_char = 0x0965; // devanagari double danda
				tap_key(fd, KEY_BACKSPACE);
				//init_buffer();
				break;
			}else{
				bn_char = 0x0964; // devanagari danda
				//init_buffer();
				break;
			}
        default:
            default_case:
			printf("entered the default_code.\n");
            bn_char = ch;
            init_buffer();
			printf("init_buffer() finished execution.\n");
			printf("after init_buffer() called: buffer_size = %d\n", buffer_size);
    }
	//dequeue();
	/* code for debug*/
	printf("The value returned: decimal= %ld, hex=0x%lx\n", bn_char, bn_char);
	printf("after detecting bangla key, buffer_size = %d", buffer_size);
	// END CODE for debug
    return bn_char;
}

void emit(int fd, int type, int code, int val) {
    struct input_event ev;
    memset(&ev, 0, sizeof(ev));
    ev.type = type;
    ev.code = code;
    ev.value = val;
    write(fd, &ev, sizeof(ev));
}

void tap_key(int fd, int keycode) {
    emit(fd, EV_KEY, keycode, 1);
    emit(fd, SYN_REPORT, 0, 0);
    usleep(10000);
    emit(fd, EV_KEY, keycode, 0);
    emit(fd, SYN_REPORT, 0, 0);
    usleep(10000);
}

void inject_unicode(int fd, long int codepoint) {
    char utf8_buf[5] = {0};

    // 1. Convert Unicode codepoint to raw UTF-8 byte sequence
    if (codepoint < 0x80) {
        utf8_buf[0] = (char)codepoint;
    } else if (codepoint < 0x800) {
        utf8_buf[0] = (char)(0xC0 | (codepoint >> 6));
        utf8_buf[1] = (char)(0x80 | (codepoint & 0x3F));
    } else if (codepoint < 0x10000) {
        utf8_buf[0] = (char)(0xE0 | (codepoint >> 12));
        utf8_buf[1] = (char)(0x80 | ((codepoint >> 6) & 0x3F));
        utf8_buf[2] = (char)(0x80 | (codepoint & 0x3F));
    } else {
        utf8_buf[0] = (char)(0xF0 | (codepoint >> 18));
        utf8_buf[1] = (char)(0x80 | ((codepoint >> 12) & 0x3F));
        utf8_buf[2] = (char)(0x80 | ((codepoint >> 6) & 0x3F));
        utf8_buf[3] = (char)(0x80 | (codepoint & 0x3F));
    }

    // 2. Pipe the UTF-8 bytes into the system clipboard using xclip
    char cmd[512];
    snprintf(cmd, sizeof(cmd), "printf '%%s' '%s' | xclip -selection clipboard", utf8_buf);
    int ret = system(cmd);
    if (ret != 0) {
        return; // Fallback if xclip execution fails
    }

    // Small delay to ensure clipboard buffer accepts the data
    usleep(15000);

	// ---> CHANGE MADE HERE <---
    // TEMPORARY FIX: Release Shift so Ctrl+V doesn't become Ctrl+Shift+V (Paste Special)
    emit(fd, EV_KEY, KEY_LEFTSHIFT, 0);
    emit(fd, EV_KEY, KEY_RIGHTSHIFT, 0);
    emit(fd, SYN_REPORT, 0, 0);
    usleep(5000);
    // -------------------------

    // 3. Simulate Ctrl + V using your emit() helper and uinput file descriptor
    emit(fd, EV_KEY, KEY_LEFTCTRL, 1);
    emit(fd, EV_KEY, KEY_V, 1);
    emit(fd, SYN_REPORT, 0, 0);
    usleep(10000);

    emit(fd, EV_KEY, KEY_V, 0);
    emit(fd, EV_KEY, KEY_LEFTCTRL, 0);
    emit(fd, SYN_REPORT, 0, 0);
    
    // Cooldown delay so the input stream settles before your next keystroke
    usleep(25000);
}// end function inject_unicode

char keycode_to_char(int code, int shift) {
    char map_lower[KEY_MAX] = {0};
    map_lower[KEY_A] = 'a'; map_lower[KEY_B] = 'b'; map_lower[KEY_C] = 'c';
    map_lower[KEY_D] = 'd'; map_lower[KEY_E] = 'e'; map_lower[KEY_F] = 'f';
    map_lower[KEY_G] = 'g'; map_lower[KEY_H] = 'h'; map_lower[KEY_I] = 'i';
    map_lower[KEY_J] = 'j'; map_lower[KEY_K] = 'k'; map_lower[KEY_L] = 'l';
    map_lower[KEY_M] = 'm'; map_lower[KEY_N] = 'n'; map_lower[KEY_O] = 'o';
    map_lower[KEY_P] = 'p'; map_lower[KEY_Q] = 'q'; map_lower[KEY_R] = 'r';
    map_lower[KEY_S] = 's'; map_lower[KEY_T] = 't'; map_lower[KEY_U] = 'u';
    map_lower[KEY_V] = 'v'; map_lower[KEY_W] = 'w'; map_lower[KEY_X] = 'x';
    map_lower[KEY_Y] = 'y'; map_lower[KEY_Z] = 'z';
    map_lower[KEY_0] = '0'; map_lower[KEY_1] = '1'; map_lower[KEY_2] = '2';
    map_lower[KEY_3] = '3'; map_lower[KEY_4] = '4'; map_lower[KEY_5] = '5';
    map_lower[KEY_6] = '6'; map_lower[KEY_7] = '7'; map_lower[KEY_8] = '8';
    map_lower[KEY_9] = '9'; map_lower[KEY_SPACE] = ' ';
    map_lower[KEY_DOT] = '.'; map_lower[KEY_MINUS] = '-'; map_lower[KEY_GRAVE] = '`';
    //map_lower[KEY_SEMICOLON] = ':'; 
	map_lower[KEY_KPASTERISK] = '^';
	//map_lower[KEY_ENTER] = '\n'; // Enter key is introduced
	
    char map_upper[KEY_MAX] = {0};
    map_upper[KEY_A] = 'A'; map_upper[KEY_B] = 'B'; map_upper[KEY_C] = 'C';
    map_upper[KEY_D] = 'D'; map_upper[KEY_E] = 'E'; map_upper[KEY_F] = 'F';
    map_upper[KEY_G] = 'G'; map_upper[KEY_H] = 'H'; map_upper[KEY_I] = 'I';
    map_upper[KEY_J] = 'J'; map_upper[KEY_K] = 'K'; map_upper[KEY_L] = 'L';
    map_upper[KEY_M] = 'M'; map_upper[KEY_N] = 'N'; map_upper[KEY_O] = 'O';
    map_upper[KEY_P] = 'P'; map_upper[KEY_Q] = 'Q'; map_upper[KEY_R] = 'R';
    map_upper[KEY_S] = 'S'; map_upper[KEY_T] = 'T'; map_upper[KEY_U] = 'U';
    map_upper[KEY_V] = 'V'; map_upper[KEY_W] = 'W'; map_upper[KEY_X] = 'X';
    map_upper[KEY_Y] = 'Y'; map_upper[KEY_Z] = 'Z';
	
	/***/
	/* Newly introduce keys go here */
	/***/
	map_upper[KEY_GRAVE] = '~'; map_upper[KEY_1] = '!'; map_upper[KEY_2] = '@';
	map_upper[KEY_3] = '#';   map_upper[KEY_4] = '$'; map_upper[KEY_5] = '%';
	map_upper[KEY_6] = '^';   map_upper[KEY_7] = '&'; map_upper[KEY_8] = '*';
	map_upper[KEY_9] = '(';   map_upper[KEY_0] = ')'; map_upper[KEY_MINUS] = '_';
	map_lower[KEY_EQUAL] = '='; map_upper[KEY_EQUAL] = '+';
	map_lower[KEY_LEFTBRACE]  = '[';  map_upper[KEY_LEFTBRACE]  = '{';
	map_lower[KEY_RIGHTBRACE] = ']';  map_upper[KEY_RIGHTBRACE] = '}';
    map_lower[KEY_BACKSLASH]  = '\\'; map_upper[KEY_BACKSLASH]  = '|';
	map_lower[KEY_SEMICOLON]    = ';';
    map_upper[KEY_SEMICOLON]    = ':';
    map_lower[KEY_APOSTROPHE]   = '\'';
    map_upper[KEY_APOSTROPHE]   = '"';
	map_lower[KEY_COMMA]  = ',';
    map_upper[KEY_COMMA]  = '<';
    map_lower[KEY_DOT]    = '.';
    map_upper[KEY_DOT]    = '>';
    map_lower[KEY_SLASH]  = '/';
    map_upper[KEY_SLASH]  = '?';
    /***/
	/* Newly introduce keys end here */
	/***/
    if (code >= 0 && code < KEY_MAX) {
        if (shift) return map_upper[code] ? map_upper[code] : map_lower[code];
        return map_lower[code];
    }
    return 0;
}

int auto_detect_keyboard(char *device_path, size_t max_len) {
    DIR *dir = opendir("/dev/input");
    if (!dir) return -1;

    struct dirent *entry;
    char path[256];
    char name[256];

    while ((entry = readdir(dir)) != NULL) {
        if (strncmp(entry->d_name, "event", 5) == 0) {
            snprintf(path, sizeof(path), "/dev/input/%s", entry->d_name);
            int fd = open(path, O_RDONLY);
            if (fd < 0) continue;

            memset(name, 0, sizeof(name));
            // Check if the device has a name containing "Keyboard" or similar identifier
            if (ioctl(fd, EVIOCGNAME(sizeof(name)), name) >= 0) {
                // Look for common keyboard identifiers in the name
                if (strstr(name, "Keyboard") || strstr(name, "keyboard") || strstr(name, "AT Translated")) {
                    unsigned long evbit[1] = {0};
                    // Ensure the device supports event type EV_KEY
                    if (ioctl(fd, EVIOCGBIT(0, sizeof(evbit)), evbit) >= 0) {
                        if (evbit[0] & (1 << EV_KEY)) {
                            close(fd);
                            closedir(dir);
                            snprintf(device_path, max_len, "%s", path);
                            printf("Auto-detected keyboard: %s (%s)\n", name, path);
                            return 0;
                        }
                    }
                }
            }
            close(fd);
        }
    }
    closedir(dir);
    return -1;
} // end function auto_detect_keyboard

int main(int argc, char *argv[]) {
	long int bn_char;
	char dev_path[256];
	int bangla_mode = 1; // 1 for Bangla phonetic mode, -1 for normal English mode
	int win_pressed = 0; // To track Windows/Super key state
	if (argc < 2) {
        printf("No event path provided. Attempting to auto-detect keyboard...\n");
        if (auto_detect_keyboard(dev_path, sizeof(dev_path)) < 0) {
            fprintf(stderr, "Error: Could not automatically find a keyboard device.\n");
            fprintf(stderr, "Usage: sudo %s /dev/input/eventX\n", argv[0]);
            return 1;
        }
    } else {
        snprintf(dev_path, sizeof(dev_path), "%s", argv[1]);
    }

    int fd_phys = open(dev_path, O_RDONLY);
    if (fd_phys < 0) {
        perror("Failed to open physical input device");
        return 1;
    }
  /*////////////////////////////////////
    if (argc < 2) {
        fprintf(stderr, "Usage: sudo %s /dev/input/eventX\n", argv[0]);
        return 1;
    }

    int fd_phys = open(argv[1], O_RDONLY);
    if (fd_phys < 0) {
        perror("Failed to open physical input device");
        return 1;
    }
*/
    int fd_uinput = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
    if (fd_uinput < 0) {
        perror("Failed to open /dev/uinput");
        close(fd_phys);
        return 1;
    }

    ioctl(fd_uinput, UI_SET_EVBIT, EV_KEY);
    ioctl(fd_uinput, UI_SET_EVBIT, EV_SYN);
    for (int i = 0; i < KEY_MAX; i++) {
        ioctl(fd_uinput, UI_SET_KEYBIT, i);
    }

    struct uinput_setup usetup;
    memset(&usetup, 0, sizeof(usetup));
    usetup.id.bustype = BUS_USB;
    usetup.id.vendor = 0x1234;
    usetup.id.product = 0x5678;
    strcpy(usetup.name, "Wasi Phonetic Virtual Keyboard");

    if (ioctl(fd_uinput, UI_DEV_SETUP, &usetup) < 0 || ioctl(fd_uinput, UI_DEV_CREATE) < 0) {
        perror("Failed to setup uinput device");
        close(fd_phys);
        close(fd_uinput);
        return 1;
    }

    if (ioctl(fd_phys, EVIOCGRAB, 1) < 0) {
        perror("Warning: Could not grab physical device");
    }

    printf("Wasi Phonetic Daemon active on %s. Press Ctrl+C to stop.\n", argv[1]);

    int shift_pressed = 0;
	int ctrl_pressed = 0;
    struct input_event ev;

    while (read(fd_phys, &ev, sizeof(ev)) > 0) {
		//printf("\nuser_space read() function executed.\n");
		//printf("Type: %u | Code: %u | Value: %d\n", ev.type, ev.code, ev.value);
        if (ev.type == EV_KEY) {
            if (ev.code == KEY_LEFTSHIFT || ev.code == KEY_RIGHTSHIFT) {
                shift_pressed = (ev.value > 0);
				// Forward the raw modifier state to uinput
                emit(fd_uinput, ev.type, ev.code, ev.value);
                emit(fd_uinput, SYN_REPORT, 0, 0);
                continue;
            }
			if (ev.code == KEY_LEFTCTRL || ev.code == KEY_RIGHTCTRL) {
                ctrl_pressed = (ev.value > 0); // <--- Track Ctrl state
                emit(fd_uinput, ev.type, ev.code, ev.value);
                emit(fd_uinput, SYN_REPORT, 0, 0);
                continue;
            }
			if (ev.code == KEY_LEFTCTRL || ev.code == KEY_RIGHTCTRL ||
                ev.code == KEY_LEFTALT || ev.code == KEY_RIGHTALT ||
                /*ev.code == KEY_LEFTMETA || ev.code == KEY_RIGHTMETA || */ev.code == KEY_COMPOSE || ev.code == KEY_CAPSLOCK) { // KEY_LEFTMETA is the Super/Windows key
                
                // Forward modifier state directly without character translation
                emit(fd_uinput, ev.type, ev.code, ev.value);
                emit(fd_uinput, SYN_REPORT, 0, 0);
                continue;
            }
			if (ev.code == KEY_LEFTMETA || ev.code == KEY_RIGHTMETA) { // check if the Windows_button is pressed
			    win_pressed = (ev.value > 0);
			    // Forward the raw modifier state to uinput
			    emit(fd_uinput, ev.type, ev.code, ev.value);
			    emit(fd_uinput, SYN_REPORT, 0, 0);
			    continue;
			}

            if (ev.value == 1) {
				// Toggle bangla_mode on Windows + Space
			    if (win_pressed && ev.code == KEY_SPACE) {
			        bangla_mode = -bangla_mode; // Flips between 1 and -1
		        if (bangla_mode == 1) {
        		    printf("\n[Mode Switched] Bangla Phonetic Mode Activated (1)\n");
        		} else {
        		    printf("\n[Mode Switched] Normal English Mode Activated (-1)\n");
        		    init_buffer(); // Clear phonetic buffer when switching back to English
        		}
        		continue; // Skip further processing for this shortcut event
    		}	
               		// If Ctrl is held down (e.g. Ctrl+A, Ctrl+C, Ctrl+V), bypass phonetic engine and pass raw key through
                if (ctrl_pressed) {
                   		tap_key(fd_uinput, ev.code);
                    continue;
                }
				// Pass arrow keys and other navigation keys straight through
                if (ev.code == KEY_LEFT || ev.code == KEY_RIGHT || ev.code == KEY_UP || ev.code == KEY_DOWN || ev.code == KEY_HOME || ev.code == KEY_END || ev.code == KEY_PAGEUP || ev.code == KEY_PAGEDOWN || ev.code == KEY_INSERT || ev.code == KEY_DELETE || ev.code == KEY_ESC || (ev.code >= KEY_F1 && ev.code <= KEY_F12) || ev.code == KEY_SYSRQ || ev.code == KEY_TAB || ev.code == KEY_ENTER){
                    tap_key(fd_uinput, ev.code);
                    continue;
                }
                if (ev.code == KEY_BACKSPACE) {
                    if (buffer_size > 0) {
                        buffer_size--;
                        buffer[buffer_size] = '\0';
                    }
					printf(" in if clause: fd_uinput = %d\n", fd_uinput);
                    tap_key(fd_uinput, KEY_BACKSPACE);
                    continue;
                }

                char ch = keycode_to_char(ev.code, shift_pressed);
                if (ch != 0) {
					// ---> FIXED: Check if we are in English mode (-1) <---
                    if (bangla_mode == -1) {
                        tap_key(fd_uinput, ev.code); // Type raw English key directly
                        continue;
                    }
					printf("\nenqueue function is going to be called.\n");
					printf("The character being passed is: %c\n", ch);
                    enqueue(ch);
					print_buffer();
					printf("before find_bangla_char value of fd_uinput is : %d\n", fd_uinput);
                    bn_char = find_bangla_char(fd_uinput);
                    if (bn_char != 0) {
				    // Check if the current character is a vowel modifier/kar or compound extender 
				    // that needs to overwrite the previous base/vowel on screen
						    /*if (buffer_size > 1 && (ch == 'h' || ch == 'g' || ch == 'i' || ch == 'a' || ch == 'e' || ch == 'u' || ch == 'o')) {
							        tap_key(fd_uinput, KEY_BACKSPACE);
    						}*/
						    inject_unicode(fd_uinput, bn_char);
					}
                }
            }
        }
    }

    ioctl(fd_phys, EVIOCGRAB, 0);
    ioctl(fd_uinput, UI_DEV_DESTROY);
    close(fd_phys);
    close(fd_uinput);
    return 0;
}