#define _DEFAULT_SOURCE
#define _BSD_SOURCE
#define _GNU_SOURCE

#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <sys/types.h>
#include <time.h>

#define HELP_MESSAGE "HELP: Ctrl-Q = quit  |  Ctrl-S = save"
#define HELP_MESSAGE_LENGTH strlen(HELP_MESSAGE)
#define TAB_SIZE 8
#define QUIT_THRESHOLD 3
#define VERSION_NUMBER "1.2.0"

// k BITWISE-AND with 00011111
#define CTRL_KEY(k) ((k) & 0x1f)


void setMessage(const char *fmt, ...);
void refreshScreen();


////////////////////////////////
/////////  BUFFER   //////////////////////////////////////////////////////////////////////
////////////////////////////////

//buffer is used to store data that needs to be written to the terminal
#define ABUF_INIT {NULL, 0}//empty buffer constructor
struct abuf 
{
  char *data;//pointer to our buffer in memory
  int length;//amount of data to be written
};

//add data to be written to terminal
void appendBuffer(struct abuf *ab, const char *string, int length)
{
  //reallocate memory of "data*" to fit new data 
  char *new_data = realloc((*ab).data, (*ab).length + length);

  if (new_data == NULL)//error handling
  {
    return;
  }

  //add new string after orignal data
  memcpy(&new_data[(*ab).length], string, length);

  (*ab).data = new_data; //set b as the new combined buffer data 
  (*ab).length = (*ab).length + length;//set the new length
}

//free memory from our buffer
void freeBuffer(struct abuf *ab)
{
  free((*ab).data);
}

/////////////////////////////////
/////////  TERMINAL CONFIG //////////////////////////////////////////////////////////////////////
/////////////////////////////////

//struct for storing text inside a row.
typedef struct row_object
{
  int length;
  char *chars;//pointer to location of rows contents.
  int real_size;
  char *real;
} row_object;

//text editor key mappings.
enum editorKey 
{
  BACKSPACE = 127, //backspace in ascii
  LEFT_ARROW = 1000,
  RIGHT_ARROW = 1001,
  UP_ARROW = 1002,
  DOWN_ARROW = 1003,
  PAGE_UP = 1004,
  PAGE_DOWN = 1005,
  HOME_KEY = 1006,
  END_KEY = 1007,
  DEL_KEY = 1008,
};

//global struct that contain editors state
struct editorSettings
{
  int changed;
  struct termios orignal_terminal;
  int rowOffset, colOffset; //to allow scrolling
  int colNum, rowNum;
  int cursorX, cursorY;
  int realX;
  int rowsOfText; //how many rows of text there are.
  row_object *row;
  char *file_name;
  char status_message[80];
  time_t status_message_time;
};
struct editorSettings editor;

//functions that throws appropriate error message
void kill(const char *s)
{
  //clear screen before exiting
  write(STDOUT_FILENO, "\x1b[2J", 4);
  write(STDOUT_FILENO, "\x1b[H", 3);

  perror(s);//print error
  exit(1);
}

void disableRawMode()
{
  //returns -1 on failure
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &editor.orignal_terminal) == -1)
  {
    kill("tcsetattr Error");
  }
}

void activateRawMode()
{
  //read terminal attributes to terminal struct
  //returns -1 on failure
  if (tcgetattr(STDIN_FILENO, &editor.orignal_terminal) == -1)
  {
    kill("tcgetattr Error");
  }
  //when program exits, disable reset terminal
  atexit(disableRawMode);
  
  //copy our default terminal attributes
  struct termios new = editor.orignal_terminal;
  //set terminal flags
  new.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  new.c_oflag &= ~(OPOST);
  new.c_cflag |= (CS8);
  new.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);

  new.c_cc[VMIN] = 0;//min bytes to input
  new.c_cc[VTIME] = 1;//time before next read

  //set terminal attributes using our new struct
  //returns -1 on failure
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &new) == -1)
  {
    kill("tcsetattr Error");
  }
}

