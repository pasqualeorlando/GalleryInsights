#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <mysql/mysql.h>
#include "database.h"
#include "../utils/log.h"

extern logger_t logger;

const tables_t tables[] = {
    {"enum_user_type", "CREATE TABLE IF NOT EXISTS enum_user_type(id INT PRIMARY KEY AUTO_INCREMENT, name VARCHAR(15) UNIQUE NOT NULL);"},
    {"enum_expertise", "CREATE TABLE IF NOT EXISTS enum_expertise(id INT PRIMARY KEY AUTO_INCREMENT, level VARCHAR(15) UNIQUE NOT NULL);"},
    {"users", "CREATE TABLE IF NOT EXISTS users ("
	        "user_id INT PRIMARY KEY AUTO_INCREMENT,"
	        "name VARCHAR(100) NOT NULL,"
	        "email VARCHAR(255) UNIQUE NOT NULL,"
	        "password VARCHAR(255) NOT NULL"
        ");"
    },
    {"exhibitions", "CREATE TABLE IF NOT EXISTS exhibitions("
	        "exhibition_id INT PRIMARY KEY AUTO_INCREMENT,"
	        "name VARCHAR(100) NOT NULL,"
            "description TEXT NOT NULL"
        ");"
    },
    {"pieces", "CREATE TABLE IF NOT EXISTS pieces("
	        "piece_id INT PRIMARY KEY AUTO_INCREMENT,"
	        "name VARCHAR(100) NOT NULL,"
            "author VARCHAR(100) NOT NULL,"
	        "year VARCHAR(50) NOT NULL,"
	        "img_link VARCHAR(255) NOT NULL"
        ");"
    },
    {"exhibitions_pieces", "CREATE TABLE IF NOT EXISTS exhibitions_pieces("
            "exhibition_id INT NOT NULL,"
            "piece_id INT NOT NULL,"
            
            "CONSTRAINT fk_piece FOREIGN KEY(piece_id) REFERENCES pieces(piece_id) ON DELETE CASCADE ON UPDATE CASCADE,"
            "CONSTRAINT fk_exhibition FOREIGN KEY(exhibition_id) REFERENCES exhibitions(exhibition_id) ON DELETE CASCADE ON UPDATE CASCADE"
        ");"
    },
    {"pieces_descriptions", "CREATE TABLE IF NOT EXISTS pieces_descriptions("
	        "piece_id INT NOT NULL,"
	        "expertise_level_id INT NOT NULL,"
	        "user_type_id INT NOT NULL,"
	        "description TEXT NOT NULL,"
	
            "CONSTRAINT fk_piece_desc FOREIGN KEY(piece_id) REFERENCES pieces(piece_id) ON DELETE CASCADE ON UPDATE CASCADE,"
            "CONSTRAINT fk_expertise FOREIGN KEY(expertise_level_id) REFERENCES enum_expertise(id) ON UPDATE CASCADE ON DELETE CASCADE,"
            "CONSTRAINT fk_user_type FOREIGN KEY(user_type_id) REFERENCES enum_user_type(id) ON UPDATE CASCADE ON DELETE CASCADE"
        ");"
    }
};

const char* initializing_enums[] = {
    "INSERT IGNORE INTO enum_user_type(name) VALUES ('SINGLE'), ('GENERIC_GROUP'), ('FAMILY'), ('SCHOOL_GROUP');",
    "INSERT IGNORE INTO enum_expertise(level) VALUES ('AMATEUR'), ('STANDARD'), ('EXPERT');"
};
const int initializing_enums_length = 2;

