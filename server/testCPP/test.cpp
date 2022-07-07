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


int main()
{
    std::vector<Block> block;
    Block b;

    block.push_back(b);

    std::cout << block[0].getter() << std::endl;

    return (0);
}