//handles generic input
int readKey()
{
  //read 1 byte from stdin into var c
  //read() returns number of bytes that were read
  //will return 0 if end of file reached.
  //assign char to nread variable
  int input_char;
  char c;
  while (( input_char = read(STDIN_FILENO, &c, 1) ) != 1) 
  {
    if (input_char == -1 && errno != EAGAIN)
    {
      kill("Read Error");
    }
  }

  //if byte read is <esc> then read two more bytes
  if (c == '\x1b')
  {
    char esc_seq[3];
    //read next 2 bytes of memory into the buffer
    read(STDIN_FILENO, &esc_seq[0], 1);
    read(STDIN_FILENO, &esc_seq[1], 1);

    //check for [ key
    if (esc_seq[0] == '[')
    {
      //check for digit (pageup or down key)
      if (esc_seq[1] >= '0' && esc_seq[1] <= '9')
      {
        //if so read next byte into buffer and check for squiggly
        read(STDIN_FILENO, &esc_seq[2], 1);
        if (esc_seq[2] == '~')
        {
          switch (esc_seq[1])
          {
            case '5': return PAGE_UP;//<esc>[~5
            case '6': return PAGE_DOWN;//<esc>[~6
            case '7': return HOME_KEY;//<esc>[~7
            case '8': return END_KEY;//<esc>[~8
            case '3': return DEL_KEY;//<esc>[3~
          }
        }
      }
      else if (esc_seq[0] == 'O')
      {
        switch (esc_seq[1])
        {
          case 'H': return HOME_KEY;//<esc>OH
          case 'F': return END_KEY;//<esc>OF
        }
      }
      else//if no digit or O, check for arrow keys
      {
        switch (esc_seq[1])
        {
          case 'A': return UP_ARROW;//<esc>[A
          case 'B': return DOWN_ARROW;//<esc>[B
          case 'C': return RIGHT_ARROW;//<esc>[C
          case 'D': return LEFT_ARROW;//<esc>[D
          case 'H': return HOME_KEY;//<esc>[H
          case 'F': return END_KEY;//<esc>[F
        }
      }
    }
    //otherwise just assume enter key was pressed
    return '\x1b';
  } 
  return c;
}

//get position of cusor
int getCursorPosition(int *rows, int *cols)
{
  char response[32];
  unsigned int i = 0;

  //cusor position report COMMAND "n" returns "<esc>[x;yR"
  if (write(STDOUT_FILENO, "\x1b[6n", 4) != 4)
  {
    return -1;//fail
  }

  //keep reading characters until we get to the R character.
  while (i < sizeof(response) - 1)//-1 as we need space for "\0"
  {
    //get response
    if (read(STDIN_FILENO, &response[i], 1) != 1)
    {
      break;//read failed
    }
    if (response[i] == 'R')
    {
      break;//r received
    }
    i++;//increment pointer
  }

  response[i] = '\0'; //last element is null terminator

  //make sure escape sequence is present
  if (response[0] != '\x1b' || response[1] != '[')
  {
    return -1;//fail
  }

  //get x and y values assign them to rows and cols
  //use buffer[2] to skip first two values
  //read rest of memory until null term
  if (sscanf(&response[2], "%d;%d", rows, cols) != 2)
  {
    return -1;//fail
  }
  return 0;
}

//get size of terminal
int getTerminalSize(int *rows, int *cols)
{
  struct winsize size;

  //first try using ioctl
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) == -1 || size.ws_col == 0)
  {
    //if failed use cursor move cmds to get to bottom right of screen
    if (write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12)
    {
      return -1;
    }
    return getCursorPosition(rows, cols);
  }
  else 
  {
    *cols = size.ws_col;
    *rows = size.ws_row;
    return 0;
  }
}

//////////////////////////////////////////////
/////////  ROW STUFF  ///////////////////////////////////////////////////////////
//////////////////////////////////////////////

