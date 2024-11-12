#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h> // For true and false
#include <ctype.h>   // For isdigit()
#include <iostream>
#include <windows.h>

#define MAX_PLAYERS 4
#define BOARD_SIZE 38
#define INITIAL_MONEY 5000
#define BUILDING_COST 500
#define PENALTY_COST 300
#define BUILDING_RENT_PERCENT 0.10
#define BUILDING_RENT_PERCENT_2 0.20

#define LIGHTBLACK_BG 40
#define LIGHTCYAN_BG 46
#define YELLOW_BG 43
#define LIGHTPURPLE_BG 45
#define LIGHTRED_BG 41
#define LIGHTYELLOW_BG 42
#define LIGHTGREEN_BG 44
#define LIGHTBLUE_BG 34

class Location {
public:
    enum LocationType {
        FREE,
        PROPERTY,
        CHEST,
        CHANCE,
        JAIL,
        TAX,
        UTILITY,
        RAIL
    };

    unsigned int ID;                    // Unique identifier for the location (1-40)
    LocationType type;                  // Type of location: FREE/CHEST/CHANCE/JAIL/TAX/UTILITY/RAIL/PROPERTY
    bool isOwnable;                     // If the location can be owned by a player
    char name[30];                      // Name of the location
    unsigned short int cost;            // Cost to buy the location
    unsigned int setID;                 // Identify properties by set ID
    int setColour;                      // Colour associated with set
    unsigned short int initialRent;     // Initial rent of the property
    unsigned short int house[4];        // Rents with 1-4 houses
    unsigned short int hotel;           // Rent with 1 hotel
    unsigned short int buildCost;       // Cost of building a house or hotel
    unsigned int ownerID;               // If owned, current owner's ID, else 0
    bool isSetComplete;                 // If the set is complete
    unsigned int housesBuilt;           // Amount of houses built on the property
    bool hotelBuilt;                    // If a hotel is built on the property
    int rent;                           // Current rent of the property

    // Constructor to initialize a Location object
    Location(unsigned int ID, LocationType type, bool isOwnable, const char* name, unsigned short int cost,
             unsigned int setID, int setColour, unsigned short int initialRent, const unsigned short int* house,
             unsigned short int hotel, unsigned short int buildCost, unsigned int ownerID,
             bool isSetComplete, unsigned int housesBuilt, bool hotelBuilt, int rent)
            : ID(ID), type(type), isOwnable(isOwnable), cost(cost), setID(setID), setColour(setColour), initialRent(initialRent),
              hotel(hotel), buildCost(buildCost), ownerID(ownerID), isSetComplete(isSetComplete), housesBuilt(housesBuilt),
              hotelBuilt(hotelBuilt), rent(rent) {
        strncpy(this->name, name, sizeof(this->name) - 1);
        this->name[sizeof(this->name) - 1] = '\0';  // Ensure null-termination
        for (int i = 0; i < 4; i++) {
            this->house[i] = house[i];
        }
    }
};

