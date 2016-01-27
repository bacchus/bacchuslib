#include "glutwin.h"

namespace bacchus {

std::map<int, glutWin*> glutWin::mWinHandles;
bool glutWin::mIsLeftMouseDown = false;

} // namespace bacchus