//find the absolute value of the cursor.
//this is formed using the realed rows as opposed to the normal rows.
int absolute_CursorX(row_object *row, int cursorX)
{
  int realX = 0;

  for (int i = 0; i < cursorX; i++)
  {
    if ((*row).chars[i] == '\t')
    {
      //find out how many columns we are to the left of the next tab 
      realX = realX + (TAB_SIZE - 1) - (realX % TAB_SIZE);
      //add it to the realX to get the real cursor position.
    } 
    realX++;
  }

  return realX;
}

void updateRow(row_object *row)
{
  int number_of_tabs = 0;

  //firstly we count the number of tabs
  for(int i = 0; i < (*row).length; i++)
  {
    if((*row).chars[i] == '\t')
    {
      number_of_tabs++;
    }
  }

  free((*row).real);
  //maximum memory needed as tabs can only be 8 or less in size.
  (*row).real = malloc((*row).length + (number_of_tabs * (TAB_SIZE-1) ) + 1);//+1 for null term
  
  int new_real_size = 0;

  for (int i = 0; i < (*row).length; i++)
  {
    //if its a tab character
    if ((*row).chars[i] == '\t') 
    {
      //keep adding space to represent the tab.
      (*row).real[new_real_size++] = ' ';
      while (new_real_size % TAB_SIZE != 0)
      {
        (*row).real[new_real_size++] = ' ';
      }
    }
    else //just add the character normally
    {
      (*row).real[new_real_size++] = (*row).chars[i];
    }
  }

  (*row).real[new_real_size] = '\0';
  (*row).real_size = new_real_size;
}

//insert row of text at given index
void insertRow(int line_number, char *line, size_t line_length)
{
  if (line_number < 0 || line_number > editor.rowsOfText)
  {
    return;
  }

  //alocate space for another rowobject in the array(+1).
  editor.row = realloc(editor.row, sizeof(row_object) * (editor.rowsOfText + 1));

  //move row to the next array element
  memmove(&editor.row[line_number + 1], &editor.row[line_number], sizeof(row_object) * (editor.rowsOfText - line_number));

  editor.row[line_number].length = line_length;

  //allocate memory for string plus null terminator.
  editor.row[line_number].chars = malloc(line_length + 1);

  //copy the string to our row
  memcpy(editor.row[line_number].chars, line, line_length);

  //terminate will null char.
  editor.row[line_number].chars[line_length] = '\0';

  editor.row[line_number].real_size = 0;
  editor.row[line_number].real = NULL;

  updateRow(&editor.row[line_number]);

  editor.rowsOfText++;//we just added a row of text to display
  editor.changed++;//contents updates when we add a row.
}

void addCharToRow(row_object *row, int row_index, int text)
{
  //if out of bouds of row text, insert at end of line.
  if (row_index < 0 || row_index > (*row).length)
  {
    row_index = (*row).length;
  }

  //reallocate for null term and new character.
  (*row).chars = realloc((*row).chars, (*row).length + 2);

  memmove(&(*row).chars[row_index + 1], &(*row).chars[row_index], (*row).length - row_index + 1);

  //correct length
  (*row).length++;

  //assign new char.
  (*row).chars[row_index] = text;

  updateRow(row);
}

void delCharFromRow(row_object *row, int row_index)
{
  //don't delete if out of bounds of line
  if (row_index < 0 || row_index >= (*row).length)
  {
    return;
  }
  //replace the following character with the one we are trying to delete.
  memmove(&(*row).chars[row_index], &(*row).chars[row_index + 1], (*row).length - row_index);
  
  //decrease the length
  (*row).length--;
  editor.changed++;

  updateRow(row);
}

void freeRow(row_object *row)
{
  free((*row).real);
  free((*row).chars);
}

void deleteRow(int line_number)
{
  if (line_number < 0 || line_number >= editor.rowsOfText)
  {
    return;//no text to delete, just return
  }
  //free the memory as we wont be using it anymore.
  freeRow(&editor.row[line_number]);
  //move next row down one to over write it.
  memmove(&editor.row[line_number], &editor.row[line_number + 1], sizeof(row_object)*(editor.rowsOfText - line_number - 1));
  
  editor.rowsOfText--;
  editor.changed++;
}

