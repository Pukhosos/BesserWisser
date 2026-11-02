#ifndef BOARD_FEN_ENUMS_SERIALIZATION_RESULT_H
#define BOARD_FEN_ENUMS_SERIALIZATION_RESULT_H

/**
 * @brief Result of serializing a board position into Forsyth–Edwards Notation.
 */
typedef enum {
    /**
     * @brief Board position serialized successfully.
     *
     * The resulting FEN string contains all six fields and has been
     * successfully copied into the destination buffer.
     */
    FEN_SERIALIZATION_OK = 0x00,

    /**
     * @brief `board_state` is NULL.
     *
     * The source board state pointer is NULL. No serialization is performed.
     *
     * @note The destination `fen` buffer remains unchanged.
     */
    FEN_SERIALIZATION_NULL_BOARD_POSITION = 0x01,

    /**
     * @brief `fen` is NULL.
     *
     * The destination FEN string pointer is NULL. No serialization is
     * performed.
     */
    FEN_SERIALIZATION_NULL_FEN = 0x02,

    /**
     * @brief Insufficient serialization buffer capacity.
     *
     * The serialized FEN string does not fit into the internal temporary
     * buffer of size `FEN_BUFFER_SIZE = 96`.
     *
     * @note Buffer overflow is impossible for a valid `BoardState`.
     */
    FEN_SERIALIZATION_BUFFER_TOO_SMALL = 0x03,

    /**
     * @brief Failed to format the halfmove clock field.
     *
     * A call to `snprintf` returned a negative value while formatting the
     * halfmove clock.
     */
    FEN_SERIALIZATION_HALFMOVE_CLOCK_FORMATTING_ERROR = 0x04,

    /**
     * @brief Failed to format the fullmove number field.
     *
     * A call to `snprintf` returned a negative value while formatting the
     * fullmove number.
     */
    FEN_SERIALIZATION_FULLMOVE_NUMBR_FORMATTING_ERROR = 0x05,
} FenSerializationResult;

#endif
