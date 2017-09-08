#include <libcgc.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <ctype.h>

#define OUNCES	1
#define GRAMS	2

#define PANTRY_COUNT 128
#define RECIPE_COUNT 512

typedef struct instruction {
	char text[128];

	/// 4 byte cookie from magic page
	size_t cookie;

	struct instruction *next;
} instruction, *pinstruction;

typedef struct ingredient {
	double amount;
	double cost_per_unit;

	// Ounces or grams
	int type;

	char name[16];

	/// 4 byte cookie from magic page
	size_t cookie;

	struct ingredient *next;
} ingredient, *pingredient;

typedef struct pantry {
	char name[16];
	size_t cookie;

	pingredient grocery_list;
} pantry, *ppantry;

typedef struct recipe {
	char name[16];

	/// 4 byte cookie from magic page
	size_t cookie;

	pingredient ing_list;
	pinstruction ins_list;
} recipe, *precipe;

/// This is done to force ordering
typedef struct global {
	pantry pantry_g[PANTRY_COUNT];
	recipe recipe_list_g[RECIPE_COUNT];
} global, *pglobal;

global g;


size_t magic_index_g;
char *magic_page_g;

int read_selection( void )
{
	int value = 2;
	char data[2];
	size_t bytes = 0;
	size_t index = 0;

	while ( value ) {
		if ( receive( STDIN, data + index, value, &bytes) != 0 ) {
			__printf("[ERROR] Failed to __read data\n");
			_terminate(-1);
		}

		if ( bytes == 0 ) {
			__printf("[ERROR] Error with reading selection\n");
			_terminate(-2);
		}

		index += bytes;
		value -= bytes;
	}

	if ( !__isdigit(data[0]) ) {
		__printf("[ERROR] Selection must be a digit\n");
		return 0;
	}

	if ( data[1] != '\n') {
		__printf("[ERROR] Only single digits allowed in the selection\n");
		return 0;
	}

	value = __atoi( data );

	return value;
}

int read_string( char *dest, size_t maxlen ) 
{
	size_t index = 0;
	size_t bytes_read = 0;
	char c = 0;

	if ( dest == NULL ) {
		return index;
	}

	while ( index < maxlen ) {
		if ( receive( STDIN, &c, 1, &bytes_read) != 0 ) {
			__printf("[ERROR] Failed to __read string\n");
			_terminate(-3);
		}

		if ( bytes_read == 0 ) {
			__printf("[ERROR] String __read failure\n");
			_terminate(-4);
		}

		if ( c == '\n') {
			return index;
		}

		dest[index] = c;
		index++;
	}

	return index;
}

size_t read_cookie( )
{
	size_t cookie;

	cookie = ((int *)magic_page_g)[magic_index_g/4];

	magic_index_g += 4;

	return cookie;
}

void display_banner( void )
{
	size_t month = 0;
	size_t year = 0;
	size_t day = 0;
	size_t hour = 0;
	size_t minute = 0;
	size_t second = 0;

	month = ((unsigned char*)magic_page_g)[0] % 12;
	year = 2016;

	switch (month) {
		case 1:
			day = ((unsigned char*)magic_page_g)[10] %29;
			break;
		case 3:
		case 6:
		case 9:
		case 11:
			day = ((unsigned char*)magic_page_g)[10] %30;
			break;
		default:
			day = ((unsigned char*)magic_page_g)[10] % 31;
			break;
	};

	hour = ((unsigned char*)magic_page_g)[20] % 24;
	minute = ((unsigned char*)magic_page_g)[30] % 60;
	second = ((unsigned char*)magic_page_g)[40] % 60;

	__printf("Today: ");

	switch ( ((unsigned char*)magic_page_g)[50] %7 ) {
		case 0:
			__printf("Sun ");
			break;
		case 1:
			__printf("Mon ");
			break;
		case 2:
			__printf("Tue ");
			break;
		case 3:
			__printf("Wed ");
			break;
		case 4:
			__printf("Thu ");
			break;
		case 5:
			__printf("Fri ");
			break;
		case 6:
			__printf("Sat ");
			break;
	}

	switch (month) {
		case 0:
			__printf("Jan ");
			break;
		case 1:
			__printf("Feb ");
			break;
		case 2:
			__printf("Mar ");
			break;
		case 3:
			__printf("Apr ");
			break;
		case 4:
			__printf("May ");
			break;
		case 5:
			__printf("Jun ");
			break;
		case 6:
			__printf("Jul ");
			break;
		case 7:
			__printf("Aug ");
			break;
		case 8:
			__printf("Sep ");
			break;
		case 9:
			__printf("Oct ");
			break;
		case 10:
			__printf("Nov ");
			break;
		case 11:
			__printf("Dec ");
			break;
	}

	__printf("$.2d:$.2d:$.2d $d\n\n", hour, minute, second, year);

	return;
}

