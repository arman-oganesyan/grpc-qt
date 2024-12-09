#include <iostream>
#include <google/protobuf/message.h>

int main()
{
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  std::cout << "Protobuf version: "
            << PROTOBUF_VERSION
            << std::endl;

  return 0;
}