#ifndef WRAP_UNWRAP_H
#define WRAP_UNWRAP_H

static const int bufLengs = 1024;
static const int hederLeng = 7;

char unwrap_data(char *buff, int *numberBytes);
void wrap_data(char *buff, int *numberBytes);

#endif //WRAP_UNWRAP_H
