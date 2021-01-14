#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

vector<string> jp_text;
vector<string> cn_text;

const int R_MAX_LEN = 64;
const int MAX_CURSOR = 8;
const double SoftmaxCoeff = 10;
const double pTransitionSkip = 0.075;
const double pTransitionNext = 1.0 - pTransitionSkip;
const double possibleCursorsThresh = 0.001;
QMap<int, double> possibleCursors;
//int **r = new int[2, R_MAX_LEN];
vector<vector<int>>r(2,vector<int>(R_MAX_LEN));

/// <summary>
/// 返回两个string的edit distance, string的长度需小于R_MAX_LEN
/// </summary>
/// <param name="first"></param>
/// <param name="second"></param>
/// <returns></returns>
///
static int Min(int e1, int e2, int e3){
    return min(min(e1, e2), e3);
}

int ComputeDistance(string first, string second)
{
    if (first.length() == 0)
    {
        return second.length();
    }

    if (second.length() == 0)
    {
        return first.length();
    }



    int current = 1;
    int previous = 0;

    for (int i = 0; i <= second.length(); i++)
    {
        r[previous][i] = i;
    }

    for (int i = 0; i < first.length(); i++)
    {
        r[current][0] = i + 1;

        for (int j = 1; j <= second.length(); j++)
        {
            int cost = (second[j - 1] == first[i]) ? 0 : 1;
            r[current][ j] = Min(
                r[previous][ j] + 1,
                r[current][ j - 1] + 1,
                r[previous][ j - 1] + cost);
        }
        previous = (previous + 1) % 2;
        current = (current + 1) % 2;
    }
    return r[previous][ second.length()];
}

static double Sigmoid(double x)
{
    //Boost x > 0.7, suppress x < 0.7, please plot the curve to visualize
    double k = exp(-15.0 * (x - 0.7));
    return 1.0 / (1.0 + k);
}

static double LengthAdjust(double x)
{
    const double offset = 0.00055277; //k(0)
    double k = exp(-0.5 * (x - 15));
    return (1.0 / (1.0 + k) - offset) / (1.0 - offset);
}


/// <summary>
/// 返回两个string的相似度, 返回值在0到1之间
/// </summary>
/// <param name="first"></param>
/// <param name="second"></param>
/// <returns></returns>
double ComputeSimilarity(string first, string second)
{
    if (first.length() >= R_MAX_LEN)
    {
        first = first.substr(0, R_MAX_LEN - 1);
    }

    if (second.length() >= R_MAX_LEN)
    {
        second = second.substr(0, R_MAX_LEN - 1);
    }
    int d = ComputeDistance(first, second);
    double m = first.length() + second.length() + 1e-9;

    return Sigmoid(1.0 - (double)d / m) * LengthAdjust(m);
}

int addNoiseState = 0;
//for test
string addNoise(string input)
{
    if (addNoiseState++ % 2 == 0)
        return input;
    return "";
}

//for test
string addNoise2(string input)
{
    QString result=QString::fromStdString(input);
    for (int i = 0; i < input.length(); i++)
    {
        if (rand() / double(RAND_MAX) < 0.5)
            result[i] = 'A';
    }
    return result.toStdString();
}

double maxval(QMap<int,double> m1){
    QList<double> listVal=m1.values();
    double max = *std::max_element(listVal.begin(), listVal.end());
    return max;
}
#include "cursorpriorityqueue.h"
string Translate(string sourceText, string desLang, string srcLang)
        {
            //sourceText = addNoise(addNoise2(sourceText));
    //The translator is able to find the correct match on hook mode under a high noise
            //Console.WriteLine(String.Format("Input:{0}", sourceText));
            if (jp_text.size() == 0)
            {
                return "No translation available";
            }

            if (sourceText.length() >= R_MAX_LEN)
            {
                sourceText = sourceText.substr(0, R_MAX_LEN - 1);
            }

            double pMostLikelyPrevCursor = possibleCursors.size() == 0 ? 1.0 / pTransitionNext : maxval(possibleCursors);

            CursorPriorityQueue nextCursorsPQ(MAX_CURSOR);

            for (int i = 0; i < jp_text.size(); i++)
            {
                double s = ComputeSimilarity(sourceText, jp_text[i]);
                if (possibleCursors.contains(i - 1)) // [Case 1]
                {
                    double pSequantial = possibleCursors[i - 1] * pTransitionNext * s;
                    nextCursorsPQ.Add(i, pSequantial);
                }
                double pSkip = pMostLikelyPrevCursor * pTransitionSkip * s; //[Case 2]
                nextCursorsPQ.Add(i, pSkip);
            }
            //Softmax on next cursors
            vector<int> nextCursorsIdx = nextCursorsPQ.Indices().ToList();
            var z = nextCursorsPQ.Values();
            double z_sum = z.Sum();
            var z_norm = z.Select(i => i / z_sum);  //take an extra normalization
            var z_exp = z_norm.Select(i => Math.Exp(SoftmaxCoeff * i));
            double sum_z_exp = z_exp.Sum();
            List<double> z_softmax = z_exp.Select(i => i / sum_z_exp).ToList();

            possibleCursors.Clear();

            for (int i = 0; i < z_softmax.Count; i++)
            {
                int j = nextCursorsIdx[i];
                if (possibleCursors.ContainsKey(j))
                {
                    possibleCursors[j] = Math.Max(possibleCursors[j], z_softmax[i]);
                }
                else
                {
                    possibleCursors[j] = z_softmax[i];
                }
            }
            int maxI = 0;
            double maxP = 0.0;
            foreach (int k in possibleCursors.Keys)
            {
                if (maxP < possibleCursors[k])
                {
                    maxP = possibleCursors[k];
                    maxI = k;
                }
                //For debug
                Console.WriteLine(String.Format("{0}:{1}", k, jp_text[k]));
                Console.WriteLine(possibleCursors[k]);
            }
            if (possibleCursors.Count == 0)
                return "无匹配文本";
//            Console.WriteLine(String.Format("[{0}:{1}]", maxI, jp_text[maxI]));
//            Console.WriteLine("------");
            return cn_text[maxI] == "" ? jp_text[maxI] : cn_text[maxI];
        }