pinstruction instruction_prompt()
{
	pinstruction pins = NULL;

	pins = (pinstruction)__malloc( sizeof( instruction) );

	if ( pins == NULL ) {
		return pins;
	}

	__bzero( pins, sizeof(instruction));

	__printf("Enter Instruction: ");
	if ( read_string( pins->text, 127) == 0 ) {
		__free(pins);
		return NULL;
	}

	if ( __strcmp( pins->text, "done") == 0 ) {
		__free(pins);
		return NULL;
	}

	pins->cookie = read_cookie();

	return pins;
}

void recipe_menu( )
{
	__printf("1) Add Recipe\n");
	__printf("2) Remove Recipe\n");
	__printf("3) List Recipe\n");
	__printf("4) List All\n");
	__printf("5) Print Name\n");
	__printf("6) Recipe Costs\n");
	__printf("7) Leave Menu\n");
	__printf("-> ");

	return;
}

pingredient ingredient_prompt( )
{
	pingredient ping = NULL;
	char data[6];

	ping = ( pingredient )__malloc( sizeof( ingredient) );

	if ( ping == NULL ) {
		return ping;
	}

	__bzero( ping, sizeof(ingredient) );

	__printf("Enter ingredient name or a \"done\" to quit.\n");
	__printf(": ");

	if ( read_string( ping->name, 15) == 0 ) {
		__free( ping );
		return NULL;
	}

	if ( __strcmp( ping->name, "done") == 0 ) {
		__free(ping);
		return NULL;
	}

	__bzero( data, 6);

	__printf("Enter the amount: ");
	if ( read_string( data, 5) == 0 ) {
		__printf("[ERROR] Invalid amount\n");
		__free(ping);
		return NULL;
	}

	ping->amount = __atof( data );

	__bzero( data, 6);

	__printf("Enter the cost per unit: ");
	if ( read_string( data, 5) == 0 ) {
		__printf("[ERROR] Invalid cost\n");
		__free(ping);
		return NULL;
	}

	ping->cost_per_unit = __atof( data );

	__printf("Enter the type (oz/gram): ");

	__bzero( data, 6 );
	if ( read_string( data, 5 ) == 0 ) {
		__printf("[ERROR] Failed to __read the measurement type.\n");
		__free(ping);
		return NULL;
	}

	if ( __strcmp( data, "oz" ) == 0 ) {
		ping->type = OUNCES;
	} else if ( __strcmp( data, "gram") == 0 ) {
		ping->type = GRAMS;
	} else { 
		__printf("[ERROR] Invalid type\n");
		__free(ping);
		return NULL;
	}

	return ping;
}

void link_recipe_ingredient( precipe pr, pingredient ping )
{
	if ( ping == NULL || pr == NULL ) {
		return;
	}

	ping->next = pr->ing_list;
	pr->ing_list = ping;

	return;
}

int check_ingredient_exists( pingredient head, pingredient new)
{
	pingredient walker = NULL;

	if ( head == NULL || new == NULL ) {
		return 0;
	}

	walker = head;

	while ( walker ) {
		if ( __strcmp( walker->name, new->name) == 0) {
			__printf("Ingredient already exists. Adding the additional amount.");
			walker->amount += new->amount;
			return 1;
		}

		walker = walker->next;
	}

	return 0;
}