void combineRows(row_object *row, char *row_text, size_t row_length)
{
  //length of line now become the oringal length plus the length of the row we are adding.
  (*row).chars = realloc((*row).chars, (*row).length + row_length + 1);//+1 for null byte

  //starting from the end of the previous row, we add our row text.
  memcpy(&(*row).chars[(*row).length], row_text, row_length);

  (*row).length += row_length;
  (*row).chars[(*row).length] = '\0';

  updateRow(row);
  editor.changed++;
}

/////////////////////////////////
/////////  EDITOR METHODS  //////////////////////////////////////////
/////////////////////////////////

//use rowAddChar to add char at cursor position
void insertChar(int new_char)
{
  //if at end of file, insert new row.
  if (editor.cursorY >= editor.rowsOfText)
  {
    insertRow(editor.rowsOfText,"",0);
    addCharToRow(&editor.row[editor.cursorY], editor.cursorX, new_char);
  }
  else
  {
    addCharToRow(&editor.row[editor.cursorY], editor.cursorX, new_char);
    editor.cursorX++;//incrment x pos for new char
  }

  editor.changed++;
  //add new char to the row.

}

//add a new empty at current Y position
void insertNewline()
{
  if (editor.cursorX == 0) 
  {
    //just a blank line if at beginning
    insertRow(editor.cursorY, "", 0);
  }
  else
  {
    row_object *row = &editor.row[editor.cursorY];
    
    //add text after the cusror to the next row.
    insertRow(editor.cursorY + 1, &(*row).chars[editor.cursorX], (*row).length - editor.cursorX);

    //reassign, as data has been changed after function call^^
    row = &editor.row[editor.cursorY];

    (*row).length = editor.cursorX;
    (*row).chars[(*row).length] = '\0';

    updateRow(row);
  }

  editor.cursorY++;
  editor.cursorX = 0;
}

void deleteChar()
{
  if (editor.cursorY == editor.rowsOfText)
  {
    return;//nothing to delete
  }
  if (editor.cursorX == 0 && editor.cursorY == 0)
  {
    return;
  }

  //create row object from address of row at current y pos.
  row_object *row = &editor.row[editor.cursorY];
  if (editor.cursorX > 0) 
  {
    //-1 to get char to left of cursor
    delCharFromRow(row, editor.cursorX - 1);
    editor.cursorX--;//move to left
  }
  else 
  {
    //X = end of previous row
    editor.cursorX = editor.row[editor.cursorY - 1].length;
    //combine the chars from the current row to the row below.
    combineRows(&editor.row[editor.cursorY - 1], (*row).chars, (*row).length);
    deleteRow(editor.cursorY);

    editor.cursorY--;
  }
}

///////////////////////////////////////////////
//////////// FILE INPUT AND OUTPUT ////////////////////////////////////////////////////
//////////////////////////////////////////////

char *toString(int *buffer_length)
{
  //sum the lengths of each row
  //to figure out how much mem to allocate
  int total_length = 0;
  for (int i = 0; i < editor.rowsOfText; i++)
  {
    total_length += editor.row[i].length + 1;//+1 for newline
  }
  //VV to use outside of function. VV
  *buffer_length = total_length;

  char *buffer = malloc(total_length);
  //use b as a pointer to the memory in buffer
  //so we don't change where *buffers memory starts from.
  char *b = buffer;

  for (int i = 0; i < editor.rowsOfText; i++)
  {
    //copy onto b
    memcpy(b, editor.row[i].chars, editor.row[i].length);
    //set pointer to after the previous line of text
    b += editor.row[i].length;
    //set the value that it poinnts to
    *b = '\n';
    //increment to make space for new line char.
    b++;
  }
  //buffer should now contain all of b's data 
  //and will read from the correct start locaiton.
  return buffer;
}


