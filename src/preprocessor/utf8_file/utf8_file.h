#ifndef UTF8_FILE_H
#define UTF8_FILE_H

#define UTF8_FILE_OK 1
#define UTF8_FILE_EOF -1
#define UTF8_FILE_ERROR -2
#define UTF8_FILE_CLOSED -4

#define MAX_ERROR_MSG_LENGTH 128

typedef struct
{
    FILE    *fp;
    int32_t status;
    bool    return_temp_next;
    int32_t temp;
    char    *error_msg;
    char    error_msg_buffer[MAX_ERROR_MSG_LENGTH];
    int     line;
    int     line_position;
} utf8_file_t;

/**
 * @brief Initializes a utf8_file_t.
 *
 * This function initializes all the elements of a utf8_file_t. Afterwards you
 * can open and close files all you want and it will properly keep track of the
 * state.
 * @param file Pointer to the utf8_file_t to initialize.
 */
void    init_utf8_file(utf8_file_t *file);

/**
 * @brief Opens utf8 file for reading.
 *
 * This uses the same rules for filenames as fopen(). It either needs just the
 * filename if file exists in the current directory or the full path if it
 * doesn't. Checking the return value doesn't matter because any attempt to
 * read if a file failed to open will return UTF8_FILE_ERROR and the error
 * message will have already been set.
 * @param file Pointer to the utf8_file_t to open.
 * @param filename The filename to pass to fopen() as is.
 * @return bool. True if file opened false if it failed.
 */
bool    open_utf8_file(utf8_file_t *file, char *filename);

/**
 * @brief Closes utf8 file
 *
 * I think this is going to be removed/changed to free utf8_file()
 * @param file Pointer to the utf8_file_t to close.
 */
void    close_utf8_file(utf8_file_t *file);

/**
 * @brief Returns status of utf8_file_t.
 *
 * The return value of this function will be UTF8_FILE_OK, UTF8_FILE_EOF,
 * UTF8_FILE_ERROR, or UTF8_FILE_CLOSED.
 * @param file Pointer to the utf8_file_t to check the status of.
 * @return A number indicating status of utf8_file_t.
 */
int32_t get_utf8_file_status(utf8_file_t *file);

/**
 * @brief Returns a character pointer to a string of the last error.
 *
 * If there is no error this returns a pointer to the string
 * "File open, no errors."
 * @param file Pointer to the utf8_file_t to get the last error message of.
 * @return Pointer to error message.
 */
char    *get_utf8_file_error_msg(utf8_file_t *file);

/**
 * @brief Generates an error message with position in the utf8_file_t buffer.
 *
 * I think this may be unnecessary and will be removed in the near future.
 * @param file Pointer to the utf8_file_t to 
 * @param msg A pointer to a character array.
 * @return UTF8_FILE_ERROR
 */
int32_t set_utf8_file_read_error(utf8_file_t *file, char *msg);

/**
 * @brief Returns the line the last read character is on.
 *
 * This will probably return a -1 under certain conditions in the near future.
 * @param file Pointer to the utf8_file_t.
 * @return
 */
int     get_utf8_file_line_number(utf8_file_t *file);

/**
 * @brief Returns the position in the line the last read character is on.
 *
 * This will probably return a -1 under certain conditions in the near future.
 * @param file Pointer to the utf8_file_t.
 * @return
 */
int     get_utf8_file_line_position(utf8_file_t *file);

/**
 * @brief returns the next byte from the file without decoding it.
 *
 *
 * @param file Pointer to the utf8_file_t to read from.
 * @return
 */
int32_t pop_byte_from_utf8_file(utf8_file_t *file);

/**
 * @brief Resets the status so you can test another error.
 *
 *
 * @param file Pointer to the utf8_file_t to reset.
 */
void    reset_utf8_file_status(utf8_file_t *file);

/**
 * @brief Decodes a unicode codepoint from the file.
 *
 *
 * @param file Pointer to the utf8_file_t to read from.
 * @return A codepoint or a negative value for an error.
 */
int32_t read_char_from_utf8_file(utf8_file_t *file);

/**
 * @brief Reads the stuff after a \u in the file and returns a codepoint.
 *
 *
 * @param file Pointer to the utf8_file_t to read from.
 * @return A codepoint or a negative value for an error.
 */
int32_t read_unicode_escape_from_utf8_file(utf8_file_t *file);

/**
 * @brief Reads a byte in a data string. Pass the first hex digit to it.
 *
 *
 * @param file Pointer to the utf8_file_t to read from.
 * @param first_ch The first hex digit.
 * @return the value of the byte.
 */
int32_t read_byte_escape_from_utf8_file(utf8_file_t *file, int32_t first_ch);

#endif

// *** end of file utf8_file.h ***
