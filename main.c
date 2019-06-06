#include <stdio.h> // printf
#include <string.h> // strcmp, strtok, strlwr
#include <ctype.h> // tolower
#include <stdbool.h> // defines bool, true, false
#include <assert.h> // assert
#include <stdlib.h> // atoi
#include "libkputils.h"

typedef struct sentence_t {
	char subject[12], object[12], sub_adjective[6], obj_adjective[6], preposition[6], verb[12];
} sentence_t;

//Some git testing

//========== Rooms

struct room_t;

typedef struct room_t {
	char name[12], description[254];
	bool visited;
	void (*enter)(struct room_t*);
} room_t;

void list_rooms(room_t temp[], int room_count) {
	printf("There are the following rooms: ");
	for (int p = 0; p < room_count; p++ ){
		if ( p < (room_count - 1)) {
			printf( "%s, " , temp[p].name);
		} else {
			printf( "and %s.\n" , temp[p].name);
		}
	}
}

void list_visited_rooms(room_t temp[], int room_count) {
	printf("You have visited the following rooms: ");
	for (int p = 0; p < room_count ; p++ ){
		if (temp[p].visited) {			
			printf( "%s, " , temp[p].name);
		}
	}
	printf("\n");
}

//========== Objects

typedef struct object_t {
	char name[12], description[254], adjective[12];
	int room;
} object_t;


void list_objects_at(object_t temp[], int ob_count, int room ) {
  for (int i=0; i < ob_count; i++) {
    if (temp[i].room == room) {
      printf("You see a %s \n", temp[i].name);
    }
  }
}

void get_object(object_t temp[], int ob_count, int room, int bag, char item[]) {
  printf("\n Item is: %s \n", item);
  for (int i=0; i < ob_count; i++) {
    if ((temp[i].room == room) && ((strcmp(temp[i].name, item) == 0) )) {
      temp[i].room = bag;
      
    }
  }
}

void show_subject(object_t temp[], int ob_count, int room, sentence_t sentence) {
	int sub_count = 0;
	for (int i=0; i < ob_count; i++) {
		if (strcicmp(sentence.subject, temp[i].name) == 0) {   //no adjective
			sub_count++;
		}
	}
	printf("\n sub count:  %d \n", sub_count);

	// 1 item in list no adjective. 
	if (sub_count == 1) {  //Just once item
		if (strcicmp(sentence.sub_adjective, "") == 0) { 
			for (int i=0; i < ob_count; i++) {
				if (strcicmp(sentence.subject, temp[i].name) == 0) { 
					printf("\n 1 item, no adjective: %s", temp[i].name);
				}
			}
		}
	}


	if (sub_count > 1) {  //More than 1 item
		if (strcicmp(sentence.sub_adjective, "") == 0) { 
			for (int i=0; i < ob_count; i++) {
				if (strcicmp(sentence.subject, temp[i].name) == 0) { 
					printf("\n 1 item, no adjective: %s", temp[i].name);
				}
			}
		}
	}

}
	
//========== Routes

enum direction {North=1, East=2, South=3, West=4, Up=5, Down=6};

typedef struct route_t {
	int start, end, reverse, direction, keyID;
	bool locked, visible;
} route_t;

const char* named_direction(int dir) {

	switch(dir) {
	case 1 :
		return "north";
		break;
	case 2 :
		return "east";
		break;
	case 3 :
		return "south";
		break;
	case 4 :
		return "west";
		break;
	case 5 :
		return "up";
		break;
	case 6 :
		return "down";
		break;

	default :
		printf("\nCan't convert direction int to string %d\n", dir);
	}
	return "Failed";
}

/**
\param room is integer, number in rooms array
\param *routes array of routes
\return void 
*/
void list_routes_at(route_t rt[], int rt_count, int room ) {
  for (int i=0; i<rt_count; i++) {
    if (rt[i].start == room) {
      printf("You can go %s: \n", named_direction(rt[i].direction) );			
    }
  }
}

//========== Persons

typedef struct person_t {
	const char *name;
	int room, bag;
} person_t;

//========== Parser


const char *verbs[] = {"go", "quit", "look", "get", "drop", "inv", "put", "examine"};
const char *nouns[] = {"key", "torch", "note", "bag", "north", "east", "south", "west" };
const char *adjectives[] = {"brass", "old", "dusty"};
const char *prepositions[] = {"in", "from"};

void show_sentence(sentence_t s) {
	printf("\nSubject: %s\nObject: %s\nVerb: %s\n:Sub Adjective: %s\n:Obj Adjective: %s\nPreposition: %s\n",s.subject, s.object, s.verb, s.sub_adjective, s.obj_adjective, s.preposition); 
}

