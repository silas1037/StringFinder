#ifndef CURSORPRIORITYQUEUE_H
#define CURSORPRIORITYQUEUE_H

#include <QMainWindow>
#include <tuple>

#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <math.h>
#include <iostream>
#include <mutex>
#include "tchar.h"
#include <Windows.h>
#include <string>
#include <map>
#include <vector>
#include <atlstr.h>
#include <QMap>
#include <QList>
#include <set>
#include <QSet>
using namespace std;

class CursorPriorityQueue
{
public:
    CursorPriorityQueue(int maxSize);

    QSet<std::tuple<int, double>> nextCursorsSet = QSet<tuple<int, double>>();
    int maxSize;


    bool Add(int i, double p)
    {
        if (nextCursorsSet.size() < maxSize)
        {
            //bool result =
            nextCursorsSet.insert(tuple<int, double>(i, p));
            //return result;
        }
        var m = nextCursorsSet.Min;
        if (p > m.Item2)
        {
            nextCursorsSet.Remove(m);
            return nextCursorsSet.Add(new Tuple<int, double>(i, p));
        }
        return false;
    }


};

#endif // CURSORPRIORITYQUEUE_H