void link_recipe_instruction( precipe pr, pinstruction pins )
{
	pinstruction walker = NULL;

	if ( pins == NULL || pr == NULL ) {
		return;
	}

	/// Base case
	if ( pr->ins_list == NULL ) {
		pr->ins_list = pins;
		return;
	}

	walker = pr->ins_list;

	while ( walker->next ) {
		walker = walker->next;
	}

	walker->next = pins;

	return;
}

void add_recipe( void )
{
	int index = 0;
	precipe pr = NULL;
	pingredient ping = NULL;
	pinstruction pins = NULL;
	size_t t;

	while ( g.recipe_list_g[index].cookie != 0 && index < RECIPE_COUNT) {
		index++;
	}

	if ( index == RECIPE_COUNT ) {
		__printf("[ERROR] Recipe List is full\n");
		return;
	}

	pr = &g.recipe_list_g[index];

	__bzero( pr, sizeof(recipe) );

	__printf("Enter recipe name: ");

	if ( read_string( pr->name, 15 ) == 0 ) {
		__printf("[ERROR] Invalid recipe name\n");
		return;
	}

	t = index;

	/// Ensure that the recipe name does not already exist
	for ( index = 0; index < RECIPE_COUNT; index++) {
		if ( index == t ) {
			continue;
		}

		if ( g.recipe_list_g[index].cookie == 0 ) {
			continue;
		}

		if ( __strcmp( g.recipe_list_g[index].name, g.recipe_list_g[t].name) == 0 ) {
			__printf("[ERROR] Recipe already exists with that name.\n");
			__bzero(pr->name, 16);
			return;
		}
	}

	pr->cookie = read_cookie();

	do {
		ping = ingredient_prompt();

		if ( check_ingredient_exists( pr->ing_list, ping ) ) {
			__free( ping );
		} else {
			link_recipe_ingredient( pr, ping );
		}
	} while ( ping != NULL );


	do {
		pins = instruction_prompt();
		link_recipe_instruction(pr, pins);

	} while ( pins != NULL );

	return;
}


void print_recipe( int index )
{
	pingredient ing_walker = NULL;
	pinstruction ins_walker = NULL;
	size_t item = 1;

	if ( RECIPE_COUNT <= index ) {
		__printf("[ERROR] Invalid index: $d\n", index);
		return;
	}

	if ( g.recipe_list_g[index].cookie == 0 ) {
		__printf("[ERROR] Empty recipe: $d\n", index+1);
		return;
	}

	__printf("Name: $s\n", g.recipe_list_g[index].name);
	__printf("Ingredients:\n");

	ing_walker = g.recipe_list_g[index].ing_list;

	while ( ing_walker ) {
		__printf("\t$f ", ing_walker->amount);
		if ( ing_walker->type == OUNCES ) {
			__printf("oz\t");
		} else {
			__printf("g\t");
		}

		__printf("$s\n", ing_walker->name);

		ing_walker = ing_walker->next;
	}

	ins_walker = g.recipe_list_g[index].ins_list;

	__printf("\nInstructions:\n");
	while ( ins_walker ) {
		__printf("\t$d) $s\n", item, ins_walker->text);

		item++;
		ins_walker = ins_walker->next;
	}

	__printf("\n\n");

	return;
}

void list_recipe( void )
{
	char data[5];
	int choice = 0;

	__bzero(data, 5 );

	__printf("Recipe Index [1..512]: ");
	if ( read_string( data, 5 ) == 0 ) {
		return;
	}

	choice = __atoi( data );

	if ( choice <= 0 || RECIPE_COUNT < choice)
	{
		__printf( "[ERROR] Must be between 1 and 512\n");
		return;
	}

	print_recipe( choice-1 );

	return;
}

void list_all( void )
{
	int i = 0;

	for ( i = 0; i < RECIPE_COUNT; i++) {
		if ( g.recipe_list_g[i].cookie != 0 ) {
			__printf("Index: $d\n", i+1);
			print_recipe( i );
		}
	}

	return;
}