void editorOpen(char *filename)
{
  free(editor.file_name);
  editor.file_name = strdup(filename);

  FILE *fp = fopen(filename, "r");
  if(!fp)
  {
    kill("FILE I/O");
  }

  char *line = NULL; //holds line text
  size_t  max_line_length = 0;//how much memory to allocate
  ssize_t  line_length;

  

  //if we are not at the end of a file. keep reading
  while (( line_length = getline(&line, &max_line_length, fp)) != -1 )
  {
    //reduce line length until no \n \r are present.
    while ( line_length > 0 && (line[line_length - 1] == '\n' || line[line_length - 1] == '\r'))
    {
      line_length--;
    }
    //allocate space for new row struct
    //add the new struct into the row array in the editor config.
    insertRow(editor.rowsOfText, line, line_length);
  }
  free(line);
  fclose(fp);
  editor.changed = 0;
}



void saveFile()
{
  //if theres a filename;
  if (editor.file_name)
  {
    int length_of_file;
    char *file_contents = toString(&length_of_file);

    //O_CREAT = create new file if file doesnt exsist.
    //O_RDWR = reading and writing mode
    //0644 = permission code to read and write to file.
    int file_writer = open(editor.file_name, O_RDWR | O_CREAT, 0644);

    //-1 if file could not be opened.
    if(file_writer != -1)
    {
      //try to trim file length to match new length
      if(ftruncate(file_writer, length_of_file) != -1)
      {
        //try to write to file
        if(write(file_writer, file_contents, length_of_file) == length_of_file)
        {
          close(file_writer);
          free(file_contents);
          editor.changed = 0;
          setMessage("%d bytes written to disk", length_of_file);
          return;
        }
      }
      close(file_writer);
    }
    free(file_contents);
  }
  else
  {//a
    return;
  }
}

//////////////////////////////
/////////  INPUT  //////////////////////////////////////////
//////////////////////////////

void moveCursor(int key)
{
  //get the row object at our current y position
  row_object *row = (editor.cursorY >= editor.rowsOfText) ? NULL : &editor.row[editor.cursorY];

  switch (key)
  {
    case UP_ARROW://up
      if(editor.cursorY > 0)
      {
        editor.cursorY--;
      }
      else
      {
        editor.rowOffset = 0;
      }
      break;

    case DOWN_ARROW://down
      if(editor.cursorY < editor.rowsOfText)
      {
        editor.cursorY++;
      }
      else
      {
      }
      
      break;

    case LEFT_ARROW://left
      if(editor.cursorX > 0)
      {
        editor.cursorX--;
      }
      else if(editor.cursorY > 0)
      {
        editor.cursorY--;
        editor.cursorX = editor.row[editor.cursorY].length;
      }
      else
      {
        editor.rowOffset = 0;
      }
      break;

    case RIGHT_ARROW://right
    //if there is text on that row and the cursor isnt past the lines length then move it. 
      if (row && editor.cursorX < (*row).length)
      {
        editor.cursorX++;
      } else if (row && editor.cursorX == (*row).length) {
        editor.cursorY++;
        editor.cursorX = 0;
      }
      break;
  }

  //get the row object at our changed y position
  row = (editor.cursorY >= editor.rowsOfText) ? NULL : &editor.row[editor.cursorY];
  
  int line_length = row ? (*row).length : 0; //if row exsists assign its line length.

  //if its already at the end of the line, keep its place.
  if (editor.cursorX > line_length)
  {
    editor.cursorX = line_length;
  }
}

