#ifndef INC_PARSER_H_
#define INC_PARSER_H_

#include <sys/types.h>

typedef struct {
	char* name;
	time_t timestamp;
} name_time_t;

int parse_file(char* filename, name_time_t* result);

#endif /* INC_PARSER_H_ */
