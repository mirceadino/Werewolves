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
} Role, *PRole;

// Creates a role with no characteristics.
PRole new_role();

// Initializes a role with the corresponding characteristics.
void role_init(PRole role,
               RoleType type,
               char* title,
               char* description,
               int number);

// Frees the memory allocated by the role.
void role_destroy(PRole role);

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
} Player, *PPlayer;

// Creates a player with no characteristics.
PPlayer new_player();

// Gives a name to the player.
void player_init(PPlayer player,
                 char* name);

// Assigns a role to the player and makes him alive.
void player_assign(PPlayer player,
                   RoleType role);

// Kills the player. Returns 1 if the player was successfully killed
// and 0 if the player was already dead.
int player_kill(PPlayer player);

// Frees the memory allocated by the player.
void player_destroy(PPlayer player);

// ================================================
// ===================== GAME =====================
// ================================================

// Definition of the room including players (and their number), roles
// (and their number), and the name of the room.
typedef struct Room {
  int num_players;
  int cap_players;
  PPlayer* players;
  int num_roles;
  PRole* roles;
  char* name;
} Room, *PRoom;

// Creates a room with no characteristics.
PRoom new_room();

// Gives a name to the room and initializes the roles and the number of
// available positions for each role.
void room_init(PRoom room,
               char* name);

// Adds a player to the room. The ownership of the player will belong to
// the room.
void room_add_player(PRoom room,
                     PPlayer player);

// Sets up the room for a new game. Each player is assigned a random
// role.
void room_setup(PRoom room);

// Frees the memory allocated by the room.
void room_destroy(PRoom room);