// Example instantiation of the Location array
Location locations[38] = {
        Location(1, Location::FREE, false, "START", 0, 0, 0, 0, new unsigned short int[4]{0, 0, 0, 0}, 0, 0, 0, false,
                 0, false, 0),
        Location(2, Location::PROPERTY, true, "MEDITERRANEAN AVENUE", 60, 2, LIGHTBLACK_BG, 10,
                 new unsigned short int[4]{10, 30, 90, 160}, 250, 50, 0, false, 0, false, 0),
        Location(3, Location::CHEST, false, "COMMUNITY CHEST", 0, 0, 0, 0, new unsigned short int[4]{0, 0, 0, 0}, 0, 0,
                 0, false, 0, false, 0),
        Location(4, Location::PROPERTY, true, "BALTIC AVENUE", 60, 4, LIGHTBLACK_BG, 20,
                 new unsigned short int[4]{20, 60, 180, 320}, 450, 50, 0, false, 0, false, 0),
        Location(5, Location::TAX, false, "INCOME TAX", 0, 0, 0, 0, new unsigned short int[4]{0, 0, 0, 0}, 0, 0, 0,
                 false, 0, false, 0),
        Location(6, Location::RAIL, true, "READING RAILROAD", 200, 25, 0, 0, new unsigned short int[4]{0, 0, 0, 0}, 0,
                 0, 0, false, 0, false, 0),
        Location(7, Location::PROPERTY, true, "ORIENTAL AVENUE", 100, 6, LIGHTCYAN_BG, 30,
                 new unsigned short int[4]{30, 90, 270, 400}, 550, 50, 0, false, 0, false, 0),
        Location(8, Location::CHANCE, false, "CHANCE", 0, 0, 0, 0, new unsigned short int[4]{0, 0, 0, 0}, 0, 0, 0,
                 false, 0, false, 0),
        Location(9, Location::PROPERTY, true, "VERMONT AVENUE", 100, 6, LIGHTCYAN_BG, 30,
                 new unsigned short int[4]{30, 90, 270, 400}, 550, 50, 0, false, 0, false, 0),
        Location(10, Location::PROPERTY, true, "CONNECTICUT AVENUE", 120, 8, LIGHTCYAN_BG, 40,
                 new unsigned short int[4]{40, 100, 300, 450}, 600, 50, 0, false, 0, false, 0),
        Location(11, Location::RAIL, false, "JUST VISITING JAIL", 0, 0, 0, 0, new unsigned short int[4]{0, 0, 0, 0}, 0,
                 0, 0, false, 0, false, 0),
        Location(12, Location::PROPERTY, true, "ST CHARLES PLACE", 140, 10, YELLOW_BG, 50,
                 new unsigned short int[4]{50, 150, 450, 625}, 650, 50, 0, false, 0, false, 0),
        Location(13, Location::UTILITY, false, "UTILITY", 0, 0, 0, 0, new unsigned short int[4]{0, 0, 0, 0}, 0, 0, 0,
                 false, 0, false, 0),
        Location(14, Location::PROPERTY, true, "STATES AVENUE", 140, 10, YELLOW_BG, 50,
                 new unsigned short int[4]{50, 150, 450, 625}, 650, 50, 0, false, 0, false, 0),
        Location(15, Location::TAX, false, "INCOME TAX", 0, 0, 0, 0, new unsigned short int[4]{0, 0, 0, 0}, 0, 0, 0,
                 false, 0, false, 0),
        Location(16, Location::RAIL, true, "PENNSYLVANIA RAILROAD", 200, 25, 0, 0,
                 new unsigned short int[4]{0, 0, 0, 0}, 0, 0, 0, false, 0, false, 0),
        Location(17, Location::PROPERTY, true, "ST JAMES PLACE", 180, 14, LIGHTPURPLE_BG, 70,
                 new unsigned short int[4]{70, 200, 550, 750}, 950, 100, 0, false, 0, false, 0),
        Location(18, Location::CHEST, false, "COMMUNITY CHEST", 0, 0, 0, 0, new unsigned short int[4]{0, 0, 0, 0}, 0, 0,
                 0, false, 0, false, 0),
        Location(19, Location::PROPERTY, true, "TENNESSEE AVENUE", 180, 14, LIGHTPURPLE_BG, 70,
                 new unsigned short int[4]{70, 200, 550, 750}, 950, 100, 0, false, 0, false, 0),
        Location(20, Location::PROPERTY, true, "NEW YORK AVENUE", 200, 16, LIGHTPURPLE_BG, 80,
                 new unsigned short int[4]{80, 220, 600, 800}, 1000, 100, 0, false, 0, false, 0),
        Location(21, Location::FREE, false, "CASSINO", 0, 0, 0, 0, new unsigned short int[4]{0, 0, 0, 0}, 0, 0, 0,
                 false, 0, false, 0),
        Location(22, Location::PROPERTY, true, "KENTUCKY AVENUE", 220, 18, LIGHTRED_BG, 90,
                 new unsigned short int[4]{90, 250, 700, 875}, 1050, 150, 0, false, 0, false, 0),
        Location(23, Location::CHANCE, false, "CHANCE", 0, 0, 0, 0, new unsigned short int[4]{0, 0, 0, 0}, 0, 0, 0,
                 false, 0, false, 0),
        Location(24, Location::PROPERTY, true, "INDIANA AVENUE", 220, 18, LIGHTRED_BG, 90,
                 new unsigned short int[4]{90, 250, 700, 875}, 1050, 150, 0, false, 0, false, 0),
        Location(25, Location::PROPERTY, true, "ILLINOIS AVENUE", 240, 20, LIGHTRED_BG, 100,
                 new unsigned short int[4]{100, 300, 750, 925}, 1100, 150, 0, false, 0, false, 0),
        Location(26, Location::RAIL, true, "B. & O. RAILROAD", 200, 25, 0, 0, new unsigned short int[4]{0, 0, 0, 0}, 0,
                 0, 0, false, 0, false, 0),
        Location(27, Location::PROPERTY, true, "ATLANTIC AVENUE", 260, 22, LIGHTYELLOW_BG, 110,
                 new unsigned short int[4]{110, 330, 800, 925}, 1150, 150, 0, false, 0, false, 0),
        Location(28, Location::PROPERTY, true, "VENTNOR AVENUE", 260, 22, LIGHTYELLOW_BG, 110,
                 new unsigned short int[4]{110, 330, 800, 925}, 1150, 150, 0, false, 0, false, 0),
        Location(29, Location::UTILITY, true, "WATER WORKS", 150, 25, 0, 0, new unsigned short int[4]{0, 0, 0, 0}, 0, 0,
                 0, false, 0, false, 0),
        Location(30, Location::PROPERTY, true, "MARVIN GARDENS", 280, 24, LIGHTYELLOW_BG, 120,
                 new unsigned short int[4]{120, 360, 850, 1025}, 1200, 150, 0, false, 0, false, 0),
        Location(31, Location::JAIL, false, "GO TO JAIL", 0, 0, 0, 0, new unsigned short int[4]{0, 0, 0, 0}, 0, 0, 0,
                 false, 0, false, 0),
        Location(32, Location::PROPERTY, true, "PACIFIC AVENUE", 300, 26, LIGHTGREEN_BG, 130,
                 new unsigned short int[4]{130, 390, 900, 1100}, 1275, 200, 0, false, 0, false, 0),
        Location(33, Location::PROPERTY, true, "NORTH CAROLINA AVENUE", 300, 26, LIGHTGREEN_BG, 130,
                 new unsigned short int[4]{130, 390, 900, 1100}, 1275, 200, 0, false, 0, false, 0),
        Location(34, Location::CHEST, false, "COMMUNITY CHEST", 0, 0, 0, 0, new unsigned short int[4]{0, 0, 0, 0}, 0, 0,
                 0, false, 0, false, 0),
        Location(35, Location::PROPERTY, true, "PENNSYLVANIA AVENUE", 320, 28, LIGHTGREEN_BG, 150,
                 new unsigned short int[4]{150, 450, 1000, 1200}, 1400, 200, 0, false, 0, false, 0),
        Location(36, Location::RAIL, true, "SHORT LINE", 200, 25, 0, 0, new unsigned short int[4]{0, 0, 0, 0}, 0, 0, 0,
                 false, 0, false, 0),
        Location(37, Location::CHANCE, false, "CHANCE", 0, 0, 0, 0, new unsigned short int[4]{0, 0, 0, 0}, 0, 0, 0,
                 false, 0, false, 0),
        Location(38, Location::TAX, false, "INCOME TAX", 0, 0, 0, 0, new unsigned short int[4]{0, 0, 0, 0}, 0, 0, 0,
                 false, 0, false, 0),
};

