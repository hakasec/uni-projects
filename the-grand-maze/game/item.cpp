//Include following header libraries. 

#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <sqlite3.h>

//declare Item structure.
typedef struct Item {
    int modID;
    std::string modName;
    int matID;
    std::string matName;
    int itemID;
    std::string itemName;
    int total_attack;
    int total_strength;
    int total_defence;
    int total_price;
    int total_rarity;       
}
Item;

// function prototypes
Item* getRandomItem(sqlite3*);
int callback(void*, int, char**, char**);

//Inspired from Diana Hintea tutorial 4005CEM. commented for self-understanding.
//Declare function main. Return integer. Two inputs argC and argV.

// int main(int argc, char** argv) 
// { 
//     //Declare structure.
//     sqlite3* DB; 
    
//     //Declare exit as integer and value 0.]
//     int exit = 0;     
    
//     //exit assigned to open function from SQLite3 library. Opens game.DB and uses structure.
//     exit = sqlite3_open("game.db", &DB); 
  
//     //return -1 for unable to open database.
//     if (exit) { 
//         std::cerr << "Error open DB " << sqlite3_errmsg(DB) << std::endl; 
//         return (-1); 
//     } 
//     //Print out if database opens correctly.
//     else
//         std::cout << "Opened Database Successfully!" << std::endl; 
    
//     Item *item = getRandomItem(DB); //Get a random item from the database

// 	// Print out the item details
//     std::cout << item->modName << " ";
//     std::cout << item->matName << " "; 
//     std::cout << item->itemName << std::endl;
    
//     sqlite3_close(DB); 
//     delete item;
//     return (0); 
// }
//inspired from tutorial END

//ITEM GENERATOR
//Callback function
int callback(void *data, int cols, char **values, char **colName) 
{ 
    Item *item = (Item*)data;

	// build item with marshalled values
    item->modID = atoi(values[0]);
    item->modName = values[1];
    item->matID = atoi(values[2]);
    item->matName = values[3];
    item->itemID = atoi(values[4]);
    item->itemName = values[5];
    item->total_attack = atoi(values[6]);
    item->total_strength = atoi(values[7]);
    item->total_defence = atoi(values[8]);
    item->total_price = atoi(values[9]);
    item->total_rarity = atoi(values[10]);
    

    return 0;
} 

Item* getRandomItem(sqlite3 *db)
{

//Declare variables here. Character pointer as SQL
//SQL commands for generating a random item and returning all values associated with it.
std::string sql = "SELECT"
                "`rand_mod`.`ID`, `rand_mod`.`name`," 
                "`rand_mat`.`ID`, `rand_mat`.`name`,"
                "`rand_item`.`ID`, `rand_item`.`name`,"
                "`rand_mod`.`attackAdd` + `rand_mat`.`attackAdd` + `rand_item`.`attackAdd` AS `total_attack`,"
                "`rand_mod`.`strengthAdd` + `rand_mat`.`strengthAdd` + `rand_item`.`strengthAdd` AS `total_strength`,"
                "`rand_mod`.`defenceAdd` + `rand_mat`.`defenceAdd` + `rand_item`.`defenceAdd` AS `total_defence`,"
                "`rand_mod`.`priceAdd` + `rand_mat`.`priceAdd` + `rand_item`.`priceAdd` AS `total_price`,"
                "`rand_mod`.`rarityAdd` + `rand_mat`.`rarityAdd` + `rand_item`.`rarityAdd` AS `total_rarity`"
            "FROM"
                "(SELECT * FROM itemModifier ORDER BY RANDOM() LIMIT 1) as `rand_mod`,"
                "(SELECT * FROM materials ORDER BY RANDOM() LIMIT 1) as `rand_mat`,"
                "(SELECT * FROM items ORDER BY RANDOM() LIMIT 1) AS `rand_item`;";

char *errmsg;
int execute;
Item *item = new Item;
//Line to execute SQL commands and callback routine. 
execute = sqlite3_exec(db, sql.c_str(), callback, item, &errmsg);

//Check if executed correctly. Verification
    if (execute != SQLITE_OK)
        std::cerr << "DB Error! " << errmsg << std::endl; 
    else { 
        std::cout << "Random Items Fetched!" << std::endl; 
    } 

//Return the struct.   
    return item;
}