//handles inbuilt text editor functions
void functionKeypress()
{
  static int quit = QUIT_THRESHOLD;
  int key = readKey();

  switch (key)
  {
    //delete chars
    case BACKSPACE:
    case CTRL_KEY('h'):
    case DEL_KEY:
      if (key == DEL_KEY) moveCursor(RIGHT_ARROW);
      deleteChar();
      break;

    //enter
    case '\r':
      insertNewline();
      break;

    //refresh screen OPTIONAL
    case CTRL_KEY('l'):
    case CTRL_KEY('j'):
    case '\x1b':
      break;

    //quit editor  
    case CTRL_KEY('q'): 
      if(editor.changed && quit > 0)
      {
        char message[80];
        sprintf(message, "WARNING!!! UNSAVED CHANGES - Press Ctrl-Q %d more times to quit.", quit);
        setMessage(message);
        quit--;
        return;
      }
      //clear screen before exiting
      write(STDOUT_FILENO, "\x1b[2J", 4);
      write(STDOUT_FILENO, "\x1b[H", 3);
      exit(0);
      break;

    //save contents
    case CTRL_KEY('s'):
      saveFile();
    break;

    //cursor movement
    case UP_ARROW:
    case DOWN_ARROW:
    case LEFT_ARROW:
    case RIGHT_ARROW:
      moveCursor(key);
      break;

    //move to top of screen
    case PAGE_UP:
    {
      for(int i = 0; i < editor.rowNum; i++)
      {
        moveCursor(UP_ARROW);
      }
      break;
    }

    //move to bottom of screen
    case PAGE_DOWN:
    {
      for(int i = 0; i < editor.rowNum; i++)
      {
        moveCursor(DOWN_ARROW);
      }
      break;
    }

    //move to right of screen
    case END_KEY:
      if (editor.cursorY < editor.rowsOfText)
      {
        editor.cursorX = editor.row[editor.cursorY].length;
      }
      break;

    //move to left of screen
    case HOME_KEY:
      editor.cursorX = 0;
      break;

    default:
      insertChar(key);
      break;
  }
  quit = QUIT_THRESHOLD;
}

//////////////////////////////
/////////  OUTPUT  //////////////////////////////////////////
//////////////////////////////

void editorScroll()
{
  //check cursor location and change accordingly.
  editor.realX = 0;

  if (editor.cursorY < editor.rowsOfText)
  {
    editor.realX = absolute_CursorX(&editor.row[editor.cursorY], editor.cursorX);
  }

  
  if (editor.cursorY < editor.rowOffset)
  {
    editor.rowOffset = editor.cursorY - 1;
  }

  if (editor.cursorY >= editor.rowOffset + editor.rowNum)
  {
    editor.rowOffset = editor.cursorY - editor.rowNum + 1;
  }

  if(editor.cursorX < editor.colOffset)
  {
    editor.colOffset = editor.realX;
  }

  if(editor.realX >= editor.colOffset + editor.colNum)
  {
    editor.colOffset = editor.realX - editor.colNum + 1;
  }
}

void drawInfoBar(struct abuf *ab) 
{
  //invert colours
  appendBuffer(ab, "\x1b[7m", 4);

  char information[80];
  int information_length;

  information_length = snprintf(information, sizeof(information), 
  "Ln: %d  |  File-Name: %.20s  |  Lines: %d  |  %s",
    editor.cursorY+1,//currentline
    editor.file_name ? editor.file_name : "NO FILE",//filename
    editor.file_name ? editor.rowsOfText+1 : editor.rowsOfText,//number or lines
    editor.changed ? "NOT-SAVED" : "SAVED");


  appendBuffer(ab, information, information_length);

  while(information_length < editor.colNum)
  {
    appendBuffer(ab, " ", 1);
    information_length++;
  }

  appendBuffer(ab, "\x1b[m", 3);
  
}

void drawMessageBar(struct abuf *ab)
{
  //clear row
  appendBuffer(ab, "\x1b[K", 3);

  //get status message length
  int status_message_length = strlen(editor.status_message);

  //trim if larger than colNumber
  if (status_message_length > editor.colNum)
  {
    status_message_length = editor.colNum;
  }

  if(time(NULL) - editor.status_message_time < 5)
  {
    appendBuffer(ab, editor.status_message, status_message_length);

    while(status_message_length != editor.colNum)
    {
      appendBuffer(ab, " ", 1);
      status_message_length++;
    }
  }
  else
  {
    appendBuffer(ab,HELP_MESSAGE,HELP_MESSAGE_LENGTH);
  }
}


