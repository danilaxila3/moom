#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    return 1;
  }

  FILE *file = fopen(argv[1], "r");

  if (file == NULL) {
    return 1;
  }

  char code[1024] = {0};
  int code_len = 0;
  char code_ch;

  while ((code_ch = fgetc(file)) != EOF) {
    code[code_len] = code_ch;
    code_len++;
  }

  fclose(file);

  char tokens[512][2][64];
  int tokens_num = 0;
  int token_len = 0;

  char *code_ptr = &code[0];

  while (*code_ptr != '\0') {
    if (isalpha(*code_ptr)) {
      token_len = 0;
      strcpy(tokens[tokens_num][1], "STATEMENT");

      while (isalpha(*code_ptr)) {
        tokens[tokens_num][0][token_len] = *code_ptr;
        token_len++;
        code_ptr++;
      }

      tokens_num++;
    }

    if (isdigit(*code_ptr) || *code_ptr == '-' || *code_ptr == '.') {
      token_len = 0;
      strcpy(tokens[tokens_num][1], "NUMBER");

      while (isdigit(*code_ptr) || *code_ptr == '-' || *code_ptr == '.') {
        tokens[tokens_num][0][token_len] = *code_ptr;
        token_len++;
        code_ptr++;
      }

      tokens_num++;
    }

    if (*code_ptr == '\"') {
      token_len = 0;
      strcpy(tokens[tokens_num][1], "STRING");
      code_ptr++;

      while (*code_ptr != '\"') {
        tokens[tokens_num][0][token_len] = *code_ptr;
        token_len++;
        code_ptr++;
      }

      code_ptr++;
      tokens_num++;
    }

    if (strncmp(code_ptr, "+=", 2) == 0 || strncmp(code_ptr, "*=", 2) == 0) {
      strcpy(tokens[tokens_num][1], "OPERATOR");
      strncpy(tokens[tokens_num][0], code_ptr, 2);
      code_ptr += 2;
      tokens_num++;
    }

    if (*code_ptr == '=') {
      strcpy(tokens[tokens_num][1], "OPERATOR");
      tokens[tokens_num][0][0] = *code_ptr;
      code_ptr++;
      tokens_num++;
    }

    if (strchr(",()[]{}", *code_ptr)) {
      strcpy(tokens[tokens_num][1], "SYMBOL");
      tokens[tokens_num][0][0] = *code_ptr;
      tokens_num++;
    }

    code_ptr++;
  }

  /*for (int i = 0; i < tokens_num; i++) {*/
  /*  printf("%s %s\n", tokens[i][1], tokens[i][0]);*/
  /*}*/

  char variables[64][2][64];
  int variables_num = 0;

  int current_token = 0;

  while (current_token != tokens_num) {
    if (strcmp(tokens[current_token][1], "STATEMENT") == 0 &&
        strcmp(tokens[current_token + 1][0], "(") == 0) {
      char statement[64];
      strcpy(statement, tokens[current_token][0]);
      current_token += 2;

      if (strcmp(statement, "moomInput") == 0) {
        for (int i = 0; i < 64; i++) {
          if (strcmp(variables[i][0], tokens[current_token][0]) == 0) {
            fgets(variables[i][1], 64, stdin);
          }
        }
      }

      while (strcmp(tokens[current_token][0], ")") != 0) {
        if (strcmp(tokens[current_token + 1][0], ",") == 0 ||
            strcmp(tokens[current_token + 1][0], ")") == 0) {
          if (strcmp(statement, "moomPrint") == 0) {
            if (strcmp(tokens[current_token][1], "STRING") == 0) {
              printf("%s", tokens[current_token][0]);
            }

            if (strcmp(tokens[current_token][1], "STATEMENT") == 0) {
              for (int i = 0; i < 64; i++) {
                if (strcmp(variables[i][0], tokens[current_token][0]) == 0) {
                  printf("%s", variables[i][1]);
                }
              }
            }
          }

          if (strcmp(statement, "moomPrintNl") == 0) {
            if (strcmp(tokens[current_token][1], "STRING") == 0) {
              printf("%s", tokens[current_token][0]);
            }

            if (strcmp(tokens[current_token][1], "STATEMENT") == 0) {
              for (int i = 0; i < 64; i++) {
                if (strcmp(variables[i][0], tokens[current_token][0]) == 0) {
                  printf("%s", variables[i][1]);
                }
              }
            }

            if (strcmp(tokens[current_token + 1][0], ")") == 0) {
              printf("\n");
            }
          }
        }

        current_token++;
      }
    }

    if (strcmp(tokens[current_token][0], "moomLet") == 0 &&
        strcmp(tokens[current_token + 2][0], "=") == 0) {
      strcpy(variables[variables_num][0], tokens[current_token + 1][0]);
      strcpy(variables[variables_num][1], tokens[current_token + 3][0]);
      variables_num++;
    }

    if (strcmp(tokens[current_token][1], "STATEMENT") == 0 &&
        strcmp(tokens[current_token + 1][0], "=") == 0) {
      for (int i = 0; i < 64; i++) {
        if (strcmp(variables[i][0], tokens[current_token][0]) == 0) {
          if (strcmp(tokens[current_token + 2][1], "NUMBER") == 0 || strcmp(tokens[current_token + 2][1], "STRING") == 0) {
            strcpy(variables[i][1], tokens[current_token + 2][0]);
          }

          if (strcmp(tokens[current_token + 2][1], "STATEMENT") == 0) {
            for (int j = 0; j < 64; j++) {
              if (strcmp(variables[j][0], tokens[current_token + 2][0]) == 0) {
                strcpy(variables[i][1], variables[j][1]);
              }
            }
          }
        }
      }
    }

    if (strcmp(tokens[current_token][1], "STATEMENT") == 0 &&
        strcmp(tokens[current_token + 1][0], "+=") == 0) {
      for (int i = 0; i < 64; i++) {
        if (strcmp(variables[i][0], tokens[current_token][0]) == 0) {
          char variable_buf[64] = {0};

          if (strcmp(tokens[current_token + 2][1], "NUMBER") == 0) {
            sprintf(variable_buf, "%f",
                    atof(variables[i][1]) + atof(tokens[current_token + 2][0]));
          }

          if (strcmp(tokens[current_token + 2][1], "STATEMENT") == 0) {
            for (int j = 0; j < 64; j++) {
              if (strcmp(variables[j][0], tokens[current_token + 2][0]) == 0) {
                sprintf(variable_buf, "%f",
                        atof(variables[i][1]) + atof(variables[j][1]));
              }
            }
          }

          strcpy(variables[i][1], variable_buf);
        }
      }
    }

    if (strcmp(tokens[current_token][1], "STATEMENT") == 0 &&
        strcmp(tokens[current_token + 1][0], "*=") == 0) {
      for (int i = 0; i < 64; i++) {
        if (strcmp(variables[i][0], tokens[current_token][0]) == 0) {
          char variable_buf[64] = {0};

          if (strcmp(tokens[current_token + 2][1], "NUMBER") == 0) {
            sprintf(variable_buf, "%f",
                    atof(variables[i][1]) * atof(tokens[current_token + 2][0]));
          }

          if (strcmp(tokens[current_token + 2][1], "STATEMENT") == 0) {
            for (int j = 0; j < 64; j++) {
              if (strcmp(variables[j][0], tokens[current_token + 2][0]) == 0) {
                sprintf(variable_buf, "%f",
                        atof(variables[i][1]) * atof(variables[j][1]));
              }
            }
          }

          strcpy(variables[i][1], variable_buf);
        }
      }
    }

    current_token++;
  }

  return 0;
}
