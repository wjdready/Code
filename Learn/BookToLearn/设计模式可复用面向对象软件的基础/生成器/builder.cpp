/**
 *
 * 将一个复杂对象的构建与它的表示分离，使得同样的构建过程可以创建不同的表示。
 * 
 * 2.适用性: 适用于使用builder模式的情况,
    a. 当创建复杂对象的方法应该独立于该对象的组成部分以及他们的装配方式时。
    b. 当构造过程必须允许被构造的对象有不同的表示时
 * 
 */

// MazeBuilder类定义下面的接口
class MazeBuilder
{
  public:
    virtual void BuildMaze()
    {
    }
    virtual void BuildRoom()
    {
    }
    virtual Maze *GetMaze();

  protected:
    MazeBuilder();
};

// 使用builder
Maze *MazeGame::CreateMaze(MazeBuilder *builder)
{
    builder.BuildMaze();
    builder.BuildRoom();
    return builder.GetMaze();
}
