#include "IAntLogic.hpp"
#include <cstdlib>
#include <cstring>

#ifndef _IANT_LOGIC_IMPL_
#define _IANT_LOGIC_IMPL_

    #define id ant.getMemory()[0]
    #define x ant.getMemory()[1]
    #define y ant.getMemory()[2]
    #define job ant.getMemory()[3]
    #define lastkick ant.getMemory()[4]
    #define dist ant.getMemory()[5]
    #define dat sensors, ant, res
    #define datdef AntSensor sensors[3][3], const antlogic::Ant &ant, AntAction &res
    #define MYSMELL 362342


namespace antlogic
{
	enum Teams{Team01 = 0, Team02, Team03, Team04};

	class AntLogicTeam01 : public IAntLogic
	{
	public:
        AntAction GetAction(const Ant&, AntSensor[][3]);
	};

	class AntLogicTeam02 : public IAntLogic
	{
        int l, r, u, d;
        int cr;
        void go( datdef, int dir );
        void kick( datdef, int dir );
        AntSensor gss( datdef, int dir );
        AntSensor mgss(datdef, int gdir, int vdir);
        long getEnemyLoc( datdef );
        bool nearHill( datdef );
        int addx(int xx, int dir);
        int addy(int xx, int dir);
        int getpri(datdef, int dir);
        void gogo( datdef );
        void farmers( datdef );
        AntAction GetAntAction(AntSensor sensors[3][3], bool hasFood, char memory[MAX_MEMORY]);
    
    public:
        AntLogicTeam02() : cr(100) {};
		AntAction GetAction(const Ant&, AntSensor[][3]);
	};


	class AntLogicTeam03 : public IAntLogic
	{
	public:
        AntAction GetAction(const Ant&, AntSensor[][3]);
	};

}

#endif
