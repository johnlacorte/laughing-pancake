/**
 * @file preprocessor.h
 * @brief
 *
 *
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
 * @brief
 *
 *
 * @param pre
 */
void free_preproc(preproc_t pre);

/**
 * @brief
 *
 *
 * @param pre
 */
int  get_preproc_status(preproc_t pre);

/**
 * @brief
 *
 *
 * @param pre
 * @return
 */
char *get_preproc_error_msg(preproc_t pre);

/**
 * @brief
 *
 *
 * @param pre
 * @return
 */
int  get_preproc_line_number(preproc_t pre);

/**
 * @brief
 *
 *
 * @param pre
 * @return
 */
int  get_preproc_line_position(preproc_t pre);

/**
 * @brief
 *
 *
 * @param pre
 */
void reset_preproc_status(preproc_t pre);

/**
 * @brief
 *
 *
 * @param pre
 * @return
 */
int  read_preproc_char(preproc_t pre);

/**
 * @brief
 *
 *
 * @param pre
 * @return
 */
int  read_preproc_extern_name(preproc_t pre);

/**
 * @brief
 *
 *
 * @param pre
 * @return
 */
int  read_preproc_data(preproc_t pre);

/**
 * @brief Not implemented, returns NULL.
 *
 * When this is finished, it will return the line from a file. This just returns
 * NULL at the moment. I think it will be needed for parser errors.
 * @param file
 * @param line_number
 * @return NULL
 */
char *get_preproc_line_string(preproc_t pre, int line_number);

#endif

// *** end of file "preprocessor.h" ***