// Player class also needs some adjustments:
class Player {
public:
    unsigned int ID;
    char name[30];
    unsigned int colour1;
    unsigned int colour2;
    int position;
    int money;
    bool whether_in_game;
    int buildings_owned[BOARD_SIZE];
    int jail_rounds;
    int round = 0;

    // Constructor to initialize a Player object
    Player(unsigned int ID, const char* name, unsigned int colour1, unsigned int colour2, int position,
           int money, bool whether_in_game, int jail_rounds)
            : ID(ID), colour1(colour1), colour2(colour2), position(position), money(money),
              whether_in_game(whether_in_game), jail_rounds(jail_rounds) {
        strncpy(this->name, name, sizeof(this->name) - 1);
        this->name[sizeof(this->name) - 1] = '\0';  // Ensure null-termination
        memset(this->buildings_owned, 0, sizeof(this->buildings_owned));
    }

    // Default constructor
    Player() : ID(0), colour1(0), colour2(0), position(0), money(0), whether_in_game(false), jail_rounds(0) {
        name[0] = '\0';
        memset(buildings_owned, 0, sizeof(buildings_owned));
    }
};


Player players[MAX_PLAYERS];          // Array to store all players in the game
int building_prices[BOARD_SIZE];      // Array to store the prices of buildings
int active_players = 0;               // Variable to track the number of active playersbuildings on the board

void enable_vt_mode() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) {
        return;
    }

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) {
        return;
    }

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!SetConsoleMode(hOut, dwMode)) {
        return;
    }
}

// Function to initialize building prices
void initialize_buildings() {
    for (int i = 0; i < BOARD_SIZE; i++) {
        building_prices[i] = rand() % 291 + 10; // Random price between 10 and 300
    }
}

// Function to calculate and collect rent from owned buildings
//void calculate_building_rent(Player *player, char board[BOARD_SIZE][BOARD_SIZE]) {
//    for (int i = 0; i < BOARD_SIZE; i++) {
//        if (player->buildings_owned[i]) {
//            int rent = (int)(building_prices[i] * BUILDING_RENT_PERCENT);  // 10% rent
//            player->money += rent;
//            printf("%s gained %d from property %c.\n", player->name, rent, board[position_to_row(i)][position_to_col(i)]);
//        }
//    }
//}

