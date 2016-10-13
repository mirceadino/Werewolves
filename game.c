#include "game.h"

#include <stdlib.h>
#include <string.h>
#include <time.h>

// =================================================
// ===================== ROLES =====================
// =================================================

Role* new_role() {
  Role* role = malloc(sizeof(Role));
  return role;
}

void role_init(Role* role, RoleType type, char* title, char* description,
               int number) {
  role->type = type;

  role->title = malloc(sizeof(char) * (strlen(title) + 1));
  strcpy(role->title, title);

  role->description = malloc(sizeof(char) * (strlen(description) + 1));
  strcpy(role->description, description);

  role->number = number;
}

void role_destroy(Role* role) {
  free(role->title);
  free(role->description);
  free(role);
}

Player* new_player() {
  Player* player = malloc(sizeof(Player));
  return player;
}

// ===================================================
// ===================== PLAYERS =====================
// ===================================================

void player_init(Player* player, char* name) {
  player->name = malloc(sizeof(char) * (strlen(name) + 1));
  strcpy(player->name, name);
}

void player_assign(Player* player, RoleType role) {
  player->role = role;
  player->state = ALIVE;
}

int player_kill(Player* player) {
  if (player->state == DEAD) {
    return 1;
  }
  player->state = DEAD;
  return 1;
}

void player_destroy(Player* player) {
  free(player->name);
  free(player);
}

Room* new_room() {
  Room* room = malloc(sizeof(Room));
  return room;
}

// ================================================
// ===================== GAME =====================
// ================================================

void room_init(Room* room, char* name) {
  room->name = malloc(sizeof(char) * (strlen(name) + 1));
  strcpy(room->name, name);

  room->num_players = 0;
  room->cap_players = 1;
  room->players = malloc(sizeof(Player*) * (room->cap_players));
  room->num_roles = 4;
  room->roles = malloc(sizeof(Role*) * (room->num_roles));
  for (int i = 0; i < room->num_roles; ++i) {
    room->roles[i] = new_role();
  }
  role_init(room->roles[0], WEREWOLF, "Werewolf",
            "He kills in group.", 3);
  role_init(room->roles[1], SEER, "Seer",
            "He sees and catches werewolves.", 1);
  role_init(room->roles[2], DOCTOR, "Doctor",
            "He cures people.", 1);
  role_init(room->roles[3], POTATO, "Villager",
            "He tries to survive.", 1024);
}

void room_add_player(Room* room, Player* player) {
  if (room->num_players == room->cap_players) {
    room->cap_players *= 2;
    room->players = realloc(room->players,
                            sizeof(Player*) * (room->cap_players));
  }
  room->players[room->num_players++] = player;
}

// Shuffles an array of integers by using Fisher-Yates shuffle.
void shuffle(int* array, int n) {
  srand(clock());
  if (n > 1) {
    int i;
    for (i = 0; i < n - 1; i++) {
      int j = i + rand() / (RAND_MAX / (n - i) + 1);
      int t = array[j];
      array[j] = array[i];
      array[i] = t;
    }
  }
}

void room_setup(Room* room) {
  int* role_array = malloc(sizeof(int) * room->num_players);
  for (int i = 0, k = 0; i < room->num_roles; ++i) {
    Role* role = room->roles[i];
    for (int j = 0; j < role->number && k < room->num_players; ++j) {
      role_array[k++] = i;
    }
  }
  shuffle(role_array, room->num_players);
  for (int i = 0; i < room->num_players; ++i) {
    player_assign(room->players[i], room->roles[role_array[i]]->type);
  }
  free(role_array);
}

void room_destroy(Room* room) {
  for (int i = 0; i < room->num_players; ++i) {
    player_destroy(room->players[i]);
  }
  for (int i = 0; i < room->num_roles; ++i) {
    role_destroy(room->roles[i]);
  }
  free(room->name);
  free(room);
}
