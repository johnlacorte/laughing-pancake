/**
 * @file utf8_encoder.h
 * @brief This header file declares stuff for encoding utf8.
 *
 * To use just pass a pointer to a utf8_encoder_t to any function. First
 * call init_utf8_encoder() then give the code point you want to encode
 * with the encode_codepoint_to_utf8() function and read from it using
 * is_utf8_encoder_empty() and read_next_byte_from_encoder(). If
 * read_next_byte_from_encoder() returns ENCODER_ERROR you can get a pointer
 * to the error message with get_utf8_encoder_error_msg().
 */
#ifndef UTF8_ENCODER_H
#define UTF8_ENCODER_H

///Status when utf8_encoder_t can be read from.
#define ENCODER_OK 0

///Status if there is an error.
#define ENCODER_ERROR -1

/**
 * @brief utf8_encoder_t holds the state of the encoder.
 */
typedef struct
{
    int  status;
    int  remaining_bytes;
    int  bytes[4];
    char *error_msg;
} utf8_encoder_t;

/**
 * @brief Initializes a utf8_encoder_t.
 *
 * This could also reset it after triggering an error in a test.
 * @param encoder Pointer to a utf8_encoder_t.
 */
void init_utf8_encoder(utf8_encoder_t *encoder);

/**
 * @brief Returns true if encoder is ready for the next codepoint.
 *
 * This returns true (the preprocessor should read from encoder) if the encoder
 * still holds bytes to return OR if something caused the status to change to
 * ENCODER_ERROR so the preprocessor can read again and set its own error
 * status. That way it doesn't create a situation where the preprocessor
 * gives every single codepoint read from the file but never reading from
 * the encoder.
 * @param encoder Pointer to a utf8_encoder_t.
 * @return True or False
 */
bool is_utf8_encoder_empty(utf8_encoder_t *encoder);

/**
 * @brief Tries to encode utf8.
 *
 * This takes a codepoint and tries to fill an internal array with bytes that
 * can be read from with read_next_byte_from_encoder(); Giving a negative or
 * too big value for ch or if the encoder was not empty will change the
 * encoder's status to ENCODER_ERROR.
 * @param encoder Pointer to a utf8_encoder_t.
 * @param ch Unicode code point.
 */
void encode_codepoint_to_utf8(utf8_encoder_t *encoder, int32_t ch);

/**
 * @brief Read next byte of utf8 from encoder.
 *
 * Returns a byte of utf8 or ENCODER_ERROR. Reading from an empty encoder will
 * set the encoder status to ENCODER_ERROR and return ENCODER_ERROR.
 * @param encoder Pointer to a utf8_encoder_t.
 * @return ENCODER_ERROR or next byte of utf8.
 */
int  read_next_byte_from_encoder(utf8_encoder_t *encoder);

/**
 * @brief Gets error message from utf8_encoder_t.
 *
 * If the status of the encoder is ENCODER_ERROR this returns a char pointer
 * to a string that describes the error. If the status of the encoder is
 * ENCODER_OK it returns a pointer to the string "Encoder okay."
 * @param encoder Pointer to a utf8_encoder_t.
 * @return Pointer to character array with error message.
 */
char *get_utf8_encoder_error_msg(utf8_encoder_t *encoder);

/**
 * @brief Not implemented, returns NULL.
 *
 * When this is finished, it will allocate a char array, encode the string and
 * return the pointer to the character array when finished. This just returns
 * NULL at the moment. I think it will be needed for parser errors.
 * @param encoder Pointer to a utf8_encoder_t.
 * @param length The length of the string passed to the function to encode.
 * @return NULL
 */
char *convert_string_to_utf8(utf8_encoder_t *encoder, size_t length, int32_t *string);

#endif

// *** end of file "utf8_encoder.h" ***