int map[38][2] = {
        { 9,36},{ 9,30},{ 9,27},{ 9,24},{ 9,21},{ 9,18},{ 9,15},{ 9,12},{ 9, 9},
        { 9, 6},{16, 6},{23, 6},{30, 6},{37, 6},{44, 6},{51, 6},{58, 6},{65, 6},{72, 6},
        {79, 6},{79, 9},{79,12},{79,15},{79,18},{79,21},{79,24},{79,27},{79,30},
        {79,36},{72,36},{65,36},{58,36},{51,36},{44,36},{37,36},{30,36},{23,36},{16,36}
};

// Function to set the console text color
void colour(int color, int intensity) {
    // intensity = 0 - off, intensity = 1 - on
    if (intensity == 1) {
        printf("\033[1;%dm", color);
    } else {
        printf("\033[0;%dm", color);
    }
}

// Function to reset the console text color
void resetColour() {
    printf("\033[0m");
}

// Function to clear the console screen
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    printf("\033[2J\033[1;1H");
    system("clear");
#endif
}

// Function to set the console cursor position
void goto_XY(int x, int y) {
    printf("\033[%d;%dH", x, y);
}

int DisplayMap() {
    colour(31, 1);
    // clearScreen();
    printf("\n");
    printf("\n                ST.CH  ELCT  STATES VRGNIA PNSLVA  ST.JA   COM    TEN    NY    CASS        ");
    printf("\n                 AVE   COMP    AVE    AVE   RAIL    AVE   CHEST   AVE   AVE    INO         ");
    printf("\n        ____________________________________________________________________________        ");
    printf("\n VISIT |      |      |      |      |      |      |      |      |      |      |      |       ");
    printf("\n  JAIL |      |      |      |      |      |      |      |      |      |      |      |       ");
    printf("\n       |______|______|______|______|______|______|______|______|______|______|______|       ");
    printf("\n CNNCT |      |                                                              |      | KNTCY ");
    printf("\n  AVE  |      |                                                              |      |  AVE  ");
    printf("\n       |______|                                                              |______|       ");
    printf("\n VERMNT|      |                                                              |      |       ");
    printf("\n  AVE  |      |                                                              |      |CHANCE ");
    printf("\n       |______|                                                              |______|       ");
    printf("\n       |      |                                                              |      |  IND  ");
    printf("\n CHANCE|      |                                                              |      |  AVE  ");
    printf("\n       |______|                                                              |______|       ");
    printf("\n ORNTL |      |                                                              |      |  ILL  ");
    printf("\n  AVE  |      |                                                              |      |  AVE  ");
    printf("\n       |______|                                                              |______|       ");
    printf("\n  READ |      |                                                              |      | B&O.  ");
    printf("\n  RAIL |      |                                                              |      | RAIL  ");
    printf("\n       |______|                                                              |______|       ");
    printf("\n       |      |                                                              |      |ATLNTC ");
    printf("\n  TAX  |      |                                                              |      |  AVE  ");
    printf("\n       |______|                                                              |______|       ");
    printf("\n BALTIC|      |                                                              |      | VNTNR ");
    printf("\n  AVE  |      |                                                              |      |  AVE  ");
    printf("\n       |______|                                                              |______|       ");
    printf("\n  COM  |      |                                                              |      | WATER ");
    printf("\n CHEST |      |                                                              |      | WORKS ");
    printf("\n       |______|______________________________________________________________|______|       ");
    printf("\n       |      |      |      |      |      |      |      |      |      |      |      | GO TO ");
    printf("\n START |      |      |      |      |      |      |      |      |      |      |      |  JAIL ");
    printf("\n /CASS |______|______|______|______|______|______|______|______|______|______|______|       ");
    printf("\n  INO           BRDWK LUXURY  PARK  CHANCE  SHORT PNSLVA   COM    NC    PCFC                ");
    printf("\n (afterstart)          TAX    PLACE         LINE    AVE   CHEST   AVE   AVE                 ");

    // Display players on the map
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (players[i].whether_in_game) {
            int pos = players[i].position;
            if (pos >= 1 && pos <= 38) {
                goto_XY(map[pos-1][0], map[pos-1][1]);
                colour(players[i].colour2, 1);
                printf("P%d", players[i].ID);
                resetColour();
            }
        }
    }

    colour(0, 0);
    return EXIT_SUCCESS;
}
//显示玩家信息
int GraphicalPropertyInfo(Player Players[], int playerCount) {
    // Map coordinates
    int map[38][2] = {
            {9, 36}, {9, 30}, {9, 27}, {9, 24}, {9, 21}, {9, 18}, {9, 15}, {9, 12}, {9, 9},
            {9, 6}, {16, 6}, {23, 6}, {30, 6}, {37, 6}, {44, 6}, {51, 6}, {58, 6}, {65, 6}, {72, 6},
            {79, 6}, {79, 9}, {79, 12}, {79, 15}, {79, 18}, {79, 21}, {79, 24}, {79, 27}, {79, 30},
            {79, 36}, {72, 36}, {65, 36}, {58, 36}, {51, 36}, {44, 36}, {37, 36}, {30, 36}, {23, 36}, {16, 36}
    };

    for (int i = 0; i < 38; i++) {
        int ID = locations[i].ID;
        if (locations[i].type == Location::PROPERTY || locations[i].type == Location::RAIL || locations[i].type == Location::UTILITY) {
            // Display basic color block
            if (locations[i].type == Location::PROPERTY) {
                goto_XY(map[ID - 1][0], map[ID - 1][1]);
                colour(locations[i].setColour, 0);
                printf(" ");
                resetColour();
            }

            // Display default information if ownable and no owner
            if (locations[i].isOwnable && locations[i].ownerID == 0) {
                goto_XY(map[ID - 1][0], map[ID - 1][1] + 1);
                resetColour();
                printf("  ");
            }
                // Display owner information if owned
            else if (locations[i].ownerID != 0) {
                for (int j = 0; j < playerCount; j++) {
                    if (locations[i].ownerID == Players[j].ID) {
                        goto_XY(map[ID - 1][0], map[ID - 1][1] + 1);
                        colour(Players[j].colour1, Players[j].colour2);
                        if (locations[i].hotelBuilt) {
                            printf("HL");
                        } else if (locations[i].housesBuilt > 0) {
                            printf("%dH", locations[i].housesBuilt);
                        } else {
                            printf(" X");
                        }
                        resetColour();
                        break;
                    }
                }
            }
        }
    }

    return EXIT_SUCCESS;
}