void remove_recipe( )
{
	char data[4];
	int choice = 0;

	__bzero(data, 4 );

	list_all();

	__printf("Recipe Index [1..512]: ");
	if ( read_string( data, 4 ) == 0 ) {
		return;
	}

	choice = __atoi( data );

	if ( choice <= 0 || RECIPE_COUNT < choice ) {
		__printf( "[ERROR] Must be between 1 and 512\n");
		return;
	}

	if ( g.recipe_list_g[choice-1].cookie == 0 ) {
		__printf("[ERROR] Empty recipe\n");
		return;
	}

	__bzero( &(g.recipe_list_g[choice-1]), sizeof(recipe) );

	return;

}

void print_recipe_costs( void )
{
	double total = 0.0;
	size_t index = 0;
	pingredient walker = NULL;

	__printf("Recipe Costs:\n");

	for ( index = 0; index < RECIPE_COUNT; index++) {
		total = 0.0;

		if ( g.recipe_list_g[index].cookie == 0 ) {
			continue;
		}

		for ( walker = g.recipe_list_g[index].ing_list; walker != NULL; walker = walker->next) {
			total += walker->cost_per_unit * walker->amount;
		}

		__printf("\t$d) $s - $$.5f\n", index+1, g.recipe_list_g[index].name, total);
	}


	return;
}

void print_recipe_name( void )
{
	char data[5];
	int choice = 0;

	__bzero(data, 5 );

	__printf("Recipe Index [1..512]: ");
	if ( read_string( data, 5 ) == 0 ) {
		return;
	}

	choice = __atoi( data );

#ifdef PATCHED_1
	if ( choice <= 0 || RECIPE_COUNT < choice)
#else
	if ( RECIPE_COUNT < choice )
#endif
	{
		__printf("[ERROR] Invalid index: $d\n", choice);
		return;
	}

	if ( g.recipe_list_g[choice-1].cookie == 0 ) {
		__printf("[ERROR] Empty recipe: $d\n", choice);
		return;
	}

	__printf("Single Name: $s\n", g.recipe_list_g[choice-1].name);

	return;
}

void handle_recipe( )
{
	int choice = 0;

	while ( 1 ) {
		recipe_menu();

		choice = read_selection();

		switch ( choice ) {
			case 1:
				add_recipe();
				break;
			case 2:
				remove_recipe();
				break;
			case 3:
				list_recipe();
				break;
			case 4:
				list_all();
				break;
			case 5:
				print_recipe_name();
				break;
			case 6:
				print_recipe_costs();
				break;
			case 7:
				__printf("Leave\n");
				return;
				break;
			default:
				__printf("Invalid selection.\n");
		};
	}

}

void pantry_menu( )
{
	__printf( "1) Create Pantry\n");
	__printf( "2) Delete Pantry\n");
	__printf( "3) Print Pantry\n");
	__printf( "4) Print All Pantries\n");
	__printf( "5) Update Pantry\n");
	__printf( "6) Leave Menu\n");
	__printf("-> ");

	return;
}

void init_globals( char *secret_page )
{
	__bzero( &g.pantry_g, sizeof(pantry) * PANTRY_COUNT);
	__bzero( &g.recipe_list_g, sizeof(recipe) * RECIPE_COUNT);

	magic_index_g = 0;
	magic_page_g = secret_page;

	return;
}

void link_pantry_ingredient( ppantry pp, pingredient ping )
{
	if ( ping == NULL || pp == NULL ) {
		return;
	}

	ping->next = pp->grocery_list;
	pp->grocery_list = ping;

	return;
}

void print_single_pantry( size_t index )
{
	pingredient pw = NULL;
	size_t item = 1;

	if ( PANTRY_COUNT <= index ) {
		__printf("[ERROR] Invalid index: $d\n", index);
		return;
	}

	if ( g.pantry_g[index].cookie == 0 ) {
		__printf("[ERROR] Empty pantry: $d\n", index+1);
		return;
	}

	__printf("Name: $s\n", g.pantry_g[index].name);
	__printf("Item:\n");

	pw = g.pantry_g[index].grocery_list;

	while ( pw ) {
		__printf("\t$f ", pw->amount);
		if ( pw->type == OUNCES ) {
			__printf("oz\t");
		} else {
			__printf("g\t");
		}

		__printf("$s\n", pw->name);

		pw = pw->next;
	}

	__printf("\n\n");

	return;

}

