#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 64
#define MAX_NEIGHBOURS 4
#define MAX_CHILDS 4
#define MAX_PARENTS 1

typedef enum ObjType {
  ROOM,
  CAR,
  DOG,
  TV,
  PERSON,
  HOOD,
  PARKING_LOT,
  TYPE_COUNT
} ObjType_t;

const char *enum_to_string(ObjType_t Type) {
  switch (Type) {
  case ROOM:
    return "ROOM";
    break;
  case CAR:
    return "CAR";
    break;
  case DOG:
    return "DOG";
    break;
  case TV:
    return "TV";
    break;
  case PERSON:
    return "PERSON";
    break;
  case HOOD:
    return "HOOD";
    break;
  case PARKING_LOT:
    return "PARKING_LOT";
    break;
  default:
    return "NONE";
    break;
  }
}
typedef struct funcs {
  void (*act)(void *);
  void (*deleter)(void *);
  void (*adder)(void *);
} Funcs_t;

typedef struct Object {
  ObjType_t type;
  char name[MAX_NAME];
  Funcs_t funcs;
  struct Object *parent_objects[MAX_PARENTS];
  struct Object *child_objects[MAX_CHILDS]; /* Four different childs possible */
  struct Object *neighbour_objects[MAX_NEIGHBOURS]; /* Four different neighbours
                                                       possible */
} Object_t;

/* Linked list of Objects_t */

typedef struct Node {
  Object_t *object;
  struct Node *next;
} Node_t;

Node_t *Objects[TYPE_COUNT];

Object_t *add_node(Node_t **node);

/* Returns the new objects so that we initialize it after calling add_node s*/
Object_t *add_node(Node_t **head) {
  if (!head)
    return NULL;

  Node_t *new_node = calloc(1, sizeof(Node_t));
  if (!new_node)
    return NULL;

  new_node->object = calloc(1, sizeof(Object_t));
  if (!new_node->object) {
    free(new_node);
    return NULL;
  }

  if (*head == NULL) {
    // List is empty: set new node as head
    *head = new_node;
  } else {
    // Traverse to end and append
    Node_t *current = *head;
    while (current->next)
      current = current->next;
    current->next = new_node;
  }

  return new_node->object;
}

Object_t *find_object_by_name(Node_t *node, const char *name);
Object_t *find_object_by_name(Node_t *node, const char *name) {
  while (node) {
    if (strcmp(node->object->name, name) == 0)
      return node->object;
    node = node->next;
  }
  return NULL;
}
Object_t *create_object(char *name, ObjType_t Type);

void add_neighbor_obj(Object_t *obj1, Object_t *obj2) {
  for (int i = 0; i < MAX_NEIGHBOURS; i++) {
    if (obj1->neighbour_objects[i] == NULL) {
      obj1->neighbour_objects[i] = obj2;
      break;
    }
  }
}
void add_parent_obj(Object_t *obj1, Object_t *obj2) {
  for (int i = 0; i < MAX_PARENTS; i++) {
    if (obj1->parent_objects[i] == NULL) {
      obj1->parent_objects[i] = obj2;
      break;
    }
  }
}
void add_child_obj(Object_t *obj1, Object_t *obj2) {
  for (int i = 0; i < MAX_CHILDS; i++) {
    if (obj1->child_objects[i] == NULL) {
      obj1->child_objects[i] = obj2;
      break;
    }
  }
}

Object_t *create_object(char *name, ObjType_t Type) {
  if (Type >= TYPE_COUNT) {
    printf("No such type\n");
    return NULL;
  }
  Object_t *obj = add_node(&Objects[Type]);
  if (!obj) {
    printf("Can't create obj\n");
    return NULL;
  }
  obj->type = Type;
  strcpy(obj->name, name);
  return obj;
}