// Function to graphically move a player on the map此处未被成功调用
int GraphicalMove(Player *player, int OldLocationID, int NewLocationID) {
    if (OldLocationID < 1 || OldLocationID > 38 || NewLocationID < 1 || NewLocationID > 38) {
        return EXIT_FAILURE; // Return failure status if location IDs are out of range
    }

    int playerID = player->ID;
    // Clear the old position
    if (OldLocationID >= 1 && OldLocationID <= 38) {
        goto_XY(map[OldLocationID-1][0] + (playerID-1) % 4, map[OldLocationID-1][1] + (playerID-1) / 4);
        printf(" ");
    }

    // Set the new position with the player's color
    if (NewLocationID >= 1 && NewLocationID <= 38) {
        colour(player->colour2, 1);
        goto_XY(map[NewLocationID-1][0] + (playerID-1) % 4, map[NewLocationID-1][1] + (playerID-1) / 4);
        printf("P%d", playerID);
        resetColour();
    }

    player->position = NewLocationID;
    return EXIT_SUCCESS;
}
// Function to simulate rolling a dice (returns a value between 1 and 6)
int roll_the_dice(void) {
    return rand() % 6 + 1;
}

// Function to initialize player data
void initialize_players(Player players[], int num_players) {
    for (int i = 0; i < num_players; i++) {
        printf("Enter name for player %d: ", i + 1);
        scanf("%s", players[i].name);
        players[i].ID = i + 1;
        players[i].money = INITIAL_MONEY;
        players[i].position = 0;
        players[i].whether_in_game = 1;
        players[i].jail_rounds = 0;  // 初始化 jail_rounds 为 0
        memset(players[i].buildings_owned, 0, sizeof(players[i].buildings_owned));  // Initialize buildings owned
        active_players++;  // Increment active players count
    }
}

// Function to convert position to row index on the board
int position_to_row(int position) {
    return position / BOARD_SIZE;
}

// Function to convert position to column index on the board
int position_to_col(int position) {
    return position % BOARD_SIZE;
}

// Function to move a player on the board by a certain number of steps
void move_player(Player *player, int steps, char board[BOARD_SIZE][BOARD_SIZE]) {
    if (player->whether_in_game == 1) {
        player->position += steps;
        if (player->position >= BOARD_SIZE) {
            player->position %= BOARD_SIZE;
        }
        int row = position_to_row(player->position);
        int col = position_to_col(player->position);
        printf("Player %s moved to position %d (%c)\n", player->name, player->position, board[row][col]);
        printf("Coordinates: (%d, %d)\n", row, col); // Print the coordinates
    }
}
// Function to check if a player is bankrupt
void check_whether_broken(Player *player) {
    if (player->money <= 0) {
        player->whether_in_game = 0;
        printf("Player %s is bankrupt!\n", player->name);
        printf("\nGame Over after %d rounds!\n", player->round);
        printf("player %s ended the game\n" , player->name);
        active_players--;  // Decrement active players count
    }
}

