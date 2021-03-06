#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <utility>
#include <tuple>

using namespace std;

class FieldEntity {
public:
    string const _type;
    int const _x;
    int const _y;
    int const _radius;
    
    FieldEntity( string type, int x, int y, int radius ) 
        : _type{ type }, _x{ x }, _y{ y }, _radius{ radius } {}
};

class Item {
public:
    string _name;
    int _cost;
    int _dmg;
    int _health;
    int _maxHealth;
    int _mana;
    int _maxMana;
    int _moveSpeed;
    int _manaRegen;
    int _isPotion;
    
    Item& operator=( Item const& ) = default;
    Item( string name, int cost, int dmg, int health, int maxHealth, int mana, int maxMana, int moveSpeed, int manaRegen, bool isPotion ) 
        : _name{ name }, _cost{ cost }, _dmg{ dmg }, _health{ health }, _maxHealth{ maxHealth }, _mana{ mana }, _maxMana{ maxMana }, _moveSpeed{ moveSpeed }, _manaRegen{ manaRegen }, _isPotion{ isPotion } {}
};

class Turn {
public:
    int _gold{ 0 };
    int _enemyGold{ 0 };
    int _roundType{ 0 };
    
    Turn() = default;
    Turn( int gold, int enemyGold, int roundType )
        : _gold{ gold }, _enemyGold{ enemyGold }, _roundType{ roundType } {}
};

class Entity {
public:
    int const _unitId;
    int const _team;
    string const _unitType; // UNIT, HERO, TOWER, can also be GROOT from wood1
    int const _x;
    int const _y;
    int const _attackRange;
    int _health;
    int const _maxHealth;
    int const _shield; // useful in bronze
    int const _attackDamage;
    int const _moveSpeed;
    int const _stunDuration; // useful in bronze
    int const _goldValue;
    int const _countDown1; // all countDown and mana variables are useful starting in bronze
    int const _countDown2;
    int const _countDown3;
    int const _mana;
    int const _maxMana;
    int const _manaRegen;
    string const _heroType; // DEADPOOL, VALKYRIE, DOCTOR_STRANGE, HULK, IRONMAN
    int const _isVisible; // 0 if it isn't
    int const _itemsOwned; // useful from wood1
    
    Entity( int unitId, int team, string unitType, int x, int y, int attackRange, int health
    , int maxHealth, int shield, int attackDamage, int moveSpeed, int stunDuration, int goldValue
    , int countDown1, int countDown2, int countDown3, int mana, int maxMana, int manaRegen, string heroType
    , int isVisible, int itemsOwned )
        : _unitId{ unitId }, _team{ team }, _unitType{ unitType }, _x{ x }, _y{ y }
        , _attackRange{ attackRange }, _health{ health }, _maxHealth{ maxHealth }, _shield{ shield }
        , _attackDamage{ attackDamage }, _moveSpeed{ moveSpeed }, _stunDuration{ stunDuration }
        , _goldValue{ goldValue }, _countDown1{ countDown1 }, _countDown2{ countDown2 }
        , _countDown3{ countDown3 }, _mana{ mana }, _maxMana{ maxMana }, _manaRegen{ manaRegen }
        , _heroType{ heroType }, _isVisible{ isVisible }, _itemsOwned{ itemsOwned } {}
};