void print_pantry( void )
{
	char data[4];
	int choice = 0;

	__bzero(data, 4 );

	__printf("Pantry Index [1..128]: ");
	if ( read_string( data, 4 ) == 0 ) {
		return;
	}

	choice = __atoi( data );

	if ( choice <= 0 || PANTRY_COUNT < choice ) {
		__printf( "[ERROR] Must be between 1 and 128\n");
		return;
	}

	print_single_pantry( choice-1 );

	return;
}

void create_pantry( void )
{
	int index = 0;
	ppantry pp = NULL;
	pingredient ping = NULL;
	int t = 0;

	while ( g.pantry_g[index].cookie != 0 && index < PANTRY_COUNT) {
		index++;
	}

	if ( index == PANTRY_COUNT ) {
		__printf("[ERROR] Pantry List is full\n");
		return;
	}

	pp = &g.pantry_g[index];

	__bzero( pp, sizeof(pantry) );

	__printf("Enter pantry name: ");

	if ( read_string( pp->name, 15 ) == 0 ) {
		__printf("[ERROR] Invalid pantry name\n");
		return;
	}

	t = index;

	/// Ensure that the pantry name does not already exist
	for ( index = 0; index < PANTRY_COUNT; index++) {
		if ( index == t ) {
			continue;
		}

		if ( g.pantry_g[index].cookie == 0 ) {
			continue;
		}

		if ( __strcmp( g.pantry_g[index].name, g.pantry_g[t].name) == 0 ) {
			__printf("[ERROR] Pantry already exists with that name.\n");
			__bzero(pp->name, 16);
			return;
		}
	}

	pp->cookie = read_cookie();

	do {
		ping = ingredient_prompt();

		if ( check_ingredient_exists( pp->grocery_list, ping ) ) {
			__free(ping);
		} else { 
			link_pantry_ingredient( pp, ping );
		}
	} while ( ping != NULL );

	return;
}

void list_all_pantries( void )
{
	int i = 0;

	for ( i = 0; i < PANTRY_COUNT; i++) {
		if ( g.pantry_g[i].cookie != 0 ) {
			__printf("Index: $d\n", i+1);
			print_single_pantry( i );
		}
	}

	return;
}

void delete_pantry( void )
{
	char data[4];
	int choice = 0;

	__bzero(data, 4 );

	list_all_pantries();

	__printf("Pantry Index [1..128]: ");
	if ( read_string( data, 4 ) == 0 ) {
		return;
	}

	choice = __atoi( data );

	if ( choice <= 0 || PANTRY_COUNT < choice ) {
		__printf( "[ERROR] Must be between 1 and 128\n");
		return;
	}

	if ( g.pantry_g[choice-1].cookie == 0 ) {
		__printf("[ERROR] Empty pantry\n");
		return;
	}

	__bzero( &(g.pantry_g[choice-1]), sizeof(pantry) );

	return;
}

