#ifndef __IN_OUT_H__
#define __IN_OUT_H__

int get_text(char *str, int size);

int get_var_text(char **str);

void clear_stdin();

void text_wrap(char *str, int wrap);

int str_to_int(char *number);

#endif