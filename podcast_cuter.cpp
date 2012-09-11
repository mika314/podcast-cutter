#include "real_fft.h"
#include <iostream>
#include <deque>
#include <vector>
using namespace std;

enum { BuffSize = 2 * 44100 };


int main()
{
    deque<short> d;
    int state = 0;
    long long fadeIn = 0;
    int s = 0;
    int start = -1;
    int end;
    short oldV = 0;
    short v = 0;
    float averageVelosity = 0;
    float oldAverageVelosity = 0;
    while (!cin.eof())
    {
        short v1, v2;
        cin.read((char *)(&v1), sizeof(v1));
        cin.read((char *)(&v2), sizeof(v2));
        d.push_back((v1 + v2) / 2);

        averageVelosity = (averageVelosity * 999.0f + abs(d.back())) / 1000.0f;
        if (abs(averageVelosity - oldAverageVelosity) > 5)
        {
            state = -1;
            if (fadeIn == -2)
            {
                fadeIn = 44100LL / 32;
                oldV = v;
            }
            if (start != -1)
            {
                clog << start << " " << end << endl;
                start = -1;
            }
            v = d.front();
            if (fadeIn > 0)
            {
                v = (v * (44100LL / 32 - fadeIn) + oldV * fadeIn) / (44100LL / 32);
                fadeIn--;
            }
            cout.write((const char *)(&v), sizeof(v));
            d.pop_front();
        }
        else
        {
            if (state == -1)
                state = BuffSize + BuffSize;
            if (state > 0)
            {
                if (start != -1)
                {
                    clog << start << ":" << end << endl;
                    start = -1;
                }
                v = d.front();
                if (fadeIn > 0)
                {
                    v = (v * (44100LL / 32 - fadeIn) + oldV * fadeIn) / (44100LL / 32);
                    fadeIn--;
                }
                d.pop_front();
                cout.write((const char *)(&v), sizeof(v));
                --state;
            }
            else
            {
                if (d.size() > BuffSize)
                {
                    d.pop_front();
                    if (start == -1)
                        start = s - BuffSize;
                    else
                    {
                        end = s - BuffSize;
                        fadeIn = -2;
                    }
                }
            }
        }
        oldAverageVelosity = averageVelosity;
        ++s;
        // if (s % 44100 == 0)
        //   clog << 1.0 * s / 44100 / 60 << endl;
    }
}
