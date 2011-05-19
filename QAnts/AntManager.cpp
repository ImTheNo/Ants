
#include	"AntManager.hpp"
#define MAX_FROZEN 8
#define TOO_FROZEN (MAX_FROZEN + 1)
#define MAX_RGB 200

namespace antlogic
{
    MyAnt::MyAnt(Position ant_pos, int team_id): ant_pos(ant_pos), food(false), team_id(team_id), f_remain(0) 
    {
        memory = new char[antlogic::MAX_MEMORY];
        memset(memory, 0, antlogic::MAX_MEMORY);
    }

    MyAnt::MyAnt(const MyAnt & new_ant) 
    {
        ant_pos = new_ant.ant_pos;
        food = new_ant.food;
        f_remain = new_ant.f_remain;
        team_id = new_ant.team_id;
        memory = new char[antlogic::MAX_MEMORY];
        memcpy(memory, new_ant.memory, MAX_MEMORY); 
    }
    
    char* MyAnt::getMemory() const
    {
        return memory;
    }
    
    bool MyAnt::hasFood() const
    {
        return food;
    }
    
    int MyAnt::getTeamId() const
    {
        return team_id;
    }
    
    void MyAnt::move_to(Position new_position)
    {
        ant_pos.first = new_position.first;
        ant_pos.second = new_position.second;
    }

    Position MyAnt::getPos()
    {
        return ant_pos;
    }

    int MyAnt::get_f_remain()
    {
        return f_remain;
    }
    
    void MyAnt::froze()
    {
        f_remain = MAX_FROZEN;
    }

    void MyAnt::froze_step()
    {
        if (f_remain > 0) 
        {
            f_remain--;
        }
    }
    
    void MyAnt::get_food()
    {
        food = true;
    }
    
    void MyAnt::put_food()
    {
        food = false;
    }
    
    MyAnt::~MyAnt()
    {
        delete memory;
    }
}

namespace antgui
{
    MyAnt::MyAnt(antlogic::MyAnt & ant)
	{
        Position pos = ant.getPos();
        ant_pos.x = pos.first;
        ant_pos.y = pos.second;
        food = ant.hasFood();
        frozen = ant.get_f_remain() > 0;
        team_id = ant.getTeamId();
	}

    bool MyAnt::hasFood() const
	{
        return food;
	}

    bool MyAnt::isFrozen() const
	{
        return frozen;
	}

    int MyAnt::getTeamId() const
	{
        return team_id;
	}

    Point MyAnt::getPoint() const
	{
        return ant_pos;
	}

    MyFood::MyFood(int count, Point pos): count(count), food_pos(pos)
	{

	}

    Point MyFood::getPoint() const
	{
        return food_pos;
	}

    int MyFood::getCount() const
	{
        return count;
	}
}
AntManager::AntManager(int height, int width, int teamCount, int MaxAntCountPerTeam): height(height), teamCount(teamCount), width(width), MaxAntCountPerTeam(MaxAntCountPerTeam), all_ants(teamCount), game_map(width, std::vector<Cell>(height)) 
{
    teamid_to_anthill.insert(IntToPos(antlogic::Team01, Position(0, 0)));
    teamid_to_anthill.insert(IntToPos(antlogic::Team02, Position(width - 1, 0)));
    teamid_to_anthill.insert(IntToPos(antlogic::Team03, Position(0, height - 1)));
    teamid_to_anthill.insert(IntToPos(antlogic::Team04, Position(width - 1, height - 1)));

    anthill_to_teamid.insert(PosToInt(Position(0, 0), antlogic::Team01));
    anthill_to_teamid.insert(PosToInt(Position(width - 1, 0), antlogic::Team02));
    anthill_to_teamid.insert(PosToInt(Position(0, height - 1), antlogic::Team03));
    anthill_to_teamid.insert(PosToInt(Position(width - 1, height - 1), antlogic::Team04));


    for (int i = 0; i < teamCount; i++) 
    {
        strategy.push_back(antlogic::IAntLogic::GetAntLogic(i));
    }
}

void AntManager::setGui(antgui::IAntGui* gui)
{
    this->gui = gui;
}

