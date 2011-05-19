#include "IAntLogicImpl.hpp"

namespace antlogic
{
    AntAction AntLogicTeam01::GetAction(const antlogic::Ant &ant, AntSensor sensors[3][3])
	{
        char *memory = ant.getMemory();
	    antlogic::AntAction action;

        if (ant.hasFood() && sensors[1][1].isMyHill)
	    {
		    action.actionType = antlogic::PUT;
	    }
        else if (!ant.hasFood() && !sensors[1][1].isMyHill && sensors[1][1].isFood) 
	    {
		    action.actionType = antlogic::GET;
	    }
	    else
	    {
            char spiralSize = std::max<char>(10, memory[0]);
		    char position = memory[1];
		    char direction = memory[2];
		    position = position % spiralSize;
		    if (position == 0)
		    {
			    if (direction == 3) spiralSize = std::max<char>(10, (spiralSize + 1) % 41);
			    direction = (direction + 1) % 4;
		    }
		    position++;
		    memory[0] = spiralSize;
		    memory[1] = position;
		    memory[2] = direction;
		    action.actionType = antlogic::AntActionType(direction);
	    }

	    return action;
	}

	AntAction AntLogicTeam02::GetAction(const antlogic::Ant &ant, AntSensor sensors[3][3])
	{
	    AntAction res;
	    long k;

	    memset( &res, 0, sizeof( res ) );

        if( id == 0 )
	    {
		    lastkick = -1;
		    srand( 6754773 );

		    id = sensors[1][1].smell + 1;
		    res.putSmell = true;
		    res.smell = id;

		    if( id == 1 )
		    {
			    l = 1; r = 3; u = 0; d = 2;
			    if( sensors[2][1].isWall ) { l = 3; r = 1; }
			    if( sensors[1][2].isWall ) { u = 2; d = 0; }
		    }

		    x = 0;
		    y = 0;
		    job = 0; if( id == 11 || id == 12 ) job = 1;
		    if( id >= 13 && id <= 15 ) job = 2;
		    //if( id >= 16 && id <= 18 ) job = 4;

		    if( job == 2 )
		    {
			    if( id == 13 )
			    {
				    go( dat, d );
				    return res;
			    }
			    if( id == 14 )
			    {
				    go( dat, r );
				    return res;
			    }
		    }
	    }

	    if( job == 1 )
	    {
		    if( id == 12 && x == 0 && y == 0 )
			    go( dat, r );
		    else if( id == 12 && x == 1 && y == 0 )
			    go( dat, d );

		    else
		    {
			    k = getEnemyLoc( dat );

			    if( k != -1 )
				    kick( dat, k );
			    else
				    kick( dat, d );
		    }
		    return res;
	    }

	    int el = getEnemyLoc(dat);
	    if (el != -1 && el != lastkick)
	    {
		    kick(dat, el);
		    lastkick = el;
		    return res;
	    }
	    lastkick = -1;

	    if( job == 2 )
	    {
		    if( id == 13 )
		    {
			    if( nearHill( dat ) )
			    {
				    k = getEnemyLoc( dat );
				    if( k != -1 )
					    kick( dat, k );
				    else
					    kick( dat, d );
			    }
			    else if( gss( dat, r ).isWall )
				    job = 0;
			    else go( dat, r );
		    }
		    if( id == 14 )
		    {
			    if( nearHill( dat ) )
			    {
				    k = getEnemyLoc( dat );
				    if( k != -1 )
					    kick( dat, k );
				    else
					    kick( dat, r );
			    }
			    else if( gss( dat, d ).isWall )
				    job = 0;
			    else go( dat, d );
		    }
		    if( id == 15 )
		    {
			    if( nearHill( dat ) )
			    {
				    k = getEnemyLoc( dat );
				    if( k != -1 )
					    kick( dat, k );
				    else
					    kick( dat, l );
			    }
			    else if( gss( dat, r ).isWall && gss( dat, d ).isWall )
				    job = 0;
			    else if( x > y )
				    go( dat, d );
			    else
				    go( dat, r );
		    }
	    }
	    else if( job == 4 )
	    {
		    if( id == 16 )
		    {
			    if( gss( dat, r ).isWall )
			    {
				    if( gss( dat, cr ).isEnemyHill )
				    {
					    k = getEnemyLoc( dat );
					    if( k != -1 )
						    kick( dat, k );
					    else if( gss( dat, cr ).isFood )
					    {
						    res.actionType = GET;
						    job = 3;
					    }
					    else
						    kick( dat, d );
				    }
				    else
					    job = 0;
			    }
			    else go( dat, r );
		    }
		    if( id == 17 )
		    {
			    if( gss( dat, d ).isWall )
			    {
				    if( gss( dat, cr ).isEnemyHill )
				    {
					    k = getEnemyLoc( dat );
					    if( k != -1 )
						    kick( dat, k );
					    else if( gss( dat, cr ).isFood )
					    {
						    res.actionType = GET;
						    job = 3;
					    }
					    else
						    kick( dat, r );
				    }
				    else
					    job = 0;
			    }
			    else go( dat, d );
		    }
		    if( id == 18 )
		    {
			    if( gss( dat, r ).isWall && gss( dat, d ).isWall )
			    {
				    if( gss( dat, cr ).isEnemyHill )
				    {
					    k = getEnemyLoc( dat );
					    if( k != -1 )
						    kick( dat, k );
					    else if( gss( dat, cr ).isFood )
					    {
						    res.actionType = GET;
						    job = 3;
					    }
					    else
						    kick( dat, l );
				    }
				    else
					    job = 0;
			    }
			    else if( x > y )
				    go( dat, d );
			    else
				    go( dat, r );
		    }
	    }
	    else if( job == 3 )
	    {
		    if( id == 16 || id == 18 )
		    {
			    if( gss( dat, r ).isWall && ant.hasFood() )
				    go( dat, l );
			    else if( ant.hasFood() )
				    res.actionType = PUT;
			    else
			    {
				    go( dat, r );
				    job = 4;
			    }
		    }
		    if( id == 17 )
		    {
			    if( gss( dat, d ).isWall && ant.hasFood() )
				    go( dat, u );
			    else if( ant.hasFood() )
				    res.actionType = PUT;
			    else
			    {
				    go( dat, d );
				    job = 4;
			    }
		    }
	    }
	    else {
		    farmers(dat);
	    }

	    return res;
    }