// Function to calculate and collect rent from owned buildings
//void calculate_building_rent(Player *player, char board[BOARD_SIZE][BOARD_SIZE]) {
//    for (int i = 0; i < BOARD_SIZE; i++) {
//        if (player->buildings_owned[i]) {
//            int rent = locations[i].rent;
//            rent = (int)(building_prices[i] * BUILDING_RENT_PERCENT);  // 10% rent
//            player->money -= rent;
//            printf("%s gained %d from property %c.\n", player->name, rent, board[position_to_row(i)][position_to_col(i)]);
//        }
//    }
//}

int ReadLocations() {
    for (int i = 0; i < BOARD_SIZE; i++) {
        Location *loc = &locations[i];  // Pointer to the current location for simplified access

        // Set location type and ownership properties
        switch (loc->type) {
            case Location::FREE:
                loc->isOwnable = false;
                break;
            case Location::CHEST:
                loc->isOwnable = false;
                break;
            case Location::CHANCE:
                loc->isOwnable = false;
                break;
            case Location::JAIL:
                loc->isOwnable = false;
                break;
            case Location::TAX:
                loc->isOwnable = false;
                break;
            case Location::UTILITY:
                loc->isOwnable = true;
                loc->initialRent = 25;  // Set default rent
                loc->cost = 150;  // Set default price
                break;
            case Location::RAIL:
                loc->isOwnable = true;
                loc->initialRent = 25;
                loc->cost = 200;
                break;
            case Location::PROPERTY:
                loc->isOwnable = true;
                loc->isSetComplete = false;
                loc->housesBuilt = 0;
                loc->hotelBuilt = 0;

                // Set color based on setID
                switch (loc->setID) {
                    case 1: loc->setColour = LIGHTBLACK_BG; break;
                    case 2: loc->setColour = LIGHTCYAN_BG; break;
                    case 3: loc->setColour = LIGHTPURPLE_BG; break;
                    case 4: loc->setColour = YELLOW_BG; break;
                    case 5: loc->setColour = LIGHTRED_BG; break;
                    case 6: loc->setColour = LIGHTYELLOW_BG; break;
                    case 7: loc->setColour = LIGHTGREEN_BG; break;
                    case 8: loc->setColour = LIGHTBLUE_BG; break;
                }
                break;
            default:
                loc->isOwnable = false;
                break;
        }

        // Set initial build cost and rent for ownable locations
        if (loc->type == Location::PROPERTY || loc->type == Location::UTILITY || loc->type == Location::RAIL) {
            loc->buildCost = BUILDING_COST;
            loc->rent = loc->initialRent;
        }
    }
    return EXIT_SUCCESS;
}

void apply_tax_effect(Player *player) {
    player->money -= 500;
    printf("Tax Deduction! %s landed on a TAX spot. Lost 500. \n",
           player->name, 500, player->money);
    check_whether_broken(player);  // 检查玩家是否破产
}

void apply_chest_effect(Player *player) {
    int effect = rand() % 3;
    switch (effect) {
        case 0:
            player->money += 200;
            printf("Community Chest! %s received a bonus of 200. New balance: %d\n", player->name, player->money);
            break;
        case 1:
            player->money -= 150;
            printf("Community Chest! %s received a penalty of 150. New balance: %d\n", player->name, player->money);
            check_whether_broken(player);
            break;
        case 2:
            player->position = 0;
            printf("Community Chest! %s goes back to START. New position: %d\n", player->name, player->position);
            break;
    }
}


void apply_chance_effect(Player *player) {
    int effect = rand() % 3;
    switch (effect) {
        case 0:
            player->money += 100;
            printf("Chance! %s received a bonus of 100. New balance: %d\n", player->name, player->money);
            break;
        case 1:
            player->money -= 200;
            printf("Chance! %s received a penalty of 200. New balance: %d\n", player->name, player->money);
            check_whether_broken(player);
            break;
        case 2:
            player->position = (player->position + 3) % BOARD_SIZE;
            printf("Chance! %s moves forward 3 spaces to position %d\n", player->name, player->position);
            break;
    }
}


void send_to_jail(Player *player) {
    player->position = 10; // 监狱位置在 10
    player->jail_rounds = 1; // 设置等待轮数为 1
    printf("Go to Jail! %s is sent to Jail and must wait for 1 round. New position: %d\n",
           player->name, player->position);
}

int is_all_digits(const char *str) {   // Define a function which is used to check whether all input characters
    int z = 0 ;                      // are digits
    int a = 0 ;
    while (*str != '\0') {   // Iterate all the characters before the ending
        if (!isdigit((unsigned char)*str)) {  // Check whether the current character is a digit
            if (*str == '.') {
                a += 1 ;  // If it gets the first '.', then ignore it
            }
            else {
                return false;    // If not, return false
            }
        }
        str++; // Moving to the next character
    }
    if (a == 1 || a == 0) {
        z += 1 ;// If all the characters are numbers(except the '.'), then reutrun z
        return z ;
    }
    else {
        return false; // If there exists more than one '.', then returns false
    }
}

