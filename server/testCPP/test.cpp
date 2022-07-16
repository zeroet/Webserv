#include <vector>
#include <iostream>

class Block
{
    public:
    int fd;
    int getter(void)
    {
        return (2);
    };
};

int &refer(int *fd)
{
    std::cout << *fd << std::endl;
    *fd = 10;
    return *fd;
}


int main()
{
    std::vector<Block> block;
    Block b;

    block.push_back(b);

    std::cout << block[0].getter() << std::endl;

    int fd = 20;
    fd = refer(&fd);
    std::cout << fd << std::endl;

    return (0);
}