#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H

//class CursorPriorityQueue
//{
//public:
//    class CPQComparator : IComparer<tuple<int, double>>
//    {
//        // Call CaseInsensitiveComparer.Compare with the parameters reversed.
//        int IComparer<tuple<int, double>>.Compare(Tuple<int, double> x, Tuple<int, double> y)
//        {
//            int r1 = x.Item2.CompareTo(y.Item2);
//            if (r1 != 0)
//            {
//                return r1;
//            }
//            else
//            {
//                return y.Item1.CompareTo(x.Item1);
//            }
//        }
//    }

//    SortedSet<Tuple<int, double>> nextCursorsSet = new SortedSet<Tuple<int, double>>(new CPQComparator());
//    int maxSize;
//    CursorPriorityQueue(int maxSize)
//    {
//        this.maxSize = maxSize;
//    }
//    bool Add(int i, double p)
//    {
//        if (nextCursorsSet.Count < maxSize)
//        {
//            bool result = nextCursorsSet.Add(new Tuple<int, double>(i, p));
//            return result;
//        }
//        var m = nextCursorsSet.Min;
//        if (p > m.Item2)
//        {
//            nextCursorsSet.Remove(m);
//            return nextCursorsSet.Add(new Tuple<int, double>(i, p));
//        }
//        return false;
//    }
//    public IEnumerable<int> Indices()
//    {
//        return nextCursorsSet.Select(i => i.Item1);
//    }
//    public IEnumerable<double> Values()
//    {
//        return nextCursorsSet.Select(i => i.Item2);
//    }
//}
