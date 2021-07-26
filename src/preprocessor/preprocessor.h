/**
 * @file preprocessor.h
 * @brief Preprocessor for WebAssembly source files.
 *
 * The preprocessor opens a utf8_file and reads from it. It removes comments,
 * places single spaces between tokens, can be used to read external names,
 * and can be used to read data strings.
 */
#ifndef PREPOCESSOR_H
#define PREPROCESSOR_H

///Status of preproc_t when it can be read from.
#define PREPROC_OK 0

///Status after normal EOF.
#define PREPROC_EOF -1

///Signals the end of an extern_name or data (where you have escape sequences).
#define PREPROC_END_OF_STRING -2

///Error opening file, error allocating memory, error reading file, etc.
#define PREPROC_ERROR -3

/**
 * @brief Holds a pointer to the preprocessor's state.
 *
 * Must be opened with open_preproc() to be used. The preprocessor state is
 * allocated from the heap so make sure to call free_preproc() to make sure
 * files are closed and memory is freed when you are finished.
 */
typedef void* preproc_t;

/**
 * @brief Returns a preproc_t pointer for a file to be preprocessed.
 *
 * This allocates memory needed for preprocessing a file and tries to open
 * the file. If there are any errors allocating memory, opening the file, or
 * reading from the file the different read functions will return
 * PREPROC_ERROR and you can get a char pointer to a string describing the
 * error by calling the get_preproc_error_msg() function.
 * @param filename The name of the file you wish to open.
 * @return a preproc_t pointer or NULL if memory allocation failed.
 */
preproc_t open_preproc(char *filename);

/**
 * @brief Closes utf8_file and frees memory.
 *
 * If passed NULL fro the parameter pre does nothing. It assumes that this
 * is because of failure to allocate memory for the preproceesor.
 * @param pre The preprocessor to free.
 */
void free_preproc(preproc_t pre);

/**
 * @brief Returns the status of the preprocessor.
 *
 * Mainly used to test if everything is working correctly the return value is
 * PREPROC_OK, PREPROC_EOF, PREPROC_END_OF_STRING, or PREPROC_ERROR. 
 * @param pre The preprocessor to check the status of.
 * return Returns the status of preprocessor.
 */
int  get_preproc_status(preproc_t pre);

/**
 * @brief Returns a char pointer to the last error message.
 *
 * If there are no errors it returns a pointer to the string
 * "Preprocessor okay.". If passed NULL for the parameter pre it returns a
 * pointer to the string "Failed to allocate memory for preprocessor."
 * @param pre The preprocessor to get last error message from.
 * @return char pointer to a string that describes error.
 */
char *get_preproc_error_msg(preproc_t pre);

/**
 * @brief Returns the number of the line in the utf8_file being read from.
 *
 * If passed NULL returns -1.
 * @param pre The preprocessor to check the current line.
 * @return The current line number counter.
 */
int  get_preproc_line_number(preproc_t pre);

/**
 * @brief Return the number of codepoints read so far on the current line.
 *
 * If passed NULL returns -1.
 * @param pre The preprocessor to check the current position
 * @return The current line position counter.
 */
int  get_preproc_line_position(preproc_t pre);

/**
 * @brief  Resets the status of the preprocessor to PREPROC_OK.
 *
 * Mainly used for tests sets the status and error message of a preprocessor
 * back to the initial values.
 * @param pre A preprocessor that has encountered a file error.
 */
void reset_preproc_status(preproc_t pre);

/**
 * @brief Returns next byte of preprocessed text from file.
 *
 * Removes comments, and returns tokens one byte at a time seperated by spaces.
 * @param pre The preprocessor to read from.
 * @return A byte of utf8 encoded text or PREPROC_ERROR.
 */
int  read_preproc_char(preproc_t pre);

/**
 * @brief Returns next byte of an external name string.
 *
 * After reading the initial double quote (a double quote followed by a space)
 * you can use this function to return an external name string encoded to utf8
 * one byte at a time followed by PREPROC_END_OF_STRING. Any errors will return
 * PREPROC_ERROR.
 * @param pre The preprocessor to read from.
 * @return A byte of utf8 encoded text or PREPROC_ERROR or PREPROC_END_OF_STRING.
 */
int  read_preproc_extern_name(preproc_t pre);

/**
 * @brief Returns next byte of a data string.
 *
 * After reading the initial double quote (a double quote followed by a space)
 * you can use this function to return an data string encoded to utf8 one byte
 * at a time followed by PREPROC_END_OF_STRING. Any errors will return
 * PREPROC_ERROR.
 * @param pre The preprocessor to read from.
 * @return A byte of utf8 encoded text or PREPROC_ERROR or PREPROC_END_OF_STRING.
 */
int  read_preproc_data_string(preproc_t pre);

/**
 * @brief Not implemented, returns NULL.
 *
 * print_preproc_line() probably makes more sense than this. This just returns
 * NULL at the moment. I think it will be needed for parser errors.
 * @param pre
 * @param line_number
 * @return NULL
 */
char *get_preproc_line_string(preproc_t pre, int line_number);

#endif

// *** end of file "preprocessor.h" ***