void drawTitle(struct abuf *ab) 
{
  //clear row
  appendBuffer(ab, "\x1b[K", 3);
  //draw title in inverted colours
  appendBuffer(ab, "\x1b[7m", 4);

  char titleMsg[80];
  int tileMsg_length = sprintf(titleMsg, "[ Joey's Text Editor - version %s ]", VERSION_NUMBER);
  appendBuffer(ab, titleMsg, tileMsg_length);

  while(tileMsg_length < editor.colNum)
  {
    appendBuffer(ab, " ", 1);
    tileMsg_length++;
  }

  //turn off inverted colours and form new line
  appendBuffer(ab, "\x1b[m", 3);
  appendBuffer(ab, "\r\n", 2);
}

void drawRows(struct abuf *ab)
{
  for (int i = 0; i < editor.rowNum; i++)
  {
    int file_index = editor.rowOffset + i;
    if (file_index >= editor.rowsOfText)
    {
      appendBuffer(ab, "~", 1);
    }
    else 
    {
      int row_length = editor.row[file_index].real_size - editor.colOffset;

      if (row_length < 0)
      {
        row_length = 0;
      }

      //trim if larger than colnum
      if (row_length > editor.colNum)
      {
        row_length = editor.colNum;
      }

      //write new row
      appendBuffer(ab, &editor.row[file_index].real[editor.colOffset], row_length);
    }

    //new line for each row.
    appendBuffer(ab, "\x1b[K", 3);
    appendBuffer(ab, "\r\n", 2);
  }
}

void refreshScreen()
{
  editorScroll();

  struct abuf toWrite = ABUF_INIT;

  // l command with "?25" will hide cursor
  appendBuffer(&toWrite, "\x1b[?25l", 6);

  //default H command moves cursor to top left.
  appendBuffer(&toWrite, "\x1b[H", 3);

  //append title
  drawTitle(&toWrite);
  drawRows(&toWrite);
  drawInfoBar(&toWrite);
  drawMessageBar(&toWrite);

  //H command to set cursor location
  char command[32];
  //concat our current cursor location to the H command
  snprintf(command, sizeof(command), "\x1b[%d;%dH", ((editor.cursorY+1) - editor.rowOffset) + 1, (editor.realX - editor.colOffset) + 1);
  //send it to the buffer.
  appendBuffer(&toWrite, command, strlen(command));

  // h command with "?25" will reset cursor
  appendBuffer(&toWrite, "\x1b[?25h", 6);

  //write commands/data to STDOUT and reset buffer.
  write(STDOUT_FILENO, toWrite.data, toWrite.length);
  freeBuffer(&toWrite);
}

//... = allow multiple args
void setMessage(const char *fmt, ...)
{
  
  va_list argument_list;

  va_start(argument_list, fmt);

  vsnprintf(editor.status_message, sizeof(editor.status_message), fmt, argument_list);

  va_end(argument_list);

  editor.status_message_time = time(NULL);

}

//////////////////////////////
/////////  INITILIZATION  //////////////////////////////////////////
//////////////////////////////

void setup()
{
  //cursor data
  editor.realX = 0;
  editor.cursorX = 0;
  editor.cursorY = 0;

  //row data
  editor.rowsOfText = 0;
  editor.row = NULL;
  editor.rowOffset = 0;
  editor.colOffset = 0;

  //status data
  editor.status_message[0] = '\0';
  editor.status_message_time = 0;
  editor.file_name = NULL;
  editor.changed = 0;//false

  if ( getTerminalSize(&editor.rowNum, &editor.colNum) == -1)
  {
    kill("getSize");
  }
  editor.rowNum = editor.rowNum - 3; //leave space for bar.

  //append title to screen.
}

int main(int argc, char *argv[])
{
  //default mode = cooked mode - input sent only when enter pressed
  //we want raw mode - sent continuosly.
  activateRawMode();
  setup();

  if (argc >= 2)
  {
    editorOpen(argv[1]);
  }

  setMessage("HELP: Ctrl-Q = quit  |  Ctrl-S = save");

  while(1)
  {
    refreshScreen();
    functionKeypress();//check if shortcuts were pressed
  }
  return 0;
}