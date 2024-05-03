#ifndef MODEL_H
#define MODEL_H

/**
 * Represents an exhibition
 * 
 * @param exhibition_id id of the exhibition stored into the database
 * @param name name of the exhibition
 * @param description description of the exhibition
*/
typedef struct struct_exhibition_t {
    int exhibition_id;
    char name[100];
    char description[65535];
} exhibition_t;

/**
 * Represents a piece of art
 * 
 * @param piece_id id of the piece stored into the database
 * @param name name of the piece
 * @param author author of the piece
 * @param year publication year
 * @param img_link link of an image representation of the piece
 * @param description description of the piece of art based on user query
*/
typedef struct struct_piece_t {
    int piece_id;
    char name[100];
    char author[100];
    char year[50];
    char img_link[2048];
    char description[65535];
} piece_t;

/**
 * Represents a user of the application
 * 
 * @param user_id id of the user
 * @param name name of the user (including last name)
 * @param email email of the user
 * @param password password of the user (possibily encoded)
*/
typedef struct struct_user_t {
    int user_id;
    char name[100];
    char email[255];
    char password[255];
} user_t;

/* -- ENUM USER TYPE --*/

#define FOREACH_USER_TYPE(USER_TYPE) \
        USER_TYPE(SINGLE)   \
        USER_TYPE(GENERIC_GROUP)  \
        USER_TYPE(FAMILY)   \
        USER_TYPE(SCHOOL_GROUP)  \

#define GENERATE_ENUM_USER_TYPE(ENUM) ENUM,
#define GENERATE_STRING_USER_TYPE(STRING) #STRING,

typedef enum enum_user_type {
    FOREACH_USER_TYPE(GENERATE_ENUM_USER_TYPE)
} enum_user_type_t;

static const char *user_type_string[] = {
    FOREACH_USER_TYPE(GENERATE_STRING_USER_TYPE)
};

#define user_type_string_array_len 4

/* -- ENUM EXPERTISE --*/

#define FOREACH_EXPERTISE(EXPERTISE) \
        EXPERTISE(AMATEUR)   \
        EXPERTISE(STANDARD)  \
        EXPERTISE(EXPERT)   \

#define GENERATE_ENUM_EXPERTISE(ENUM) ENUM,
#define GENERATE_STRING_EXPERTISE(STRING) #STRING,

typedef enum enum_expertise {
    FOREACH_EXPERTISE(GENERATE_ENUM_EXPERTISE)
} enum_expertise_t;

static const char *expertise_string[] = {
    FOREACH_EXPERTISE(GENERATE_STRING_EXPERTISE)
};

#define expertise_string_array_len 4

#endif