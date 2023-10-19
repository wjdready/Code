/**
 * 抽象工厂
 *
 * 提供一个创建一系列相关或相互依赖对象的接口，而无需指定它们具体的类。
 *
 */
#include "base.h"
#include <string>

class MazeFactory
{
  public:
    MazeFactory()
    {

    }

    virtual Maze *MakeMaze() const
    {
        return new Maze();
    }

    virtual Wall *MakeWall() const
    {
        return new Wall();
    }

    virtual Room *MakeRoom(int n) const
    {
        return new Room(n);
    }

    virtual Door *MakeDoor(Room *r1, Room *r2) const
    {
        return new Door(r1, r2);
    }
};

class EnchantedRoom : public Room 
{
  public:
    EnchantedRoom(int n, std::string spell)
    {
        
    }

  private:
    int n;
};


/* 需要咒语的门 */
class NeedSpellDoor : public Door
{
  public:
    NeedSpellDoor(Room *r1, Room *r2) {}
};

std::string CastSpell(const char *spell)
{
    return std::string(spell);
}

/* 魔法迷宫工厂  */
class EnchantedMazeFactory : public MazeFactory
{
  public:
    EnchantedMazeFactory()
    {

    }

    virtual Room *MakeRoom(int n) const
    {
        return new EnchantedRoom(n, CastSpell("good"));
    }

    virtual Door *MakeDoor(Room *r1, Room *r2) const
    {
        return new NeedSpellDoor(r1, r2);
    }
};

class MazeGame
{
  public:
    Maze *CreateMaze(MazeFactory &factory)
    {
        Maze *maze = new Maze();

        Room *r1 = factory.MakeRoom(1);
        Room *r2 = factory.MakeRoom(2);
        Door *theDoor = factory.MakeDoor(r1, r2);

        maze->AddRoom(r1);
        maze->AddRoom(r2);

        r1->SetSide(North, factory.MakeWall());
        // ...... 

        return maze;
    }
};

int main(int argc, char const *argv[])
{
    MazeGame mg;
    EnchantedMazeFactory emf;
    mg.CreateMaze(emf);

    return 0;
}
