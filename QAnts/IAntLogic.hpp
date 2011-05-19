#ifndef _IANTLOGIC_H_

#define _IANTLOGIC_H_

#include <string>
#include <stack>

namespace antlogic
{

    const int MAX_MEMORY = 32; // Размер памяти вашего муравья

    // Показания органа чувств муравья.
    // Орган чувств возвращает информацию об одной клетке игрового поля
    struct AntSensor {
        AntSensor();
        int smell; // оттенок запаха
        int smellIntensity; // интенсивность запаха
        bool isFriend; // наличие "своего" муравья
        bool isEnemy; // наличие "чужого" муравья
        bool isMyHill; // наличие "своего" муравейника
        bool isEnemyHill; // наличие "своего" муравейника
        bool isFood; // наличие еды
        bool isWall; // наличие стены
    };

    // Все возможные действия муравья
    enum AntActionType{
        // передвижение:
        MOVE_UP = 0,
        MOVE_LEFT = 1,
        MOVE_DOWN = 2,
        MOVE_RIGHT = 3,
        // кусание:
        BITE_UP = 4,
        BITE_LEFT = 5,
        BITE_DOWN = 6,
        BITE_RIGHT = 7,
        // манипуляция с едой:
        GET = 8,
        PUT = 9,
    };

    // Действие муравья на текущем ходу  
    struct AntAction
    {
        AntActionType actionType; // передвижение, кусание или манипуляция с едой
        bool putSmell; // оставить ли запах на клетке
        int smell; // оттенок оставляемого муравьём запаха
    };

    class Ant
    {
    public:
        virtual char* getMemory() const = 0;
        virtual bool hasFood() const = 0;
        virtual int getTeamId() const = 0;
        virtual ~Ant() {}
    };

    class IAntLogic
    {
    public:
        static IAntLogic* GetAntLogic(int teamId);
        virtual AntAction GetAction(const Ant&, AntSensor[3][3]) = 0;
        virtual ~IAntLogic() {}
    };
}
#endif