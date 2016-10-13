// =================================================
// ===================== ROLES =====================
// =================================================

// The main roles in the game.
typedef enum RoleType {
  WEREWOLF,
  SEER,
  DOCTOR,
  POTATO
} RoleType;

// Definition of a role including title, description, and number of
// available positions in the game for the role.
typedef struct Role {
  RoleType type;
  char* title;
  char* description;
  int number;
} Role;

// Creates a role with no characteristics.
Role* new_role();

// Initializes a role with the corresponding characteristics.
void role_init(Role* role, RoleType type, char* title, char* description,
               int number);

// Frees the memory allocated by the role.
void role_destroy(Role* role);

// ===================================================
// ===================== PLAYERS =====================
// ===================================================

// State of being of a player.
typedef enum State {
  ALIVE,
  DEAD
} State;

// Definition of the player including name, role, and current state.
typedef struct Player {
  RoleType role;
  char* name;
  State state;
} Player;

// Creates a player with no characteristics.
Player* new_player();

// Gives a name to the player.
void player_init(Player* player, char* name);

// Assigns a role to the player and makes him alive.
void player_assign(Player* player, RoleType role);

// Kills the player. Returns 1 if the player was successfully killed
// and 0 if the player was already dead.
int player_kill(Player* player);

// Frees the memory allocated by the player.
void player_destroy(Player* player);

// ================================================
// ===================== GAME =====================
// ================================================

// Definition of the room including players (and their number), roles
// (and their number), and the name of the room.
typedef struct Room {
  int num_players;
  int cap_players;
  Player** players;
  int num_roles;
  Role** roles;
  char* name;
} Room;

// Creates a room with no characteristics.
Room* new_room();

// Gives a name to the room and initializes the roles and the number of
// available positions for each role.
void room_init(Room* room, char* name);

// Adds a player to the room. The ownership of the player will belong to
// the room.
void room_add_player(Room* room, Player* player);

// Sets up the room for a new game. Each player is assigned a random
// role.
void room_setup(Room* room);

// Frees the memory allocated by the room.
void room_destroy(Room* room);
