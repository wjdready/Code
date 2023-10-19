#ifndef __BASE_H__
#define __BASE_H__

enum Direction
{
    North,
    East,
    South,
    West
};

class MapSite
{
  public:
    virtual void Enter() = 0;
};

class Room : public MapSite
{
  public:
    Room()
    {

    }

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

    virtual void Enter()
    {

    }

  private:
    MapSite *_sides[4];
    int _roomNumber;
};

// 下面的类描述了一个房间的每一面所出现的墙壁或门
class Wall : public MapSite
{
  public:
    Wall()
    {

    }

    virtual void Enter()
    {

    }
};

class Door : public MapSite
{
  public:
    Door(Room * = 0, Room * = 0)
    {
        
    }

    virtual void Enter()
    {

    }

    Room *OtherSideFrom(Room *)
    {
        return nullptr;
    }

  private:
    Room *_room1;
    Room *_room2;
    bool _isOpen;
};

class Maze
{
  public:
    Maze()
    {

    }
    void AddRoom(Room *)
    {

    }
    Room *RoomNo(int n) const
    {
        return _rooms[n];
    }

  private:
    Room *_rooms[100];
};

#endif
