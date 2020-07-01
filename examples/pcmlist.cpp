#include <tinyalsa.hpp>

#include <iostream>

int main()
{
  tinyalsa::pcm_list list;

  for (const auto& pcm_info : list) {
    std::cout << pcm_info << std::endl;
  }

  return 0;
}