sentence_t make_sentence(char input[256]) {
	sentence_t sentence = {"", "", "", "", "", ""};
	bool got_subject = false;
	char *temp = strdup(input);
	char *orig=NULL, *inp=NULL;
	orig = temp; //copy original pointer to free later
	
	while( (inp = strsep(&temp," ")) != NULL ) {
   
	  for (int n=0; n<(int)(sizeof(nouns)/(sizeof(nouns[0]))); n++) {
	    if (strcmp(inp, nouns[n]) == 0) {
	      if (got_subject == false) {
			  strlcpy(sentence.subject, inp, sizeof(sentence.subject) );
			  got_subject = true;
	      } else {
			  strlcpy(sentence.object, inp, sizeof(sentence.object) );
	      }
	    }
	  }
	  
	  for (int a=0; a<(int)(sizeof(adjectives)/(sizeof(adjectives[0]))); a++) {		  
   		  if (strcmp(inp, adjectives[a]) == 0) {
			  if (got_subject == false) {
				  strlcpy(sentence.sub_adjective, inp, sizeof(sentence.sub_adjective) );
			  } else {
				  strlcpy(sentence.obj_adjective, inp, sizeof(sentence.obj_adjective) );
			  }
		  }
	  }
	  
	  for (int p=0; p<(int)(sizeof(prepositions)/(sizeof(prepositions[0]))); p++) {
	    if (strcmp(inp, prepositions[p]) == 0) {
	      strlcpy(sentence.preposition, inp, sizeof(sentence.preposition) );	
	    }
	  }
	
	  for (int v=0; v<(int)(sizeof(verbs)/(sizeof(verbs[0]))); v++) {
	    if (strcmp(inp, verbs[v]) == 0) {
	      strlcpy(sentence.verb, inp, sizeof(sentence.verb) );
	    }
	  }
	}
	free(orig);
	return sentence;	
}

//========== Main

void fnDefault(room_t *this) {
	if (this->visited == false) {
		this->visited = true;
		printf("My first visit to the %s.\n%s.\n", this->name, this->description);
	} else {
		printf("You have been here, the %s, before.\n", this->name);
	}
}

void fnPorch(room_t *this) {
	fnDefault(this);
}

void fnHall(room_t *this) {
	fnDefault(this);
}

