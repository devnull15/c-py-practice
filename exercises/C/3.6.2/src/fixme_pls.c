#include "../include/fixme_pls.h"
#include <ctype.h>

#define UNDERSCORE 95
#define BUFSIZE 0x20

struct test *usr;

bool _char_is_valid(char c) {
  if(!isalnum(c) && c != UNDERSCORE) { return false; }
  else { return true; }
}

/**
 * @brief Check if the provided name is valid.
 * Valid characters are [a-zA-Z0-9_]
 * @param name The name to check 
 * @return true is the name is valid false otherwise
 */
bool name_is_valid(char *name)
{
  char *c = name;
  while(*c != 0) {
    if(!_char_is_valid(*c)) { return false; }
    c++;
  }
  return true;
} 

/**
 * @brief Remove nonvalid characters from the name.
 * You should allocate new memory to store the new name.
 * Valid characters are [a-zA-Z0-9_]
 * @param name The name to sanitize 
 * @return A pointer to new memory that contains the sanitized name.
 */
char *sanitize_name(char *name)
{
  char *new = calloc(strlen(name)+1, 1);
    char *new_p = new;
    char *c = name;
    while(*c != 0) {
      if(!_char_is_valid(*c)) {
	while(!_char_is_valid(*c) && *c != 0) { c++; }
      }

      if(*c == 0) { break; }
      memcpy(new_p, c, 1);
      c++;
      new_p++;
    }
    //strcpy(new, name);
    return new;
}

void ask_name(void)
{
    char _name[BUFSIZE];
    printf("What is your name?\n> ");
    scanf("%s", _name);
    if (name_is_valid(_name))
    {
      strncpy(usr->buf, _name,BUFSIZE-1);
    }
    else
    {
        char *new = sanitize_name(_name);
        strncpy(usr->buf, new, BUFSIZE-1);
        free(new);
    }
}

int print_menu(void)
{
    unsigned int n = 0;
    puts("0) Exit");
    puts("1) Echo Chamber");
    puts("2) Play game");
    puts("");
    scanf("%d", &n);
    return n;
}

void echo(void)
{
    char buf[0x100];
    puts("What do you want to say?");
    scanf("%256s", buf);
    printf("%s", buf);
}

void play_game(void)
{
    getc(stdin);
    struct test goodluck;
    zero_memory(&goodluck, sizeof(struct test));
    puts("Try your luck");
    fgets(goodluck.buf, BUFSIZE, stdin);
    if (goodluck.val == 0xdeadbeef)
    {
        puts("Good job");
    }
}

void check_auth(void)
{
    if (usr->val)
    {
        puts("Congrats you win");
    }
    else 
    {
        puts("I don't think so");
    }
}

void *zero_memory(void *ptr, size_t size)
{
  void *ptr_i = ptr;
  for(size_t i = 0; i < size; i++) {
    *(char*)ptr_i = 0;
    ptr_i++;
  }
  
  return ptr;
}

int my_main(void)
{
    unsigned int choice = 0;
    usr = malloc(sizeof(struct test));
    zero_memory(usr, sizeof(struct test));
    ask_name();
    printf("Welcome, ");
    printf("%s", usr->buf);
    while (1)
    {
        choice = print_menu();
        switch(choice)
        {
	case 0:
	  puts("Bye");
	  free(usr);
	  return 0;
	case 1:
	  echo();
	  break;
	case 2:
	  play_game();
	  break;
	case 3:
	  check_auth();
	  break;
	default:
	  puts("I didn't understand that");
        }
    }
}
