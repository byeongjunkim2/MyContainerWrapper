#include <iostream>

#include "ContainerWrapper.h"


int main() {
    
    using namespace mc;

    {
        std::vector<int> arr{ 1,2,3,4,5 };
        MyContainerWrapper myContainer(arr);

        std::cout << myContainer.RangeSum(1, 3) << '\n'; // 9
        std::cout << myContainer.RangeSum(0, 4) << '\n'; // 15
    }


    {
        std::cout << std::endl;

        std::vector<int> arr{ 2,4,6,8,10 };
        MyContainerWrapper myContainer(arr);

        std::cout << myContainer[4] << '\n';
        std::cout << myContainer.RangeMult(1, 3) << '\n'; 
        std::cout << myContainer.RangeMult(0, 4) << '\n';
        std::cout << myContainer.RangeMult(3, 4) << "\n\n";
        myContainer[4] = 12;
        myContainer.Refresh();
        std::cout << myContainer[4] << '\n';
        std::cout << myContainer.RangeMult(1, 3) << '\n';
        std::cout << myContainer.RangeMult(0, 4) << '\n';
        std::cout << myContainer.RangeMult(3, 4) << '\n';
    }

    {
        std::cout << std::endl;

        int arr[5]{ 1,2,0,4,5 };
        MyContainerWrapper w1(arr, 5);

        std::cout << w1.RangeSum(1, 3) << '\n';
        std::cout << w1.RangeMult(1, 4) << '\n';
    }
    
    return 0;
}