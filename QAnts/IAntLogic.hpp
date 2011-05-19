#ifndef _IANTLOGIC_H_

#define _IANTLOGIC_H_

#include <string>
#include <stack>

namespace antlogic
{

    const int MAX_MEMORY = 32; // ������ ������ ������ �������

    // ��������� ������ ������ �������.
    // ����� ������ ���������� ���������� �� ����� ������ �������� ����
    struct AntSensor {
        AntSensor();
        int smell; // ������� ������
        int smellIntensity; // ������������� ������
        bool isFriend; // ������� "������" �������
        bool isEnemy; // ������� "������" �������
        bool isMyHill; // ������� "������" �����������
        bool isEnemyHill; // ������� "������" �����������
        bool isFood; // ������� ���
        bool isWall; // ������� �����
    };

    // ��� ��������� �������� �������
    enum AntActionType{
        // ������������:
        MOVE_UP = 0,
        MOVE_LEFT = 1,
        MOVE_DOWN = 2,
        MOVE_RIGHT = 3,
        // �������:
        BITE_UP = 4,
        BITE_LEFT = 5,
        BITE_DOWN = 6,
        BITE_RIGHT = 7,
        // ����������� � ����:
        GET = 8,
        PUT = 9,
    };

    // �������� ������� �� ������� ����  
    struct AntAction
    {
        AntActionType actionType; // ������������, ������� ��� ����������� � ����
        bool putSmell; // �������� �� ����� �� ������
        int smell; // ������� ������������ �������� ������
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