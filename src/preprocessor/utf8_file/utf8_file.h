/**
 * @file utf8_file.h
 * @brief This header file declares stuff for reading utf8 files.
 *
 * Source files encoded in utf8 are decoded to unicode codepoints for use in
 * the preprocessor before being encoded again to be used for matching tokens
 * and the rest. It seemed like a good way to be absolutely sure of the
 * character encoding of the file and positions in lines being codepoints
 * is slightly better than in bytes. int32_t is used for returning codepoints
 * and other values in place of codepoints because full unicode only takes
 * 21 or 22 bits (I forget how many)and I wanted something signed to return EOF
 * or errors. 
 */
#ifndef UTF8_FILE_H
#define UTF8_FILE_H

///Status of utf8_file_t when it can be read from.
#define UTF8_FILE_OK 0

///Status of utf8_file_t when it reaches the end of file.
#define UTF8_FILE_EOF -1

///Status of utf8_file_t when it encounters an error.
#define UTF8_FILE_ERROR -2

typedef void* utf8_file_t;

/**
 * @brief Opens utf8 file for reading.
 *
 * This uses the same rules for filenames as fopen(). It either needs just the
 * filename if file exists in the current directory or the full path if it
 * doesn't. Checking the return value doesn't matter because any attempt to
 * read if a file failed to open will return UTF8_FILE_ERROR and the error
 * message will have already been set.
 * @param filename The filename to pass to fopen() as is.
 * @return A void pointer to the structure that keeps the state (utf8_file_t).
 */
utf8_file_t open_utf8_file(char *filename);

/**
 * @brief Frees utf8 file.
 *
 * Closes any open files and free any memory that was used.
 * @param file void pointer to the structure that keeps the state(utf8_file_t).
 */
void free_utf8_file(utf8_file_t file);

/**
 * @brief Returns status of utf8_file_t.
 *
 * The return value of this function will be UTF8_FILE_OK, UTF8_FILE_EOF,
 * UTF8_FILE_ERROR, or UTF8_FILE_CLOSED.
 * @param file void pointer to the structure that keeps the state(utf8_file_t).
 * @return A number indicating status of utf8_file_t.
 */
int32_t get_utf8_file_status(utf8_file_t file);

/**
 * @brief Returns a character pointer to a string of the last error.
 *
 * If there is no error this returns a pointer to the string
 * "File open, no errors."
 * @param file void pointer to the structure that keeps the state(utf8_file_t).
 * @return Pointer to error message.
 */
char    *get_utf8_file_error_msg(utf8_file_t file);

/**
 * @brief Returns the line the last character read is on.
 *
 * The count starts at one and increases after every newline.
 * @param file void pointer to the structure that keeps the state(utf8_file_t).
 * @return The number of the current line.
 */
int     get_utf8_file_line_number(utf8_file_t file);

/**
 * @brief Returns the position in the line the last character read is on.
 *
 * The count starts at zero before first read of the file and before first read
 * after a newline and increases after each read of unicode codepoints. Using
 * pop_byte_from_utf8_file() (when testing if malformed utf8 errors are being
 * triggered for example) will lead to the count being off.
 * @param file void pointer to the structure that keeps the state(utf8_file_t).
 * @return The number of codepoints read from the beginning of the line.
 */
int     get_utf8_file_line_position(utf8_file_t file);

/**
 * @brief returns the next byte from the file without decoding it.
 *
 * This function is meant to be used to eat the remaining bytes of a bad utf8
 * sequence in a test. The return value of each call will be the byte read.
 * MAKE SURE TO RESET the utf8_file_t using reset_utf8_file_status() before
 * using this function or it will return UTF8_FILE_ERROR instead of reading
 * any bytes. This function will not change the status unless it encounters
 * an end of file.
 * @param file void pointer to the structure that keeps the state(utf8_file_t).
 * @return UTF8_FILE_EOF, UTF8_FILE_ERROR, or value of the byte. 
 */
int32_t pop_byte_from_utf8_file(utf8_file_t file);

/**
 * @brief Resets the status so you can test another error.
 *
 * Resets status, error message, file position, etc. to what they were after
 * the file successfully opened. 
 * @param file void pointer to the structure that keeps the state(utf8_file_t).
 */
void    reset_utf8_file_status(utf8_file_t file);

/**
 * @brief Decodes a unicode codepoint from the file.
 *
 * Decodes utf8, checks for a LOT of different problems (including Byte Order
 * Mark anywhere but the beginning of the file), and it converts all newlines
 * to the \n type. It stops reading if the end of file is found or some error
 * is encountered and will only return UTF8_FILE_EOF or UTF8_FILE_ERROR from
 * then on.
 * @param file void pointer to the structure that keeps the state(utf8_file_t).
 * @return UTF8_FILE_EOF, UTF8_FILE_ERROR, or a codepoint.
 */
int32_t read_char_from_utf8_file(utf8_file_t file);

/**
 * @brief Reads hex digits in a unicode escape and returns a codepoint.
 *
 * Reads the stuff after \u escape. In WebAssembly these are something like
 * "\u{1af2}", some hex digits between curly braces. This function checks the
 * codepoint with the same checks that codepoints read from utf8 have to pass.
 * An end of file found in an escape will return an error along with normal
 * errors you would expect. 
 * @param file void pointer to the structure that keeps the state(utf8_file_t).
 * @return A codepoint or UTF8_FILE_ERROR for an error.
 */
int32_t read_unicode_escape_from_utf8_file(utf8_file_t file);

/**
 * @brief Reads a byte in a data string. Pass the first hex digit to it.
 *
 * If a hex digit is found after a backslash (a byte escape), the first hex
 * digit can be passed to this function to read the second (byte escapes are
 * exactly two hex digits in WebAssembly) and it returns the value of the byte
 * or UTF8_FILE_ERROR.
 * @param file void pointer to the structure that keeps the state(utf8_file_t).
 * @param first_ch The first hex digit.
 * @return The value of the byte or UTF8_FILE_ERROR.
 */
int32_t read_byte_escape_from_utf8_file(utf8_file_t file, int32_t first_ch);


/**
 * @brief Not implemented, returns zero.
 *
 * When this is finished, it will return the length of a line from a file. This
 * just returns zero at the moment. I think it will be needed for parser errors.
 * @param file void pointer to the structure that keeps the state(utf8_file_t).
 * @param line_number
 * @return zero
 */
size_t get_utf8_file_line_length(utf8_file_t file, int line_number);

/**
 * @brief Not implemented, returns NULL.
 *
 * When this is finished, it will return the line from a file. This just returns
 * NULL at the moment. I think it will be needed for parser errors.
 * @param file void pointer to the structure that keeps the state(utf8_file_t).
 * @param line_number
 * @return NULL
 */
int32_t *get_utf8_file_line_string(utf8_file_t file, int line_number);

#endif

// *** end of file "utf8_file.h" ***