    void AntLogicTeam02::go( datdef, int dir )
    {
	    res.actionType = ( AntActionType )dir;
	    if( dir == r ) ++ x;
	    if( dir == l ) -- x;
	    if( dir == u ) -- y;
	    if( dir == d ) ++ y;
    }

    void AntLogicTeam02::kick( datdef, int dir )
    {
	    res.actionType = ( AntActionType )( 4 + dir );
    }

    AntSensor AntLogicTeam02::gss( datdef, int dir )
    {
	    if( dir == 0 )
		    return sensors[1][0];
	    if( dir == 1 )
		    return sensors[0][1];
	    if( dir == 2 )
		    return sensors[1][2];
	    if( dir == 3 )
		    return sensors[2][1];
	    if( dir == 100 )
		    return sensors[1][1];

	    return sensors[1][1];
    }

    AntSensor AntLogicTeam02::mgss(datdef, int gdir, int vdir)
    {
	    if (gdir == 1 && vdir == 0) return sensors[0][0];
	    if (gdir == 3 && vdir == 0) return sensors[2][0];
	    if (gdir == 1 && vdir == 2) return sensors[0][2];
	    if (gdir == 3 && vdir == 2) return sensors[2][2];
	    return sensors[1][1];
    }

    long AntLogicTeam02::getEnemyLoc( datdef )
    {
	    long i;
	    int vals[5];
	    long vn = 0;

	    for( i =0; i <= 3; ++ i )
		    if( gss( dat, i ).isEnemy )
			    vals[vn ++] = i;

	    if( !vn )
		    return -1;
	    else
		    return vals[rand( ) % vn];
    }

    bool AntLogicTeam02::nearHill( datdef )
    {
	    if( sensors[0][0].isEnemyHill )
		    return 1;
	    if( sensors[2][0].isEnemyHill )
		    return 1;
	    if( sensors[0][2].isEnemyHill )
		    return 1;
	    if( sensors[2][2].isEnemyHill )
		    return 1;

	    return 0;
    }