auto readInit() {
    int myTeam;
    cin >> myTeam; cin.ignore();
    
    vector< FieldEntity > fieldEntities;
    int bushAndSpawnPointCount; // usefrul from wood1, represents the number of bushes and the number of places where neutral units can spawn
    cin >> bushAndSpawnPointCount; cin.ignore();
    for (int i = 0; i < bushAndSpawnPointCount; i++) {
        string entityType; // BUSH, from wood1 it can also be SPAWN
        int x;
        int y;
        int radius;
        cin >> entityType >> x >> y >> radius; cin.ignore();
        fieldEntities.emplace_back( entityType, x, y, radius );
    }
    
    vector< Item > items;
    int itemCount; // useful from wood2
    cin >> itemCount; cin.ignore();
    for (int i = 0; i < itemCount; i++) {
        string itemName; // contains keywords such as BRONZE, SILVER and BLADE, BOOTS connected by "_" to help you sort easier
        int itemCost; // BRONZE items have lowest cost, the most expensive items are LEGENDARY
        int damage; // keyword BLADE is present if the most important item stat is damage
        int health;
        int maxHealth;
        int mana;
        int maxMana;
        int moveSpeed; // keyword BOOTS is present if the most important item stat is moveSpeed
        int manaRegeneration;
        int isPotion; // 0 if it's not instantly consumed
        cin >> itemName >> itemCost >> damage >> health >> maxHealth >> mana >> maxMana >> moveSpeed >> manaRegeneration >> isPotion; cin.ignore();
        items.emplace_back( itemName, itemCost, damage, health, maxHealth, mana, maxMana, moveSpeed, manaRegeneration, isPotion );
    }
    sort( begin( items ), end( items ),
        [&]( Item const& i1, Item const& i2 ) {
            if( i1._isPotion )
                return false;
            if( i2._isPotion )
                return true;
            return (double)i1._dmg / (double)i1._cost > (double)i2._dmg / (double)i2._cost;    
        } );
    for( auto& i : items )
        cerr << i._name << ":" << i._dmg << ":" << i._cost << ":" << (double)i._dmg / (double)i._cost << endl;
        
    return make_tuple( myTeam, fieldEntities, items );
}

auto readTurn() {
    int gold;
    cin >> gold; cin.ignore();
    int enemyGold;
    cin >> enemyGold; cin.ignore();
    int roundType; // a positive value will show the number of heroes that await a command
    cin >> roundType; cin.ignore();
    Turn turn( gold, enemyGold, roundType );
    
    vector< Entity > entities;
    int entityCount;
    cin >> entityCount; cin.ignore();
    for (int i = 0; i < entityCount; i++) {
        int unitId;
        int team;
        string unitType; // UNIT, HERO, TOWER, can also be GROOT from wood1
        int x;
        int y;
        int attackRange;
        int health;
        int maxHealth;
        int shield; // useful in bronze
        int attackDamage;
        int movementSpeed;
        int stunDuration; // useful in bronze
        int goldValue;
        int countDown1; // all countDown and mana variables are useful starting in bronze
        int countDown2;
        int countDown3;
        int mana;
        int maxMana;
        int manaRegeneration;
        string heroType; // DEADPOOL, VALKYRIE, DOCTOR_STRANGE, HULK, IRONMAN
        int isVisible; // 0 if it isn't
        int itemsOwned; // useful from wood1
        cin >> unitId >> team >> unitType >> x >> y >> attackRange >> health >> maxHealth >> shield >> attackDamage >> movementSpeed >> stunDuration >> goldValue >> countDown1 >> countDown2 >> countDown3 >> mana >> maxMana >> manaRegeneration >> heroType >> isVisible >> itemsOwned; cin.ignore();
        entities.emplace_back( unitId, team, unitType, x, y, attackRange, health, maxHealth, shield, attackDamage, movementSpeed, stunDuration, goldValue, countDown1, countDown2, countDown3, mana, maxMana, manaRegeneration, heroType, isVisible, itemsOwned );
    }
    
    return make_tuple( turn, entities );
}

bool inf( int team, int x1, int x2 ) {
    if( team == 0 )
        return x1 < x2;
    else
        return x2 < x1;
}

bool sup( int team, int x1, int x2 ) {
    if( team == 0 )
        return x1 > x2;
    else
        return x2 > x1;
}

int dist( int team, int x1, int x2 ) {
    if( team == 0 )
        return x2 - x1;
    else
        return abs( x1 - x2 );
}

int sub( int team, int x1, int val ) {
    if( team == 0 )
        return x1 - val;
    else
        return x1 + val;
}