void AntManager::setFoodGeneretor(antgui::food_iterator *it)
{
    antgui::food_iterator eos;
    while (*it != eos) 
    {
        antgui::Point food_point = (**it).getPoint();
        game_map[food_point.x][food_point.y].food_count += (**it).getCount(); 
        (*it)++;
    }
}

void AntManager::step(int step)
{
    int i, j;
    if (all_ants[0].size() < MaxAntCountPerTeam) 
    {
        for (i = 0; i < teamCount; i++) 
        {
            Position start = teamid_to_anthill.find(i)->second;
            all_ants[i].push_back(antlogic::MyAnt(start, i));
            game_map[start.first][start.second].ants.insert(Position(i, all_ants[i].size() - 1));
        }
    }

    int curAntPerTeam = all_ants[0].size();
//                    std::cout <<  "   " << all_ants.size() << " " << curAntPerTeam << std::endl; 
    for (i = 0; i < teamCount; i++) 
    {
        for (j = 0; j < curAntPerTeam; j++) 
        {
            all_ants[i][j].froze_step();
        }
    }
    for (i = 0; i < teamCount; i++) 
    {
        for (j = 0; j < curAntPerTeam; j++) 
        {
            if (all_ants[i][j].get_f_remain() > 0) 
            {
                continue; 
            }
            antlogic::AntSensor sens[antlogic::SENSOR_ARRAY_SIZE][antlogic::SENSOR_ARRAY_SIZE];
            Position ant_pos = all_ants[i][j].getPos();
            for (int w = -1; w < antlogic::DEL_BOUND; w++) //w is counter for height (Ox) axis 
            {
                int x = ant_pos.first + w;
                for (int h = -1; h < antlogic::DEL_BOUND; h++)//h is counter for height (Oy) axis
                {
                    int y = ant_pos.second + h;
                    antlogic::AntSensor & tmp = sens[w + 1][h + 1];
                    if (isWall(Position(x, y))) 
                    {
                        tmp.isWall = true;
                        continue;
                    }
                    if (anthill_to_teamid.find(Position(x, y)) != anthill_to_teamid.end())
                    {
                        if (anthill_to_teamid.find(Position(x, y))->second == i) 
                        {
                            tmp.isMyHill = true;
                        }
                        else
                        {
                            tmp.isEnemyHill = true;
                        }
                    }
                    for (std::set<Position>::iterator it_s = game_map[x][y].ants.begin() ; it_s != game_map[x][y].ants.end(); it_s++) 
                    {
                        if (it_s->first == i && it_s->second == j) 
                        {
                            continue;

                        }
                        if (all_ants[it_s->first][it_s->second].getTeamId() == i) 
                        {
                            tmp.isFriend = true;
                        }
                        else 
                        {
                            tmp.isEnemy = true;
                        }
                    }
                    tmp.isFood = game_map[x][y].food_count > 0;
                    tmp.smell = game_map[x][y].smell;
                    tmp.smellIntensity = game_map[x][y].smell_intens;

//                    std::cout << x << " " << y << " " << tmp.isWall << tmp.isFriend << tmp.isEnemy << tmp.isMyHill << tmp.isEnemyHill << tmp.isFood << tmp.smell << tmp.smellIntensity << std::endl; // for debugging
                }
            }
            antlogic::AntAction action = strategy[i]->GetAction(all_ants[i][j], sens);
            if (action.putSmell) 
            {
                game_map[ant_pos.first][ant_pos.second].smell = action.smell;
                game_map[ant_pos.first][ant_pos.second].smell_intens = antlogic::MAX_INTENS;
            }

            if (action.actionType >= antlogic::MOVE_UP && action.actionType <= antlogic::BITE_RIGHT) 
            {
                Position new_ant_pos = Position(ant_pos.first, ant_pos.second);
                switch (action.actionType) 
                {
                    case antlogic::BITE_UP:	
                    case antlogic::MOVE_UP:	
                        new_ant_pos.second -= 1;
                        break;

                    case antlogic::BITE_LEFT:	
                    case antlogic::MOVE_LEFT:	
                        new_ant_pos.first -= 1;
                        break;

                    case antlogic::BITE_DOWN:	
                    case antlogic::MOVE_DOWN:	
                        new_ant_pos.second += 1;
                        break;

                    case antlogic::BITE_RIGHT:	
                    case antlogic::MOVE_RIGHT:	
                        new_ant_pos.first += 1;
                        break;

                }
                if (isWall(new_ant_pos)) 
                {
                    continue;
                }
                if (action.actionType < antlogic::BITE_UP) 
                {
                    game_map[new_ant_pos.first][new_ant_pos.second].ants.insert(Position(i, j));
                    game_map[ant_pos.first][ant_pos.second].ants.erase(Position(i, j));
                    all_ants[i][j].move_to(new_ant_pos);
                }
                else
                {
                    Cell & bitten_cell = game_map[new_ant_pos.first][new_ant_pos.second];
                    Position bitten_ant;
                    int f_remain_min = TOO_FROZEN;
                    for (std::set<Position>::iterator it_s = bitten_cell.ants.begin() ; it_s != bitten_cell.ants.end(); it_s++) 
                    {
                        antlogic::MyAnt tmp_ant = all_ants[it_s->first][it_s->second];
                        if (tmp_ant.getTeamId() != i && tmp_ant.get_f_remain() < f_remain_min) 
                        {
                            bitten_ant = *it_s;
                            f_remain_min = tmp_ant.get_f_remain();
                            if (f_remain_min == 0) 
                            {
                                break;
                            }
                        }
                    }
                    if (f_remain_min < TOO_FROZEN) 
                    {
                        if (all_ants[bitten_ant.first][bitten_ant.second].hasFood()) 
                        {
                            (bitten_cell.food_count)++; 
                            all_ants[bitten_ant.first][bitten_ant.second].put_food();
                        }
                        all_ants[bitten_ant.first][bitten_ant.second].froze();
                    }
                }
            }
            else if (action.actionType == antlogic::GET && game_map[ant_pos.first][ant_pos.second].food_count > 0 && !all_ants[i][j].hasFood()) 
            {
                all_ants[i][j].get_food();
                (game_map[ant_pos.first][ant_pos.second].food_count)--; 
            }
            else if (action.actionType == antlogic::PUT && all_ants[i][j].hasFood())
            {
                (game_map[ant_pos.first][ant_pos.second].food_count)++; 
                all_ants[i][j].put_food();
            }
        }
    }
    for (i = 0; i < width; i++) 
    {
        for (j = 0; j < height; j++) 
        {
            if (game_map[i][j].smell_intens > 0) 
            {
                game_map[i][j].smell_intens--;
            }
        }

    }
    update_gui();
}