    int AntLogicTeam02::addx(int xx, int dir)
    {
	    if (dir == l) return xx - 1;
	    if (dir == r) return xx + 1;
	    return xx;
    }

    int AntLogicTeam02::addy(int xx, int dir)
    {
	    if (dir == u) return xx - 1;
	    if (dir == d) return xx + 1;
	    return xx;
    }

    int AntLogicTeam02::getpri(datdef, int dir)
    {
	    if (gss(dat, dir).isMyHill) return -9999;
	    if (gss(dat, dir).isWall) return -10000;
	    int pp = 0;
	    if (gss(dat, dir).isFood) pp = 10000;
	    if (dir == l || dir == r) {
		    if (mgss(dat, dir, u).isFood) pp += 1000;
		    if (mgss(dat, dir, d).isFood) pp += 1000;
		    if (mgss(dat, dir, u).isEnemy) pp -= 3000;
		    if (mgss(dat, dir, d).isEnemy) pp -= 3000;
	    }
	    if (dir == u || dir == d) {
		    if (mgss(dat, l, dir).isFood) pp += 1000;
		    if (mgss(dat, r, dir).isFood) pp += 1000;
		    if (mgss(dat, l, dir).isEnemy) pp -= 3000;
		    if (mgss(dat, r, dir).isEnemy) pp -= 3000;
	    }
	    if (dir == r || dir == d) pp += 50;
	    int ss;
	    if (abs(gss(dat, dir).smell - MYSMELL) > 5000)/* + addx(x, dir) * 100 + addy(y, dir)*/
		    ss = 500;
	    else ss = (gss(dat, dir).smell - MYSMELL) * 10 + gss(dat, dir).smellIntensity;
	    pp -= ss;
	    return pp;
    }

    void AntLogicTeam02::gogo( datdef )
    {
		    int i, mi, mx;
		    int p[4];
		    mx = -100000;
		    for (i = 0; i < 4; ++i) {
			    int t = p[i] = getpri(dat, i);
			    if (t > mx) {
				    mx = t;
			    }
		    }
		    for (;;)
		    {
			    int ii = rand() % 4;
			    if (p[ii] == mx) {
				    mi = ii;
				    break;
			    }
		    }
		    go(dat, mi);
    }

    void AntLogicTeam02::farmers( datdef )
    {
        if (ant.hasFood())
	    {
		    if (gss(dat, cr).isMyHill) res.actionType = PUT;
		    else {
			    res.putSmell = true;
			    res.smell = MYSMELL + dist;
			    if (gss(dat, l).isWall) go(dat, u);
			    else if (gss(dat, u).isWall) go(dat, l);
			    else {
				    long s = x + 1 + y;
                    if (s == 0) 
                    {
                        s = 1;
                    }
				    if (rand() % s < x) go(dat, l);
				    else go(dat, u);
			    }
			    ++dist;
		    }
	    }
	    else {
		    if (gss(dat, cr).isMyHill) gogo(dat);
		    else
		    {
			    res.putSmell = true;
			    res.smell = gss(dat, cr).smell;
			    if (!res.smell) res.smell = MYSMELL;
			    res.smell += 10;

			    if (gss(dat, cr).isFood) {
				    res.actionType = GET;
				    dist = 0;
			    }
			    else gogo(dat);
		    }
	    }
    }

