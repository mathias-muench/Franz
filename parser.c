#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <libgen.h>
#include <time.h>
#include <assert.h>

#include "parser.h"

#define MAX_IGC_RECORD_LEN 79

typedef struct {
	char *dirname;
	char *filename;
	int flight_number_of_the_day;
} path_tokens_t;

static int fnotd_char_to_int(char fnotd)
{
	if (fnotd < 65) {
		return (int) (fnotd - 48);
	} else {
		return (int) (fnotd - 55);
	}
}

static path_tokens_t *split_path(char *path, path_tokens_t * tokens)
{
	char *pos_dot = strrchr(path, '.');
	*pos_dot = '\0';

	tokens->flight_number_of_the_day = fnotd_char_to_int(*(pos_dot - 1));

	char *pos_slash = strrchr(path, '/');
	tokens->filename = pos_slash + 1;
	*pos_slash = '\0';
	tokens->dirname = path;

	return tokens;
}

struct a_record {
	char manufacturer[4];
	char serial_number[4];
};

char *int2base36(unsigned int n, char *buf)
{
	static const char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUFWXYZ";

	char *b = buf + 4;
	*--b = 0;

	do {
		if (b == buf)
			return NULL;

		*--b = digits[n % 36];
		n /= 36;
	} while (n);

	return b;
}

struct a_record *parse_a_record(struct a_record *record, const char *line)
{
	assert(line[0] == 'A');

	int result;

	char sn[6];
	result = sscanf(line, "A%3s%5[0-9]", record->manufacturer, sn);
	if (result == 2 && strlen(sn) == 5) {
		int2base36(atoi(sn), record->serial_number);
	} else {
		result = sscanf(line, "A%3s%3s", record->manufacturer, record->serial_number);
	}

	if (result != 2) {
		record = NULL;
	}

	return record;
}

int parse_file(const char *path, name_time_t * result)
{
	FILE *fp = fopen(path, "r");

	if (!fp) {
		fprintf(stderr, "failed to open file!\n");
		return 0;
	}

	char line[MAX_IGC_RECORD_LEN];
	char *read;

	read = fgets(line, MAX_IGC_RECORD_LEN, fp);

	if (!read) {
		fprintf(stderr, "file is empty!\n");
		return 0;
	}

	if (line[0] != 'A') {
		fprintf(stderr, "file seems not to conform IGC standard!\n");
		return 0;
	}

	struct a_record a_recordS, *a_record = &a_recordS;
	a_record = parse_a_record(a_record, line);
	if (!a_record) {
		fprintf(stderr, "file seems not to conform IGC standard!\n");
		return 0;
	}

	char date_str[255];

	while (fgets(line, MAX_IGC_RECORD_LEN, fp)) {
		line[strcspn(line, "\r\n")] = '\0';
		if (strncmp("HFDTE", line, 5) == 0) {
			int day, month, year;
			struct tm tm_struct;
			memset(&tm_struct, 0, sizeof(struct tm));
			putenv("TZ=");
			sscanf(line + 5, "%02d%02d%02d", &day, &month, &year);
			tm_struct.tm_year = year < 80 ? year + 100 : year;
			tm_struct.tm_mon = month - 1;
			tm_struct.tm_mday = day;
			tm_struct.tm_isdst = -1;
			tm_struct.tm_hour = 12;
			result->timestamp = mktime(&tm_struct);
			strftime(date_str, sizeof(date_str), "%Y-%m-%d", &tm_struct);
			break;
		}

		if (line[0] != 'H') {
			fprintf(stderr, "we are done reading the header.\n");
			break;
		}
	}

	fclose(fp);

	char buffer[FILENAME_MAX];
	strcpy(buffer, path);
	path_tokens_t tokens;
	split_path(buffer, &tokens);

	sprintf(result->name,
			"%s/%s-%s-%s-%02d.igc",
			tokens.dirname,
			date_str,
			a_record->manufacturer,
			a_record->serial_number,
			tokens.flight_number_of_the_day);

	return 1;
}

#ifdef UNIT_TEST
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "parser.h"

#include "cu.h"

int parse_file_should_return_null_on_invalid_path()
{
	name_time_t *input = malloc(sizeof(name_time_t));
	int result = parse_file("file_does_not_exist", input);

	return result == 0;
}

int parse_file_should_return_null_on_empty_file()
{
	name_time_t *input = malloc(sizeof(name_time_t));
	int result = parse_file("test_igc/empty", input);

	return result == 0;
}

int parse_file_should_return_null_on_invalid_file()
{
	name_time_t *input = malloc(sizeof(name_time_t));
	int result = parse_file("test_igc/invalid", input);

	return result == 0;
}

int parse_file_should_not_return_null_on_valid_path()
{
	name_time_t input;
	int result = parse_file("test_igc/58cd1vj1.igc", &input);

	return result != 0;
}

int parse_file_should_return_correct_filename()
{
	name_time_t *input = malloc(sizeof(name_time_t));
	int result = parse_file("test_igc/58cd1vj1.igc", input);
	/*printf("actual: %s\n", input->name);/* */
	const char *expected = "test_igc/2015-08-12-FLA-1VJ-01.igc";
	/*printf("actual: %s, expected: %s", input->name, expected);/* */

	return strcmp(input->name, expected) == 0;
}

int parse_file_should_return_correct_time()
{
	name_time_t *input = malloc(sizeof(name_time_t));
	int result = parse_file("test_igc/58cd1vj1.igc", input);
	struct tm tm_struct = {
		.tm_year = 115,
		.tm_mon = 7,
		.tm_mday = 12,
		.tm_hour = 12, /**/.tm_isdst = -1
	};
	time_t expected = mktime(&tm_struct);

	return input->timestamp == expected;
}

int split_path_should_return_correct_tokens()
{
	path_tokens_t tokens = { NULL, NULL, 0, NULL };
	char buffer[FILENAME_MAX];
	strcpy(buffer, "test_igc/58cd1vjC.igc");
	split_path(buffer, &tokens);

	return strcmp(tokens.dirname, "test_igc") == 0
		&& strcmp(tokens.filename, "58cd1vjC") == 0
		&& tokens.flight_number_of_the_day == 12;
}

int parse_a_record_with_three_char_serial_number()
{
	struct a_record recordS, *record = &recordS;

	record = parse_a_record(record, "AFLA1VJ");

	return (strcmp(record->manufacturer, "FLA") == 0
			&& strcmp(record->serial_number, "1VJ") == 0);
}

int parse_a_record_with_five_digit_serial_number()
{
	struct a_record recordS, *record = &recordS;

	record = parse_a_record(record, "AFIL10258");

	return (strcmp(record->manufacturer, "FIL") == 0
			&& strcmp(record->serial_number, "7WY") == 0);
}

int main(char **argv, int argc)
{
	run_test(parse_file_should_return_null_on_invalid_path);
	run_test(parse_file_should_not_return_null_on_valid_path);
	run_test(parse_file_should_return_null_on_empty_file);
	run_test(parse_file_should_return_null_on_invalid_file);
	run_test(parse_file_should_return_correct_filename);
	run_test(parse_file_should_return_correct_time);
	run_test(split_path_should_return_correct_tokens);
	run_test(parse_a_record_with_three_char_serial_number);
	run_test(parse_a_record_with_five_digit_serial_number);
	exit(!test_result());
}
#endif

/* vi:noai:ts=4:sw=4
*/
