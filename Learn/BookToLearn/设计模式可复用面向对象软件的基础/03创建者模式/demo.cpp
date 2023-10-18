#include <iostream>

/**
 *
 * 类 MapSite 是所有迷宫组件的公共抽象类。
 * 
 * 为简化例子，MapSite仅定义了一个操作Enter，它的含义决定于你在进入什么。
 * 如果你进入一个房间，那么你的位置会发生改变。
 * 如果你试图进入一扇门，那么这两件事中就有一件会发生：
 *  如果门是开着的，你进入另一个房间。
 *  如果门是关着的，你就会碰壁。
 * 
 */
class MapSite
{
  public:
    virtual void Enter() = 0;
};


/**
 * 
 * Enter 为更加复杂的游戏操作提供了一个简单基础。
 * 
 * 例如，如果你在一个房间中说“向东走”，游戏只能确定直接在东边的是哪一个MapSite并对它调用Enter。
 * 特定子类的Enter操作将计算出你的位置是发生改变，还是你会碰壁。
 * 
 * 在一个真正的游戏中，Enter 可以将移动的游戏者对象作为一个参数。
 * 
 * Room 是 MapSite 的一个具体的子类，而 MapSite 定义了迷宫中构件之间的主要关系。
 * Room 有指向其他 MapSite 对象的引用，并保存一个房间号，这个数字用来标识迷宫中的房间。
 */
enum Direction
{
    North,
    East,
    South,
    West
};

class Room : public MapSite
{
  public:
    Room(int roomNo) 
    {
        _roomNumber = roomNo;
    }

    MapSite *GetSide(Direction dir) const 
    {
        return _sides[dir];
    }

    void SetSide(Direction dir, MapSite *side)
    {
        _sides[dir] = side;
    }

    virtual void Enter();

  private:
    MapSite *_sides[4];
    int _roomNumber;
};

// 下面的类描述了一个房间的每一面所出现的墙壁或门
class Wall : public MapSite
{
  public:
    Wall();

    virtual void Enter();
};

class Door : public MapSite
{
  public:
    Door(Room * = 0, Room * = 0);

    virtual void Enter();
    Room *OtherSideFrom(Room *);

  private:
    Room *_room1;
    Room *_room2;
    bool _isOpen;
};

class Maze
{
  public:
    Maze();
    void AddRoom(Room *);
    Room *RoomNo(int) const;

  private:
    // ......
};

class MazeGame
{
  public:
    Maze *CreateMaze()
    {
        Maze *aMaze = new Maze;
        Room *r1 = new Room(1);
        Room *r2 = new Room(2);
        Door *theDoor = new Door(r1, r2);

        aMaze->AddRoom(r1);
        aMaze->AddRoom(r2);

        r1->SetSide(North, new Wall);
        r1->SetSide(East, theDoor);
        r1->SetSide(South, new Wall);
        r1->SetSide(West, new Wall);

        r2->SetSide(North, new Wall);
        r2->SetSide(East, new Wall);
        r2->SetSide(South, new Wall);
        r2->SetSide(West, theDoor);

        return aMaze;
    }
};

int main(int argc, char const *argv[])
{
    std::cout << "Hello World!" << std::endl;

    MazeGame *mg = new MazeGame;
    Maze *maze = mg->CreateMaze();

    return 0;
}