float CASSINO(Player *player)
{
    std::string decision;
    float bet_money;
    char char_bet_money[100];
    long t;
    printf("You are in a cassino. Do you want to bet your money here? Please input 1 = Yes, 0 = No \n");
    std::cin >> decision;
    while (decision != "0" && decision != "1")
    {
        printf("Wrong input! Please input 0 or 1 !\n");
        std::cin >> decision;
    }
    if (decision == "1")
    {
        printf("How much money are you going to bet here? \n");
        std::cin >> char_bet_money;
        // 是否有足够钱？
        sscanf(char_bet_money, "%f", &bet_money);

        while (is_all_digits(char_bet_money) != 1 || bet_money < 0 || bet_money > player->money)
        {
            printf("Wrong input! Please input a positive number and it shouldn't higher than your account balance!\n");
            std::cin >> char_bet_money;
            sscanf(char_bet_money, "%f", &bet_money);
        }

        printf("The casino will roll a dice for you. Do you think the outcome will be greater than 3 or not? Please input 1 = Yes, 0 = No \n");
        std::cin >> decision;
        while (decision != "0" && decision != "1")
        {
            printf("Wrong input! Please input 0 or 1 !\n");
            std::cin >> decision;
        }

        t = time(0);
        srand(t);
        int result = 1+(rand()%6);

        printf("The result is %d !\n", result);

        if (result > 3 && decision == "1")
        {
            player->money += 2 * bet_money;
            printf("You're right ! You win %f dollars! \n", 2 * bet_money);

        }
        else if (result > 3 && decision == "0")
        {
            player->money -= bet_money;
            printf("You're wrong ! You lose %f dollars! \n", bet_money);

        }
        else if (result <= 3 && decision == "1")
        {
            player->money -= bet_money;
            printf("You're wrong ! You lose %f dollars! \n", bet_money);

        }
        else if (result <= 3 && decision == "0")
        {
            player->money += 2 * bet_money;
            printf("You're right ! You win %f dollars! \n", 2 * bet_money);

        }
    }else if (decision == "0")
    {
        printf("You decide not to bet here. Nothing happens here. \n");

    }
    return 0;
}

// Function to check if a player wants to quit the game
bool check_quit_game() {
    int end_signal;
    printf("Do you want to continue the game? Please input 1 = Yes, 0 = No \n");
    scanf("%d", &end_signal);
    return end_signal == 0;
}

// Function to check if a player has won by exceeding 15,000 units
bool check_winner(Player *player) {
    if (player->money > 15000) {
        printf("Player %s wins the game by exceeding 15,000 units!\n", player->name);
        return true;
    }
    return false;
}



// Function to determine the winner of the game
void determine_winner(Player players[], int num_players) {
    int max_money = -1;
    char winner[20];
    for (int i = 0; i < num_players; i++) {
        if (players[i].money > max_money && players[i].whether_in_game) {
            max_money = players[i].money;
            strcpy(winner, players[i].name);
        }
    }
    printf("\nThe winner is %s with %d money!\n", winner, max_money);
}