void update_pantry( void )
{
	char data[16];
	int choice = 0;
	ppantry pp = NULL;
	pingredient ing_walker = NULL;

	__bzero(data, 16 );

	list_all_pantries();

	__printf("Pantry Index [1..128]: ");
	if ( read_string( data, 4 ) == 0 ) {
		return;
	}

	choice = __atoi( data );

	if ( choice <= 0 || PANTRY_COUNT < choice ) {
		__printf( "[ERROR] Must be between 1 and 128\n");
		return;
	}

	if ( g.pantry_g[choice-1].cookie == 0 ) {
		__printf("[ERROR] Empty pantry\n");
		return;
	}

	pp = &g.pantry_g[choice-1];

	print_single_pantry( choice-1 );

	__printf("Which item to update: ");

	__bzero( data, 16 );

	if ( read_string( data, 15) == 0 ) {
		return;
	}

	ing_walker = pp->grocery_list;

	while ( ing_walker ) {
		if ( __strcmp( ing_walker->name, data ) != 0 ) {
			ing_walker = ing_walker->next;
		} else {
			break;
		}
	}

	if ( ing_walker == NULL ) {
		__printf("This pantry does not have $s\n", data );
		return;
	}

	__bzero( data, 16);

	__printf("Enter the amount: ");
	if ( read_string( data, 5) == 0 ) {
		__printf("[ERROR] Invalid amount\n");
		return;
	}

	ing_walker->amount = __atof( data );

	__printf("Enter the type (oz/gram): ");

	__bzero( data, 16 );
	if ( read_string( data, 5 ) == 0 ) {
		__printf("[ERROR] Failed to __read the measurement type.\n");
		return;
	}

	if ( __strcmp( data, "oz" ) == 0 ) {
		ing_walker->type = OUNCES;
	} else if ( __strcmp( data, "gram") == 0 ) {
		ing_walker->type = GRAMS;
	} else { 
		__printf("[ERROR] Invalid type\n");
	}

	return;

}

void handle_pantry( void )
{
	int choice = 0;

	while ( 1 ) {
		pantry_menu();

		choice = read_selection();

		switch (choice) {
			case 1:
				create_pantry();
				break;
			case 2:
				delete_pantry();
				break;
			case 3:
				print_pantry();
				break;
			case 4:
				list_all_pantries();
				break;
			case 5:
				update_pantry();
				break;
			case 6:
				__printf("Leave.\n");
				return;
				break;
			default:
				__printf("Invalid selection\n");
				break;
		};
	}

	return;
}

void query_recipes( void )
{
	size_t index;
	char data[16];
	size_t selection;
	int found;

	/// Recipe ingredient walker
	pingredient riw = NULL;

	/// Pantry ingredient walker
	pingredient piw = NULL;

	__printf("Select a pantry:\n");

	for ( index = 0; index < PANTRY_COUNT; index++) {
		if ( g.pantry_g[index].cookie != 0 ) {
			__printf("$d) $s\n", index+1, g.pantry_g[index].name);
		}
	}

	__printf("-> ");

	if ( read_string( data, 4 ) == 0 ) {
		return;
	}

	selection = __atoi( data );

	if ( selection <= 0 || PANTRY_COUNT < selection) {
		__printf("[ERROR] Invalid selection\n");
		return;
	}

	if ( g.pantry_g[selection-1].cookie == 0 ) {
		__printf("[ERROR] Empty pantry\n");
		return;
	}

	/// Loop through each recipe.
	/// For each ingredient loop through the grocery list of the pantry
	/// If the ingredient is found, check the amount.
	/// If there is enough then check the next ingredient
	///	If not then move on to the next recipe.
	for ( index = 0; index < RECIPE_COUNT; index ++ ) {
		if ( g.recipe_list_g[index].cookie == 0 ) {
			continue;
		}

		found = 0;

		for ( riw = g.recipe_list_g[index].ing_list; riw != NULL; riw = riw->next ) {

			found = 0;
			for ( piw = g.pantry_g[selection-1].grocery_list; piw != NULL && found == 0; piw = piw->next ) {
				if ( __strcmp( piw->name, riw->name) != 0 ) {
					continue;
				}

				if ( piw->amount < riw->amount) {
					continue;
				}

				found = 1;
			}

			if ( found == 0 ) {
				break;
			}

		}

		if ( found != 0 ) {
			__printf("You can make: $s\n", g.recipe_list_g[index].name);
		}

	}

	return;
}

void query_by_price( void )
{
	char data[10];
	double max = 0.0;
	size_t index = 0;
	pingredient walker = NULL;
	double total = 0.0;

	__bzero( data, 10);

	__printf("How much can you spend: ");

	if ( read_string( data, 9 ) == 0 ) {
		return;
	}

	max = __atof( data );

	__printf("You can make: \n");
	for ( index = 0; index < RECIPE_COUNT; index++) {
		total = 0.0;

		if ( g.recipe_list_g[index].cookie == 0 ) {
			continue;
		}

		for ( walker = g.recipe_list_g[index].ing_list; walker != NULL; walker = walker->next) {
			total += walker->cost_per_unit * walker->amount;
		}

		if ( total <= max ) {
			__printf("\t$d) $s\n", index+1, g.recipe_list_g[index].name);
		}
	}
	return;
}