const char* insert_mock_query[] = {
    "INSERT IGNORE INTO exhibitions VALUES (1, 'Il Divin Artista', \"La straordinaria mostra Il Divin Artista svela la vita e il genio di Raffaello, offrendo uno sguardo avvincente sulla sua evoluzione artistica e sulla sua influenza duratura. Attraverso una narrazione coinvolgente, l'esibizione esplora il contesto storico e culturale che ha plasmato l'arte di Raffaello, rivelando il suo impatto transcendentale nel panorama artistico del Rinascimento. Un'immersione affascinante nella mente e nel percorso straordinario di questo maestro dell'arte.\"), (2, 'Vincent van Gogh', \"La straordinaria esposizione offre uno sguardo coinvolgente nella vita e nell'eredità del celebre artista olandese. Attraverso un racconto avvincente, l'esibizione delinea il percorso tumultuoso di Van Gogh, esplorando le influenze che hanno plasmato la sua visione artistica. Un'immersione nel mondo emozionale e creativo di un'icona dell'arte, che ha lasciato un'impronta indelebile nella storia dell'espressionismo.\");",
    "INSERT IGNORE INTO pieces VALUES "
        "(1, 'Lo Sposalizio della Vergine', 'Raffaello Sanzio', '1504', 'https://upload.wikimedia.org/wikipedia/commons/0/06/Raffaello_-_Spozalizio_-_Web_Gallery_of_Art.jpg'),"
        "(2, 'Trionfo di Galatea', 'Raffaello Sanzio', '1512','https://upload.wikimedia.org/wikipedia/commons/2/27/Raphael%27s_Triumph_of_Galatea_02.jpg'),"
        "(3, 'Notte stellata', 'Vincent Van Gogh', '1889', 'https://images.agi.it/pictures/agi/agi/2023/03/27/100233409-4566f2fb-add9-4854-8b10-afad283f20ca.jpg'),"
        "(4, 'Mangiatori di patate', 'Vincent Van Gogh', '1885', 'https://upload.wikimedia.org/wikipedia/commons/2/22/Vincent_van_Gogh_-_The_potato_eaters_-_Google_Art_Project_%285776925%29.jpg');",
    "INSERT IGNORE INTO exhibitions_pieces VALUES (1, 1), (1, 2), (2, 3), (2, 4);",
    "INSERT IGNORE INTO pieces_descriptions VALUES "
        "(1, 2, 1, \"Lo sposalizio della Vergine di Raffaello mostra il matrimonio sacro di Maria e Giuseppe. Colori vivaci, angeli, e un'atmosfera spirituale rendono l'opera affascinante e significativa.\"),"
        "(1, 2, 2, \"Lo sposalizio della Vergine di Raffaello mostra il matrimonio sacro tra Maria e Giuseppe. Nel dipinto, la coppia è al centro, circondata da angeli. I colori vivaci e l'atmosfera spirituale rendono l'opera interessante e significativa.\"),"
        "(1, 2, 4, \"Lo sposalizio della Vergine di Raffaello è un dipinto che mostra il matrimonio di Maria e Giuseppe. Al centro ci sono loro due, circondati da angeli. I colori sono belli e l'opera racconta una storia importante della tradizione cristiana.\"),"
        "(1, 2, 3, \"Lo sposalizio della Vergine di Raffaello mostra il matrimonio di Maria e Giuseppe. Nel dipinto, la coppia è al centro, circondata da angeli. I colori vivaci e l'atmosfera spirituale lo rendono un'opera affascinante sulla storia cristiana.\"),"
        "(1, 3, 1, \"Lo sposalizio della Vergine di Raffaello è un capolavoro rinascimentale che raffigura il matrimonio sacro tra la Vergine Maria e San Giuseppe. Con precisione prospettica, maestria cromatica e dettagli ricchi, l'opera cattura la solennità e la spiritualità di questo momento cruciale nella tradizione cristiana.\"),"
        "(2, 2, 1, \"Il Trionfo di Galatea di Raffaello è un affascinante dipinto che raffigura la dea Galatea circondata da nereidi e tritoni. L'opera emana una sensazione di leggerezza e grazia, con colori vivaci e dettagli incantevoli. È un quadro che cattura la bellezza e l'eleganza della mitologia greca in un modo accessibile e affascinante.\"),"
        "(2, 2, 2, \"Il Trionfo di Galatea di Raffaello mostra una donna circondata da creature marine come pesci e delfini. I colori vivaci e l'atmosfera armoniosa rendono l'opera un esempio affascinante di grazia artistica.\"),"
        "(2, 2, 4, \"Il Trionfo di Galatea di Raffaello è un dipinto che ritrae una donna circondata da creature marine come pesci e delfini. I colori sono vivaci, creando un'atmosfera leggera e armoniosa. L'opera esprime la bellezza attraverso la rappresentazione artistica di una figura femminile trionfante, avvolta dalla natura marina.\"),"
        "(2, 2, 3, \"Il Trionfo di Galatea di Raffaello è un dipinto che presenta una donna circondata da creature marine come pesci e delfini. I colori sono vivaci, creando un'atmosfera leggera e armoniosa. L'opera cattura la bellezza di una figura femminile trionfante, immergendola in un ambiente marino incantevole. È un quadro che può essere apprezzato per la sua grazia artistica e la rappresentazione della natura.\"),"
        "(2, 3, 1, \"Il Trionfo di Galatea di Raffaello è un dipinto straordinario che raffigura la dea Galatea, circondata da creature marine come pesci e delfini. La maestria di Raffaello si riflette nei dettagli vividi e nell'armonia cromatica dell'opera. La composizione elegante e la perfezione anatomica della figura femminile testimoniano l'abilità artistica di Raffaello nel rappresentare la grazia e la bellezza. Il dipinto è un esempio magistrale della sua capacità di combinare mitologia e estetica in un'opera d'arte straordinaria.\"),"
        "(3, 2, 1, \"Notte stellata di Van Gogh è un dipinto incantevole che mostra un cielo notturno vibrante e stellato. I colori sono intensi, e le stelle sembrano danzare nel firmamento. La scena notturna è resa con pennellate audaci e dinamiche, trasmettendo un senso di emozione e movimento. È un'opera che cattura la bellezza del cielo notturno in modo suggestivo e coinvolgente.\"),"
        "(3, 2, 2, \"Notte stellata di Van Gogh è un dipinto affascinante che ritrae un cielo notturno ricco di stelle. I colori sono brillanti e le pennellate accentuano il movimento del cielo. È un'opera che trasmette un senso di magia e meraviglia, catturando la bellezza della notte in modo suggestivo e coinvolgente.\"),"
        "(3, 2, 4, \"Notte stellata di Van Gogh è un dipinto che mostra un cielo notturno con stelle scintillanti. I colori sono vivaci, e le stelle sembrano danzare nel cielo. È un'opera che cattura la bellezza della notte in modo magico e suggestivo, rendendo il cielo notturno un vero spettacolo d'arte.\"),"
        "(3, 2, 3, \"Notte stellata di Van Gogh è un dipinto che mostra un cielo notturno pieno di stelle luminose. I colori sono vivaci e le stelle sembrano brillare come diamanti nel buio. È un'opera che cattura la magia della notte e può ispirare un senso di meraviglia e stupore nella bellezza della natura.\"),"
        "(3, 3, 1, \"Notte Stellata di Van Gogh è un capolavoro che trasporta gli osservatori in un universo di emozioni e colori vibranti. Il dipinto cattura la notte con un cielo stellato in un modo unico, attraverso pennellate audaci e dinamiche. Van Gogh utilizza una tavolozza intensa e contrasti forti per creare un'atmosfera eterea e vibrante. Le stelle sembrano pulsare di vita, mentre la città sottostante è immersa in una quiete serena. L'opera riflette la profonda connessione di Van Gogh con la natura e la sua visione artistica intensa e emotiva.\"),"
        "(4, 2, 1, \"Mangiatori di Patate di Van Gogh mostra contadini intorno a un tavolo che mangiano patate. I colori caldi e le pennellate spesse creano un'atmosfera rustica e familiare. È un quadro che cattura la vita quotidiana in modo semplice e accogliente.\"),"
        "(4, 2, 2, \"Mangiatori di Patate di Van Gogh mostra contadini seduti intorno a un tavolo mentre mangiano patate. I colori sono caldi e le pennellate sono spesse, creando un'atmosfera rustica e accogliente. L'opera cattura la semplicità e la vita quotidiana in modo affascinante.\"),"
        "(4, 2, 4, \"Mangiatori di Patate di Van Gogh mostra contadini intorno a un tavolo che mangiano patate. I colori sono caldi e le pennellate spesse, creando un'atmosfera accogliente e familiare. È un dipinto che rappresenta la vita quotidiana in modo semplice e affascinante.\"),"
        "(4, 2, 3, \"Mangiatori di Patate di Van Gogh ritrae contadini intorno a un tavolo che mangiano patate. I colori sono caldi e il dipinto trasmette un'atmosfera rustica e familiare. È un'opera che cattura la vita quotidiana con semplicità e autenticità.\"),"
        "(4, 3, 1, \"Mangiatori di Patate di Van Gogh è un dipinto che cattura magistralmente la vita quotidiana attraverso il suo stile distintivo. Raffigura contadini intorno a un tavolo, mangiando patate. Van Gogh utilizza pennellate spesse e colori terrosi, creando un senso di gravità e umanità nelle figure. La luce calda e l'attenzione ai dettagli conferiscono al dipinto un'atmosfera intima. L'opera rappresenta la sensibilità unica di Van Gogh nel ritrarre la vita delle persone comuni con una profonda empatia e intensità espressiva.\");"
};

