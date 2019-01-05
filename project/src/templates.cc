/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <iostream>

/*******************************************************************************
 * Class Definitions (Note that these are almost ALWAYS in a header file; can
 * only be in .cc file for trivial cases like this. If you try to separate the
 * interface and implementation of a template class, you will most likely run
 * into linker errors)
 ******************************************************************************/
template <class T, int N>
class MySequence {
 public:
  MySequence(void) : memblock_() {}

  void set_member(int x, T value) {
    memblock_[x] = value;
  }

  T get_member(int x) {
    return memblock_[x];
  }

 private:
  T memblock_[N];
};

/*******************************************************************************
 * Functions
 ******************************************************************************/
// There are no arguments at this time
int main() {
  MySequence<int, 5> myints;
  MySequence<double, 5> myfloats;
  myints.set_member(0, 100);
  myfloats.set_member(3, 3.1416);
  std::cout << myints.get_member(0) << '\n';
  std::cout << myfloats.get_member(3) << '\n';
  std::cout << "Amin Sakher" << std::endl;
  return 0;
}