namespace action {
    void select_hero( string hero ) { cout << hero << endl; }    
    void wait() { cout << "WAIT" << endl; }
    void move( int x, int y ) { cout << "MOVE " << x << " " << y << endl; }
    void attack( int unitId ) { cout << "ATTACK " << unitId << endl; }
    void attack_nearest( string unitType ) { cout << "ATTACK_NEAREST " << unitType << endl; }
    void move_attack( int x, int y, int unitId ) { cout << "MOVE_ATTACK " << x << " " << y << " " << unitId << endl; }
    void buy( string itemName ) { cout << "BUY " << itemName.c_str() << endl; }
    void sell( string itemName ) { cout << "SELL " << itemName.c_str() << endl; }
    
    namespace dr {
        void aoeheal( int x, int y ) { cout << "AOEHEAL " << x << " " << y << endl; }
        void shield( int unitId ) { cout << "SHIELD " << unitId << endl; }
    }
    namespace im {
        void fireball( int x, int y ) { cout << "FIREBALL " << x << " " << y << endl; }
        void burning( int x, int y ) { cout << "BURNING " << x << " " << y << endl; }
    }
}

auto findHeroes( int team, vector< Entity > const& entities ) {
    vector< Entity const* > heroes;
    
    for( auto& entity : entities )
        if( entity._team == team && entity._heroType == "IRONMAN" )
            heroes.push_back( &entity );
            
    for( auto& entity : entities )
        if( entity._team == team && entity._heroType == "DOCTOR_STRANGE" )
            heroes.push_back( &entity );
                            
    return heroes;
}

auto findFurthestAlly( int team, vector< Entity > const& entities ) {
    Entity const* furthestAlly = nullptr;
    for( auto const& entity : entities ) {
        if( entity._team == team ) {
            if( !furthestAlly )
                furthestAlly = &entity;
            else
                furthestAlly = sup( team, entity._x, furthestAlly->_x ) ? &entity : furthestAlly;
        }            
    }
    return *furthestAlly;
}

auto findClosestEnemy( int team, vector< Entity > const& entities ) {
    Entity const* closestEnemy = nullptr;
    for( auto const& entity : entities ) {
        if( entity._team != team && entity._unitType != "GROOT" ) {
            if( !closestEnemy )
                closestEnemy = &entity;
            else
                closestEnemy = inf( team, entity._x, closestEnemy->_x ) ? &entity : closestEnemy;
        }            
    }
    return *closestEnemy;
}

auto findEnemyTower( int team, vector< Entity > const& entities ) {
    auto enemy = find_if( begin( entities ), end( entities ),
                            [&]( Entity const& entity ) {
                                return entity._team != team
                                && entity._unitType == "TOWER";
                            } );
    return *enemy;
}

auto findRangestEnemy( int team, vector< Entity > const& entities ) {
    Entity const* enemy = nullptr;
    for( auto const& entity : entities ) {
        if( entity._team != team && entity._unitType != "GROOT" ) {
            if( !enemy )
                enemy = &entity;
            else
                enemy = sup( team,
                            sub( team, entity._attackRange, entity._x ),
                            sub( team, enemy->_attackRange, enemy->_x ) )
                        ? &entity : enemy;
        }            
    }
    return *enemy;
}

auto findNextEnemy( int team, vector< Entity > const& entities ) {
    vector< Entity >::const_iterator nextEnemy;
    for( auto const& unitType : { "UNIT", "HERO", "TOWER" } ) {
        cerr << unitType << endl;
        nextEnemy = find_if( begin( entities ), end( entities ),
                            [&]( Entity const& entity ) {
                                return entity._team != team
                                && entity._unitType == unitType;
                            } );
        if( nextEnemy != end( entities ) )
            break;
    }
    return *nextEnemy;
}

auto findLowestNonNeutralEntity( int team, vector< Entity > const& entities ) {
    Entity const* lowestEntity = nullptr;
    for( auto const& entity : entities ) {
        if( entity._unitType == "GROOT" )
            continue;
        if( !lowestEntity )
            lowestEntity = &entity;
        else
            lowestEntity = entity._health < lowestEntity->_health ? &entity : lowestEntity;
    }
    return *lowestEntity;
}