const int insert_mock_query_length = 4;

bool check_database_exists(MYSQL* connection) {
    bool exists = false;

    //get the list of available databases (including the default ones)
    MYSQL_RES *dbs = mysql_list_dbs(connection, DB_NAME);

    //if databases are found, check if there's a database called GalleryInsights
    if(dbs != NULL) {
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(dbs))) {
            if(strcmp(DB_NAME, row[0]) == 0)
                exists = true;
        }
        mysql_free_result(dbs);
    }

    return exists;
}

bool check_tables_exist(MYSQL* connection) {
    bool to_return = true;
    const int len = get_number_of_tables();

    //for each table check if it's into the database and log the corresponding message
    for(int i = 0; i < len; i++) {
        if(check_table_exists(connection, tables[i].name)) {
            logger_log(&logger, INFO, __FILE__, __LINE__, "Table %s found", tables[i].name);
        } else {
            logger_log(&logger, WARNING, __FILE__, __LINE__, "Table %s NOT found. I will create it after checking them all", tables[i].name);
            to_return = false;
        }
    }

    return to_return;
}

bool check_table_exists(MYSQL* connection, const char* table_name) {
    char buffer[100] = {0};

    if(table_name == NULL || strlen(table_name) == 0)
        return false;
    
    //build query to execute
    sprintf(buffer, "SHOW TABLES LIKE '%s';", table_name);
    if(mysql_query(connection, buffer) == 0) {
        MYSQL_RES *tables = mysql_store_result(connection);

        if(tables == NULL)
            return false;

        //check if the table exists
        return mysql_num_rows(tables) == 1;
    } else {
        return false;
    }
}

