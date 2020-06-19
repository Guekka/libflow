
#include <flow.hpp>

#include <iostream>

#ifdef FLOW_HAVE_COROUTINES

flow::async<int> fibonacci(int a = 1, int b = 1)
{
    while (true) {
        co_yield a;
        std::tie(a, b) = std::tuple(b, a + b);
    }
}

int main()
{
    std::cout << "The first ten Fibonacci numbers are:\n";
    fibonacci().take(10).write_to(std::cout);
}

#else // !FLOW_HAVE_COROUTINES

int main()
{
    std::cout << "This example requires coroutines support\n";
}

#endif