
#include	"IAntGUI.hpp"
#include	"IAntLogic.hpp"
#include    <vector>
#include    <map>
#include    <set>
#include    <iostream>
#include    <cstring>
//#include    <QDebug>

#if !defined ANT_MANAGER_HPP
#define ANT_MANAGER_HPP


typedef std::pair<int, int> Position;
typedef std::pair<Position, int> PosToInt;
typedef std::pair<int, Position> IntToPos;

namespace antlogic
{
	enum TeamsIDs{Team01 = 0, Team02, Team03, Team04};

	enum Sensor_Boundary{DEL_BOUND = 2, SENSOR_ARRAY_SIZE, MAX_INTENS = 100};

    class MyAnt: public Ant
    {
        Position ant_pos;
        bool food;
        int f_remain; // number of steps that remains until ant will be unfrozen
        int team_id;
        char* memory;
    public:
        MyAnt(Position ant_pos, int team_id); 
        MyAnt(const MyAnt &); 
        virtual char* getMemory() const;
        virtual bool hasFood() const;
        virtual int getTeamId() const;
        void move_to(Position new_position);
        Position getPos();
        int get_f_remain();
        void froze();
        void froze_step();
        void get_food();
        void put_food();
        ~MyAnt();
    };
}

namespace antgui
{
    class MyAnt: public Ant
    {
        Point ant_pos;
        bool food;
        bool frozen; // number of steps that remains until ant will be unfrozen
        int team_id;
    public:
        MyAnt(antlogic::MyAnt & ant);
        virtual bool hasFood() const;
        virtual bool isFrozen() const;
        virtual int getTeamId() const;
        virtual Point getPoint() const;
    };

    class MyFood: public Food
    {
        int count;
        Point food_pos;
    public:
        MyFood(int count, Point pos);
        virtual Point getPoint() const;
        virtual int getCount() const;
    };
}

class AntManager
{
    struct Cell // class that contain information about everyone cell
    {
        static int count;
        std::set<Position> ants; //set of ants positions in all_ants vector 
        int food_count;
        size_t smell;
        size_t smell_intens; 
        Cell();
    };

//    antlogic::AntSensor** getSensor();
    void update_gui();

    bool isWall(Position pos);

    std::vector<std::vector<antlogic::MyAnt> > all_ants;
    std::vector<std::vector<Cell> > game_map;
    std::vector<antlogic::IAntLogic* > strategy;
    std::map<int, Position > teamid_to_anthill;
    std::map<Position, int > anthill_to_teamid;
    antgui::IAntGui * gui;
    int height, width;
    int teamCount, MaxAntCountPerTeam;

    public:
        void step(int nstep);
        void setGui(antgui::IAntGui* gui);
        void setFoodGeneretor(antgui::food_iterator *it);
        AntManager(int height, int width, int teamCount, int MaxAntCountPerTeam = 40);
};

#endif