    AntAction AntLogicTeam03::GetAction(const antlogic::Ant &ant, antlogic::AntSensor sensors[3][3])
    {
	    AntAction result;
    	
        char *memory = ant.getMemory();
	    //seed(time(NULL));
    	
	    //	Нумерация муравьев и определение их количества
	    if(!memory[0])
	    {
            srand(rand());

		    memory[0] = sensors[1][1].smell + 1;
		    result.smell = memory[0];
		    result.putSmell = true;
		    result.actionType = PUT;

		    return result;
	    }
	    else
		    if(!memory[1])
		    {
			    memory[1] = sensors[1][1].smell;
			    //memory[31] = 1;
		    }

	    //	Выбор стратегии
	    if(memory[1])
	    {	
		    // 1 тур
		    // Координаты дома
		    if(!memory[2] && !memory[3])
		    {
			    if(memory[1] == 10)
			    {
				    if(sensors[1][0].isWall && sensors[0][1].isWall)
				    {
					    memory[2] = 1;
					    memory[3] = 1;
				    }
				    if(sensors[1][2].isWall && sensors[0][1].isWall)
				    {
					    memory[2] = 1;
					    memory[3] = 21;
				    }
				    if(sensors[1][0].isWall && sensors[2][1].isWall)
				    {
					    memory[2] = 21;
					    memory[3] = 1;
				    }
				    if(sensors[1][2].isWall && sensors[2][1].isWall)
				    {
					    memory[2] = 21;
					    memory[3] = 21;
				    }
				    memory[4] = memory[2];
				    memory[5] = memory[3];
    			
				    memory[6] = 1 + (20 * rand() / RAND_MAX);
				    memory[7] = 1 + (20 * rand() / RAND_MAX);
			    } // if Координаты дома
    //			if(memory[1] == 20)
    //			{
			  			    else
			    {
				    if(sensors[1][0].isWall && sensors[0][1].isWall)
				    {
					    memory[2] = 1;
					    memory[3] = 1;
				    }
				    if(sensors[1][2].isWall && sensors[0][1].isWall)
				    {
					    memory[2] = 1;
					    memory[3] = 41;
				    }
				    if(sensors[1][0].isWall && sensors[2][1].isWall)
				    {
					    memory[2] = 41;
					    memory[3] = 1;
				    }
				    if(sensors[1][2].isWall && sensors[2][1].isWall)
				    {
					    memory[2] = 41;
					    memory[3] = 41;
				    }
				    memory[4] = memory[2];
				    memory[5] = memory[3];
    			
				    memory[6] = 1 + (40 * rand() / RAND_MAX);
				    memory[7] = 1 + (40 * rand() / RAND_MAX);
			    }
		    }

		    // Достигли цели
		    if((memory[6] == memory[4]) && 
			    (memory[7] == memory[5]))
		    {
                if(ant.hasFood() && sensors[1][1].isMyHill)
			    {
				    result.actionType = PUT;
				    if(memory[1] == 10)
				    {
					    memory[6] = 1 + (20 * rand() / RAND_MAX);
					    memory[7] = 1 + (20 * rand() / RAND_MAX);
				    }
				    else
				    {
					    memory[6] = 1 + (40 * rand() / RAND_MAX);
					    memory[7] = 1 + (40 * rand() / RAND_MAX);
				    }

				    return result;
			    }
			    else
			    {
				    if(memory[1] == 10)
				    {
					    memory[6] = 1 + (20 * rand() / RAND_MAX);
					    memory[7] = 1 + (20 * rand() / RAND_MAX);
				    }
				    else
				    {
					    memory[6] = 1 + (40 * rand() / RAND_MAX);
					    memory[7] = 1 + (40 * rand() / RAND_MAX);
				    }
			    }
		    }
		    // Нашли еду по пути
            if(!ant.hasFood() && !sensors[1][1].isMyHill && sensors[1][1].isFood)
		    {
			    result.actionType = GET;
			    memory[6] = memory[2];
			    memory[7] = memory[3];
		    }
		    // Движемся к цели
		    else
		    {
			    if(sensors[0][1].isEnemy)
			    {
				    result.actionType = BITE_LEFT;
				    return result;
			    }
			    if(sensors[2][1].isEnemy)
			    {
				    result.actionType = BITE_RIGHT;
				    return result;
			    }
			    if(sensors[1][0].isEnemy)
			    {
				    result.actionType = BITE_UP;
				    return result;
			    }
			    if(sensors[1][2].isEnemy)
			    {
				    result.actionType = BITE_DOWN;
				    return result;
			    }
    			
			    if(memory[6] > memory[4])
			    {
				    result.actionType = MOVE_RIGHT;
				    memory[4]++;
			    }else
			    if(memory[6] < memory[4])
			    {
				    result.actionType = MOVE_LEFT;
				    memory[4]--;
			    }else
			    if(memory[7] > memory[5])
			    {
				    result.actionType = MOVE_DOWN;
				    memory[5]++;
			    }else
			    if(memory[7] < memory[5])
			    {
				    result.actionType = MOVE_UP;
				    memory[5]--;
			    }
		    }
	    }

	    return result;
    }
}