bool create_tables(MYSQL* connection) {
    const int len = get_number_of_tables();

    //for each table check if it exists. If not create it with its definition
    for(int i = 0; i < len; i++) {
        if(!check_table_exists(connection, tables[i].name)) {
            if(mysql_query(connection, tables[i].definition) != 0) {
                logger_log(&logger, ERROR, __FILE__, __LINE__, "Cannot create %s table", tables[i].name);
                return false;
            } else {
                logger_log(&logger, INFO, __FILE__, __LINE__, "Table %s created", tables[i].name);
            }
        }
    }

    return true;
}

bool init_database(MYSQL* connection) {
    for(int i = 0; i < initializing_enums_length; i++) {
        if(mysql_query(connection, initializing_enums[i]) != 0) {
            return false;
        }
    }

    return true;
}

bool insert_mock(MYSQL* connection) {
    for(int i = 0; i < insert_mock_query_length; i++) {
        if(mysql_query(connection, insert_mock_query[i]) != 0) {
            logger_log(&logger, ERROR, __FILE__, __LINE__, "Cannot query: %s", insert_mock_query[i]);
            return false;
        }
    }

    return true;
}

bool create_entire_database(MYSQL* connection) {
    //create database
    char query[1024] = {0};
    sprintf(query, "CREATE DATABASE %s", DB_NAME);

    if(mysql_query(connection, query) != 0) {
        logger_log(&logger, ERROR, __FILE__, __LINE__, "Cannot create database");
        return false;
    }
    else
        logger_log(&logger, INFO, __FILE__, __LINE__, "Database created. Now i'll create tables");

    //connection to database
    if(mysql_select_db(connection, DB_NAME) != 0)
        return false;

    //create tables
    if(!create_tables(connection))
        return false;
    else
        logger_log(&logger, INFO, __FILE__, __LINE__, "All tables has been created");

    return true;
}

MYSQL* connect_to_database(bool flag_insert_mock) {
    MYSQL* con = mysql_init(NULL);

    if(con == NULL)
      return NULL;
    
    //connect to server (not database)
    if(mysql_real_connect(con, getenv("DB"), "mysql", "mysql", NULL, 0, NULL, CLIENT_MULTI_STATEMENTS) == NULL) {
      mysql_close(con);
      return NULL;
    }

    if(check_database_exists(con)) {
        //if database exists check tables (and create them if needed) and connect
        if(mysql_select_db(con, DB_NAME) == 0) {
            logger_log(&logger, INFO, __FILE__, __LINE__, "Database found, let me check tables");
        } else {
            logger_log(&logger, ERROR, __FILE__, __LINE__, "%s", mysql_error(con));
            mysql_close(con);
            return NULL;
        }
        
        
        if(check_tables_exist(con)) {
            logger_log(&logger, INFO, __FILE__, __LINE__, "All tables has been found");
        } else {
            if(create_tables(con)) {
                logger_log(&logger, INFO, __FILE__, __LINE__, "All tables has been created");
            } else {
                logger_log(&logger, ERROR, __FILE__, __LINE__, "Error while creating tables");
                mysql_close(con);
                return NULL;
            }
        }
    } else {
        //if database doesn't exist create it (including tables) and connect to it
        logger_log(&logger, WARNING, __FILE__, __LINE__, "Database NOT found, let me create it for you :)");
        if(create_entire_database(con)) {
            
            if(mysql_select_db(con, DB_NAME) != 0) {
                mysql_close(con);
                return NULL;
            }
        } else {
            mysql_close(con);
            return NULL;
        }
    }

    //initialize database executing some queries
    if(!init_database(con)) {
        logger_log(&logger, ERROR, __FILE__, __LINE__, "Cannot insert default enum values");
        return NULL;
    } else
        logger_log(&logger, INFO, __FILE__, __LINE__, "Tables initialized");

    //insert mock data if required
    if(flag_insert_mock) {
        if(!insert_mock(con))
            logger_log(&logger, WARNING, __FILE__, __LINE__, "Cannot insert sample data");
        else
            logger_log(&logger, INFO, __FILE__, __LINE__, "Sample data inserted successfully");
    }

    return con;
}

int get_number_of_tables(void) {
    return sizeof(tables) / sizeof(tables_t);
}