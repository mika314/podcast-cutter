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

    set<pair<int64_t, int64_t> > frames;

    while (!f.eof())
    {
        int64_t b, e;
        f >> b >> e;
        if (!f.eof())
            frames.insert(make_pair(b - (e - b) / 2, e - (e - b) / 2));
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

    auto yuv = new unsigned char[w * h * 3 / 2];
    auto yuvAve = new unsigned char[w * h * 3 / 2];;

    int skip = 0;
    int currentFrame = 0;

    int skipCount = 0;

    while (!cin.eof())
    {
        char line[300];
        do 
        {
            cin.getline(line, sizeof(line));
            if (strlen(line) == 0)
                return false;
        } while (strcmp(line, "FRAME") != 0);

        cin.read((char *)yuv, w * h * 3 / 2);
        ++currentFrame;
        if (frames.size() > 0 && currentFrame * 44100LL / 24LL > frames.begin()->first)
        {
            skip += frames.begin()->second - frames.begin()->first;
            clog << frames.begin()->second << " " <<  frames.begin()->first << endl;
            frames.erase(frames.begin());
        }
        if (skip > 44100 / 24 && ((skip < 10 * 44100 && skipCount < 4) || (skip >= 10 * 44100 && skipCount < 20)))
        {
            skip -= 44100 / 24;
            if (skipCount == 0)
                std::copy(yuv, yuv + w * h * 3 / 2, yuvAve);
            else
            {
                auto cur = yuv;
                auto ave = yuvAve;
                for (; ave != yuvAve + w * h * 3 / 2; ++ave, ++cur)
                    *ave = (*ave + *cur) / 2;
            }
            ++skipCount;
        }
        else
        {
            cout << "FRAME\n";
            if (skipCount > 0)
            {
                auto cur = yuv;
                auto ave = yuvAve;
                for (; ave != yuvAve + w * h * 3 / 2; ++ave, ++cur)
                    *cur = (*ave + *cur) / 2;
            }
            cout.write((char *)yuv, w * h * 3 / 2);
            skipCount = 0;
        }
    }

    delete [] yuv;
}