void AntManager::update_gui()
{
    antgui::MyFood *tmp;
    antgui::MyAnt *tmp_ant;
    gui->BeginPaint();
    gui->Clear();
    for (int i = 0; i < width; i++) 
    {
        for (int j = 0; j < height; j++) 
        {
            if (game_map[i][j].food_count > 0) 
            {
                gui->SetFood(tmp = new antgui::MyFood(game_map[i][j].food_count < MAX_RGB ? game_map[i][j].food_count : MAX_RGB, antgui::Point(i, j)));
                delete tmp;
            }
        }
    }
    for (int i = 0; i < teamCount; i++) 
    {
        for (int j = 0; j < all_ants[i].size(); j++) 
        {
            gui->SetAnt(tmp_ant = new antgui::MyAnt(all_ants[i][j]));
            delete tmp_ant;
        }
        Position pos = teamid_to_anthill.find(i)->second;
        gui->SetTeamScore(i, game_map[pos.first][pos.second].food_count);
//        qDebug() << "TEAM " << i + 1 << " : " << game_map[pos.first][pos.second].food_count; // score counter
    }
    gui->EndPaint();
}

AntManager::Cell::Cell(): food_count(0), smell(0), smell_intens(0)
{

}

bool AntManager::isWall(Position pos)
{
    return pos.first < 0 || pos.second < 0 || pos.first >= width || pos.second >= height;
}