/**
\mainpage main The start
*/
int main(void) {

  

/**
\page enums Enumerations
*/
enum room {Porch=0, Hall=1, Kitchen=2, Cellar=3};
int room_count = 4;
room_t *rooms;
rooms = (room_t*)malloc(room_count * sizeof(room_t));

person_t hero = {.name="Flakey", .room=Porch, .bag=9999};
 
strlcpy(rooms[Porch].name, "Porch", sizeof(rooms[Porch].name));
strlcpy(rooms[Porch].description, "A bit draughty", sizeof(rooms[Porch].description));
rooms[Porch].visited = true;
rooms[Porch].enter = &fnPorch;

strlcpy(rooms[Hall].name, "Hall", sizeof(rooms[Hall].name));
strlcpy(rooms[Hall].description, "A bit draughty", sizeof(rooms[Hall].description));
rooms[Hall].visited = false;
rooms[Hall].enter = &fnHall;

strlcpy(rooms[Kitchen].name, "Kitchen", sizeof(rooms[Kitchen].name));
strlcpy(rooms[Kitchen].description, "A bit draughty", sizeof(rooms[Kitchen].description));
rooms[Kitchen].visited = false;
rooms[Kitchen].enter = &fnDefault;

strlcpy(rooms[Cellar].name, "Cellar", sizeof(rooms[Cellar].name));
strlcpy(rooms[Cellar].description, "A bit draughty", sizeof(rooms[Cellar].description));
rooms[Cellar].visited = false;
rooms[Cellar].enter = &fnDefault;


enum object {Note=0, Torch=1, BrassKey=2, IronKey=3};
int object_count = 4;
object_t *objects;
objects = (object_t*)malloc(object_count * sizeof(object_t));

objects[Note].room = Porch;
strlcpy(objects[Note].name, "note", sizeof(objects[Note].name));
strlcpy(objects[Note].description, "Some useful notes", sizeof(objects[Note].description));
strlcpy(objects[Note].adjective, "paper", sizeof(objects[Note].adjective)); 

objects[Torch].room = hero.bag;
strlcpy(objects[Torch].name, "torch", sizeof(objects[Torch].name));
strlcpy(objects[Torch].description, "A useful torch", sizeof(objects[Torch].description));
strlcpy(objects[Torch].adjective, "plastic", sizeof(objects[Torch].adjective)); 

objects[BrassKey].room = hero.bag;
strlcpy(objects[BrassKey].name, "key", sizeof(objects[BrassKey].name));
strlcpy(objects[BrassKey].description, "An brass key", sizeof(objects[BrassKey].description));
strlcpy(objects[BrassKey].adjective, "brass", sizeof(objects[BrassKey].adjective));

objects[IronKey].room = hero.bag;
strlcpy(objects[IronKey].name, "key", sizeof(objects[IronKey].name));
strlcpy(objects[IronKey].description, "A iron key", sizeof(objects[IronKey].description));
strlcpy(objects[IronKey].adjective, "iron", sizeof(objects[IronKey].adjective)); 

enum route {Porch_Hall=0, Hall_Porch=1, Hall_Kitchen=2, Kitchen_Hall=3, Kitchen_Cellar=4, Cellar_Kitchen=5};
int route_count = 6;
route_t *routes;
routes = (route_t*)malloc(route_count * sizeof(route_t));

routes[Porch_Hall].locked = true;
routes[Porch_Hall].visible = true;
routes[Porch_Hall].keyID = 1;
routes[Porch_Hall].start = Porch;
routes[Porch_Hall].end = Hall;
routes[Porch_Hall].reverse = Hall_Porch;
routes[Porch_Hall].direction = North;

routes[Hall_Porch].locked = true;
routes[Hall_Porch].visible =true;
routes[Hall_Porch].keyID = 1;
routes[Hall_Porch].start = Hall;
routes[Hall_Porch].end = Porch;
routes[Hall_Porch].reverse = Porch_Hall;
routes[Hall_Porch].direction = South;

routes[Hall_Kitchen].locked = false;
routes[Hall_Kitchen].visible =true;
routes[Hall_Kitchen].keyID = 2;
routes[Hall_Kitchen].start = Hall;
routes[Hall_Kitchen].end = Kitchen;
routes[Hall_Kitchen].reverse = Kitchen_Hall;
routes[Hall_Kitchen].direction = North;

routes[Kitchen_Hall].locked = false;
routes[Kitchen_Hall].visible = true;
routes[Kitchen_Hall].keyID = 2;
routes[Kitchen_Hall].start = Kitchen;
routes[Kitchen_Hall].end = Hall;
routes[Kitchen_Hall].reverse = Hall_Kitchen;
routes[Kitchen_Hall].direction = South;

routes[Kitchen_Cellar].locked = false;
routes[Kitchen_Cellar].visible =true;
routes[Kitchen_Cellar].keyID = 3;
routes[Kitchen_Cellar].start = Kitchen;
routes[Kitchen_Cellar].end = Hall;
routes[Kitchen_Cellar].reverse = Cellar_Kitchen;
routes[Kitchen_Cellar].direction = Down;

routes[Cellar_Kitchen].locked = false;
routes[Cellar_Kitchen].visible = true;
routes[Cellar_Kitchen].keyID = 3;
routes[Cellar_Kitchen].start = Cellar;
routes[Cellar_Kitchen].end = Kitchen;
routes[Cellar_Kitchen].reverse = Kitchen_Cellar;
routes[Cellar_Kitchen].direction = Up;

//list_rooms(rooms, room_count);
//list_visited_rooms(rooms, room_count);



char input[256];
sentence_t sentence;
do {
	puts("\n?> ");
	fgets(input, sizeof(input), stdin); //fgets adds \n
	input[strcspn(input, "\n")] = '\0'; // replace with \O
	sentence = make_sentence(input);
	show_sentence(sentence);

	if (strcmp(sentence.verb, "look") == 0) {
		list_objects_at(objects, object_count, hero.room);
		list_routes_at(routes, route_count, hero.room);
	};

	if (strcmp(sentence.verb, "inv") == 0) {
		list_objects_at(objects, object_count, hero.bag);
	};
	
	if (strcmp(sentence.verb, "get") == 0) {
		//printf("\nSend: %s", sentence.subject); 
		get_object(objects, object_count, hero.room, hero.bag, sentence.subject);

	}

	if (strcmp(sentence.verb, "examine") == 0) {
		puts("\nExamining\n");
		show_subject(objects, object_count, hero.bag, sentence);
	}
	
} while ( (strcicmp(input, "q") != 0) && (strcicmp(input, "quit") != 0) );


free(rooms);
free(objects);
free(routes);

}