// Function to play a single turn for a player
void play_turn(Player *player, char board[BOARD_SIZE][BOARD_SIZE]) {
    if (player->whether_in_game == 1) {
        // Check if the player is in jail
        if (player->jail_rounds > 0) {
            printf("\n%s is in Jail and must wait for %d more round(s).\n\n", player->name, player->jail_rounds);
            player->jail_rounds--; // Reduce jail rounds
            return; // Skip this turn
        }

        printf("\n--- It's %s's turn ---\n", player->name);
        int steps = roll_the_dice();
        printf("\n%s rolled a %d!\n", player->name, steps);

        int old_position = player->position;
        move_player(player, steps, board);
        int new_position = player->position;

        Location *current_location = &locations[player->position];
        printf("\nPlayer %s landed on %s (Location ID: %d, Type: %d)\n", player->name,
               current_location->name, current_location->ID, current_location->type);

        // Check ownership of the property
        if (current_location->type == Location::PROPERTY && current_location->isOwnable) {
            if (current_location->ownerID == 0) {
                // Property is not owned, player can choose to buy
                printf("\nYou landed on %s. Do you want to buy it for %d money? (y/n): ",
                       current_location->name, current_location->cost);

                char choice;
                scanf(" %c", &choice);
                fflush(stdin);  // 清空输入缓冲区

                if (choice == 'y' && player->money >= current_location->cost) {
                    player->money -= current_location->cost;
                    current_location->ownerID = player->ID;
                    player->buildings_owned[player->position] = 1;
                    printf("You bought %s. Remaining balance: %d\n", current_location->name, player->money);
                } else if (choice == 'n') {
                    printf("You chose not to buy %s.\n", current_location->name);
                } else {
                    printf("Invalid input. You chose not to buy %s.\n", current_location->name);
                }
            } else if (current_location->isOwnable && current_location->ownerID != player->ID) {
                // Property is owned by another player, pay rent
                int rent = (int)(building_prices[player->position] * BUILDING_RENT_PERCENT);  // 10% rent
                player->money -= rent;

                printf("\nRent Triggered! %s landed on %s owned by Player %d. Paid rent of %d. Remaining balance: %d\n",
                       player->name, current_location->name, current_location->ownerID, rent, player->money);

                // Credit rent to the owner
                for (int i = 0; i < MAX_PLAYERS; i++) {
                    if (players[i].ID == current_location->ownerID) {
                        players[i].money += rent;
                        break;
                    }
                }

                check_whether_broken(player);
            } else {
                // Property is owned by the player themselves, no action
                printf("You own this property. No action needed.\n");
            }
        }
        // Apply special effects for other location types
        switch (current_location->type) {
            case Location::TAX:
                apply_tax_effect(player);
                break;
            case Location::CHEST:
                apply_chest_effect(player);
                break;
            case Location::CHANCE:
                apply_chance_effect(player);
                break;
            case Location::JAIL:
                send_to_jail(player);
                break;
            case Location::FREE:
                CASSINO(player);
                break;
            default:
                break;
        }

        check_whether_broken(player);

        // Display player information
        printf("\n--- Player Information ---\n");
        printf("Name: %s\n", player->name);
        printf("Position: %d\n", player->position);
        printf("Money: %d\n", player->money);
        printf("Buildings Owned: ");
        for (int i = 0; i < BOARD_SIZE; i++) {
            if (player->buildings_owned[i]) {
                printf("%d ", i);
            }
        }
        printf("\n\n");

        // Check if player wants to quit
        if (check_quit_game()) {
            player->whether_in_game = 0;
            active_players--;
            printf("Player %s has quit the game.\n", player->name);
        }

        // Check if player has won by exceeding 15,000 units
        if (check_winner(player)) {
            exit(0);
        }
    }
}


// Function to run the game
void run_game(Player players[], int num_players, int max_rounds, char board[BOARD_SIZE][BOARD_SIZE]) {
    int game_on = 1;
    int round = 1;

    while (game_on && round <= max_rounds) {
        printf("\n\n");
        DisplayMap();  // Draw the map first
        printf("\n\n");
        GraphicalPropertyInfo(players, num_players);  // Display player positions

        printf("\n--- Round %d ---\n\n", round);
        for (int i = 0; i < num_players; i++) {
            if (players[i].whether_in_game) {
                play_turn(&players[i], board);  // Execute player's turn
            }
        }

        // Display player information at the end of each round
        printf("\n--- End of Round %d ---\n", round);
        for (int i = 0; i < num_players; i++) {
            printf("--- Player Information ---\n");
            printf("Name: %s\n", players[i].name);
            printf("Position: %d\n", players[i].position);
            printf("Money: %d\n", players[i].money);
            printf("Buildings Owned: ");
            for (int j = 0; j < BOARD_SIZE; j++) {
                if (players[i].buildings_owned[j]) {
                    printf("%d ", j);
                }
            }
            printf("\n");
        }

        // Check if there are still players in the game
        game_on = 0;
        for (int i = 0; i < num_players; i++) {
            if (players[i].whether_in_game) {
                game_on = 1;
                break;
            }
        }

        // If less than 2 players are left, end the game
        if (active_players < 2) {
            printf("\nNot enough players to continue the game.\n");
            determine_winner(players, num_players);
            return;
        }

        round++;
    }

    printf("\nGame Over after %d rounds!\n", max_rounds);
    determine_winner(players, num_players);  // Determine the winner
}


int main(void) {
    enable_vt_mode();  // Enable VT mode for ANSI escape sequences
    char board[BOARD_SIZE][BOARD_SIZE];
    srand(time(0));  // Seed the random number generator

    int num_players, max_rounds;
    printf("Enter the number of players (2 to 4): ");
    scanf("%d", &num_players);
    while (getchar() != '\n');  // Clear input buffer

    if (num_players < 2 || num_players > MAX_PLAYERS) {
        printf("Invalid number of players. Exiting...\n");
        return 1;
    }

    printf("Enter the maximum number of rounds: ");
    scanf("%d", &max_rounds);
    while (getchar() != '\n');  // Clear input buffer

    Player players[MAX_PLAYERS];
    initialize_players(players, num_players);
    initialize_buildings(); // Initialize building prices
    ReadLocations(); // Initialize locations
    DisplayMap(); // Display the map
    GraphicalPropertyInfo(players, num_players); // Display property information
    run_game(players, num_players, max_rounds, board);  // Start the game

    return 0;
}