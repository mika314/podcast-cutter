#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <set>
#include <algorithm>
using namespace std;

int main()
{
  ifstream f("rmlist");
  if (f.bad())
  {
    cerr << "file rmlist does not exist" << endl;
    return -1;
  }

  set<pair<int, int> > frames;

  while (!f.eof())
  {
    int b, e;
    f >> b >> e;
    frames.insert(make_pair(b, e));
  }

  char line[300];
  cin.getline(line, sizeof(line));
  cout << line << "\n";
  int w = 0;
  int h = 0;
  int isw = false;
  int ish = false;
  for (char *i = line; *i != 0; ++i)
  {
    switch (*i)
    {
    case 'W':
      isw = true;
      break;
    case 'H':
      ish = true;
      break;
    default:
      if (isdigit(*i))
      { 
        if (isw)
          w = w * 10 + *i - '0';
        if (ish)
          h = h * 10 + *i - '0';
      }
      else
      {
        isw = false;
        ish = false;
      }
    }
  }
  clog << w << "x" << h << endl;

  char *yuv = new char[w * h * 3 / 2];

  int skip = 0;
  int currentFrame = 0;

  while (!cin.eof())
  {
    char line[300];
    do 
    {
        cin.getline(line, sizeof(line));
        if (strlen(line) == 0)
            return false;
    } while (strcmp(line, "FRAME") != 0);

    cin.read(yuv, w * h * 3 / 2);
    ++currentFrame;
    if (currentFrame * 44100 / 24 > frames.begin()->first)
    {
      skip += frames.begin()->second - frames.begin()->first;
      frames.erase(frames.begin());
    }
    if (skip > 44100 / 24)
      skip -= 44100 / 24;
    else
    {
      cout << "FRAME\n";
      cout.write(yuv, w * h * 3 / 2);
    }
  }

  delete [] yuv;
}