void print_cheapest( void )
{
	double cheapest_value = 0.0;
	size_t cheapest_index = 0;
	size_t index = 0;
	pingredient walker = NULL;
	double total = 0.0;

	for ( index = 0; index < RECIPE_COUNT; index++) {
		total = 0.0;

		if ( g.recipe_list_g[index].cookie == 0 ) {
			continue;
		}

		for ( walker = g.recipe_list_g[index].ing_list; walker != NULL; walker = walker->next) {
			total += walker->cost_per_unit * walker->amount;
		}

		/// If this is the first one then it is automatically the cheapest
		if ( cheapest_index == 0 ) {
			cheapest_index = index+1;
			cheapest_value = total;
		} else if ( total < cheapest_value ) {
			cheapest_value = total;
			cheapest_index = index+1;
		}
	}

	if ( cheapest_index == 0 ) {
		__printf("No cheapest recipe found\n");
	} else {
		__printf("Cheapest: $d at $f\n", cheapest_index, cheapest_value);
	}

	return;

}

void print_expensivest( void )
{
	double expensivest_value = 0.0;
	size_t expensivest_index = 0;
	size_t index = 0;
	pingredient walker = NULL;
	double total = 0.0;

	for ( index = 0; index < RECIPE_COUNT; index++) {
		total = 0.0;

		if ( g.recipe_list_g[index].cookie == 0 ) {
			continue;
		}

		for ( walker = g.recipe_list_g[index].ing_list; walker != NULL; walker = walker->next) {
			total += walker->cost_per_unit * walker->amount;
		}

		/// If this is the first one then it is automatically the most expensive
		if ( expensivest_index == 0 ) {
			expensivest_index = index + 1;
			expensivest_value = total;
		} else if ( expensivest_value < total ) {
			expensivest_value = total;
			expensivest_index = index+1;
		}
	}

	if ( expensivest_index == 0 ) {
		__printf("The most expensive recipe was not found\n");
	} else {
		__printf("Most expensive: $d at $f\n", expensivest_index, expensivest_value);
	}

	return;

}

void query_menu( void )
{
	int value = 0;

	while ( 1 ) {
		__printf("1) List Doable Recipes by Pantry\n");
		__printf("2) List Doable Recipes by Cost\n");
		__printf("3) Print Cheapest Recipe\n");
		__printf("4) Print Most Expensive Recipe\n");
		__printf("5) Leave Menu\n");
		__printf("-> ");

		value = read_selection();

		switch(value) {
			case 1:
				query_recipes();
				break;
			case 2:
				query_by_price();
				break;
			case 3:
				print_cheapest();
				break;
			case 4:
				print_expensivest();
				break;
			case 5:
				__printf("Leaving..\n");
				return;
				break;
			default:
				__printf("[ERROR] Invalid selection\n");
				break;
		};

	}

	return;
}

int __attribute__((fastcall)) main(int secret_page_i, char *unused[]) 
{

	int value = 0;
	size_t eax;
	size_t eip;

	init_globals( (char*)secret_page_i);

	display_banner();

	while ( 1 ) {
		__printf( "1) Recipe Menu\n");
		__printf( "2) Pantry Menu\n");
		__printf( "3) Query Recipe\n");
		__printf( "4) Quit\n");
		__printf( "-> ");
	
		value = read_selection( );

		switch (value) {
			case 1:
				handle_recipe();
				break;
			case 2:
				handle_pantry();
				break;
			case 3:
				query_menu();
				break;
			case 4:
				__printf("Quit\n");
				_terminate(0);
				break;
			default:
				__printf("unknown\n");
				break;
		};
	}
	return 0;
}
