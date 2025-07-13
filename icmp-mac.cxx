#include <bits/stdc++.h>

#include "message.hxx"
#include "send.hxx"

int main() {
  message m{};

  int s{send(m)};

  std::cerr << s << '\n';
}