void print_node(Node_t *node) {
  printf("%s ", node->object->name);
  Object_t *first_neighbour = node->object->neighbour_objects[0];

  if (first_neighbour) {
    printf("( Neighbours: %s", first_neighbour->name);
    for (int i = 1; i < MAX_NEIGHBOURS; i++) {
      if (node->object->neighbour_objects[i] != NULL) {
        printf(" , %s ", node->object->neighbour_objects[i]->name);
      } else {
        printf(" )");
        break;
      }
    }
  }
  printf(" -> ");
}
void print_list(Node_t *head) {
  if (!head) {
    return; /*printf("List empty");*/
  }
  Node_t *node = head;
  while (node) {
    print_node(node);
    node = node->next;
  }
  printf("\n");
}
void print_objects() {
  printf("TRACE %d\n", __LINE__);
  for (int i = 0; i < TYPE_COUNT; i++) {
    print_list(Objects[i]);
  }
}

void print_objects_by_type(ObjType_t Type) {
  Node_t *node = Objects[Type];
  printf("Printing %s\n ##", enum_to_string(Type));
  print_list(node);
}

void delete_list(Node_t **head) {
  Node_t *curr_head = *head;
  while (curr_head) {
    Node_t *next = curr_head->next;
    // printf("Deleting %s : %s\n", enum_to_string(curr_head->object->type),
    // curr_head->object->name);
    free(curr_head);
    curr_head = next;
  }
  *head = NULL;
}
void delete_all() {
  for (int i = 0; i < TYPE_COUNT; i++) {
    delete_list(&Objects[i]);
  }
}
void swap_objects(Object_t *obj1, Object_t *obj2) {
  if (obj1->type != obj2->type) {
    printf("\n Can't swap different object types.\n");
    return;
  }

  Node_t *npo1 = NULL;  // Node before obj1
  Node_t *npo2 = NULL;  // Node before obj2
  Node_t *node1 = NULL; // Node containing obj1
  Node_t *node2 = NULL; // Node containing obj2
  Node_t **head =
      &Objects[obj1->type]; // Pointer to head (to modify it if needed)
  Node_t *node = *head;

  // Find node1, node2, and their predecessors
  while (node && (!node1 || !node2)) {
    if (node->object == obj1) {
      node1 = node;
    } else if (node->next && node->next->object == obj1) {
      npo1 = node;
      node1 = node->next;
    }

    if (node->object == obj2) {
      node2 = node;
    } else if (node->next && node->next->object == obj2) {
      npo2 = node;
      node2 = node->next;
    }

    node = node->next;
  }

  // Early exit if either object is not found
  if (!node1 || !node2) {
    printf("\n One or both objects not found in the list.\n");
    return;
  }

  // Case 1: obj1 and obj2 are the same node
  if (node1 == node2)
    return;

  // Case 2: obj1 is the head
  if (node1 == *head) {
    *head = node2; // Update head to node2
  }
  // Case 3: obj2 is the head
  else if (node2 == *head) {
    *head = node1; // Update head to node1
  }

  // Case 4: Swap node1 and node2
  if (npo1)
    npo1->next = node2;
  if (npo2)
    npo2->next = node1;

  // Swap the next pointers
  Node_t *tmp = node1->next;
  node1->next = node2->next;
  node2->next = tmp;
}
int main(void) {

  Object_t *pers1 = create_object("Mamadou", PERSON);
  Object_t *pers2 = create_object("Dembele", PERSON);
  Object_t *pers3 = create_object("Dali", PERSON);
  Object_t *pers4 = create_object("Hamma", PERSON);
  Object_t *pers5 = create_object("Jean", PERSON);
  Object_t *car1 = create_object("206", CAR);
  Object_t *car2 = create_object("Fiat", CAR);
  Object_t *car3 = create_object("Nissan", CAR);
  Object_t *car4 = create_object("Porshe Carera", CAR);
  Object_t *car5 = create_object("Hundai", CAR);
  Object_t *car6 = create_object("Ford", CAR);
  Object_t *car7 = create_object("Mitsubichi", CAR);
  add_neighbor_obj(pers1, pers2);
  add_neighbor_obj(car4, car1);

  print_objects_by_type(PERSON);

  delete_all();

  return 0;
}