auto castOrWait( int team, vector< Entity > const& entities, Entity const& hero ) {
    if( hero._heroType == "IRONMAN" ) {
        action::wait();
    }
    else {
        action::wait();
    }
}

void lastHit( int team, vector< Entity > const& entities, Entity const* hero ) {    
    auto lowestEntity = findLowestNonNeutralEntity( team, entities );
    cerr << "lowest health: " << lowestEntity._health << endl;
    if( hero ) {
        if( lowestEntity._health <= hero->_attackDamage && lowestEntity._health < lowestEntity._maxHealth * 40 / 100 ) {
            action::attack( lowestEntity._unitId );
            lowestEntity._health -= hero->_attackDamage;
        }
        else
            castOrWait( team, entities, *hero );
    }
}

int main()
{   
    int team;
    vector< FieldEntity > fieldEntities;
    vector< Item > items;
    tie( team, fieldEntities, items ) = readInit();
    bool ironman = true;
    
    // game loop
    while (1) {
        Turn turn;
        vector< Entity > entities;
        tie( turn, entities ) = readTurn();

        if( turn._roundType < 0 ) {
            if( ironman ) { 
                action::select_hero("IRONMAN");
                ironman = false;
            }
            else
                action::select_hero("DOCTOR_STRANGE");
        }
        else {
            auto heroes = findHeroes( team, entities );
            
            auto furthestAlly = findFurthestAlly( team, entities );            
            auto nextEnemy = findNextEnemy( team, entities );
            auto closestEnemy = findClosestEnemy( team, entities );
            auto rangestEnemy = findRangestEnemy( team, entities );
            auto enemyTower = findEnemyTower( team, entities );
            auto allyTower = findEnemyTower( !team, entities );
            auto allyDistance = dist( team, heroes[0]->_x, furthestAlly._x );
            
            cerr << "gold: " << turn._gold << endl;
            cerr << " furthestAlly->_x: " <<  furthestAlly._x << endl;
            cerr << " closestEnemy._x: " <<  closestEnemy._x << endl;
            cerr << " heroes[0]._x: " <<  heroes[0]->_x << endl;
            cerr << "dist closestEnemy: " << dist( team, heroes[0]->_x, closestEnemy._x ) << endl;
            cerr << "dist rangestEnemy: " << dist( team, heroes[0]->_x, rangestEnemy._attackRange + rangestEnemy._x ) << endl;
            cerr << "dist furthestAlly: " << dist( team, heroes[0]->_x, furthestAlly._x ) << endl;
            
            for( auto* hero : heroes ) {
                if( dist( team, hero->_x, closestEnemy._x ) < hero->_attackRange
                    || dist( team, hero->_x, rangestEnemy._attackRange + rangestEnemy._x ) <= 30 ) {                        
                        action::move( sub( team, hero->_x, 300 ), hero->_y );
                }            
    
                else if( dist( team, hero->_x, furthestAlly._x ) > 200 ) {
                    if( furthestAlly._unitId != allyTower._unitId )
                        action::move( sub( team, furthestAlly._x, 200 ), furthestAlly._y );
                        
                    else if( furthestAlly._unitId == allyTower._unitId && dist( team, hero->_x, allyTower._x ) <= 100 )
                        lastHit( team, entities, hero );
                    
                    else
                        action::move( allyTower._x, allyTower._y );  // to check                                  
                }
                    
                else {
                    bool bought = false;
                    if( hero->_itemsOwned < 4 ) {
                        auto item = find_if( begin( items ), end( items ),
                            [&]( Item const& item ) {
                                return item._cost <= turn._gold && item._dmg > 0;
                            } );
                        if( item != end( items ) ) {
                            action::buy( item->_name );
                            items.erase( item );
                            turn._gold -= item->_cost;
                            bought = true;
                        }
                    }
    
                    if( !bought )
                        lastHit( team, entities, hero );
                }
            }
        }
    }
}
