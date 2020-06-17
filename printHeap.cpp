#include<iostream>
#include<vector>
#include<sstream>
#include<string>
#include<cmath>
#include<iomanip>

// http://stackoverflow.com/questions/994593/how-to-do-an-integer-log2-in-c
// will be used to compute height of the heap
size_t IntegerLogarithm2(size_t arg) {
    size_t logarithm = 0;
    while (arg >>= 1) ++logarithm;
    return logarithm;
}

// will be used to compute number of elements at the level i
size_t IntegerPower2(size_t arg) {
    if (arg)
        return (size_t)2 << (arg - 1);
    else
        return 1;
}
void swap1(int index1, int index2, std::vector<int>& elem) {
    /* Komplexität 0(1) */
    int tmp = elem[index1];
    elem[index1] = elem[index2];
    elem[index2] = tmp;
}
void bubbleUp(int index, std::vector<int>& elem,bool y) {
  
    if (index <= 0) { return; }
    int  e = elem[index];
    int parent = (index - 1) / 2;
    //Element wird in die richtige Position 


    while (e < elem[parent] && index >= 0) {
        swap1(index, parent, elem);
        index = parent;
        parent = (index - 1) / 2;
    }
}
void bubbleUp2(int index, std::vector<int>& elem, bool y) {

    if (index <= 0) { return; }
    int  e = elem[index];
    int parent = (index - 1) / 2;
    //Element wird in die richtige Position 


    while (e > elem[parent] && index >= 0) {
        swap1(index, parent, elem);
        index = parent;
        parent = (index - 1) / 2;
    }
}
void add2(int x, std::vector<int>& elem, bool y) {
    /*
        Komplexität: best	 -> 0(1)
                     average -> O(logn(n))
                     worst   -> 0(logn(n))
    */
    //man setzt das neue Element an die letzte Position
    elem.push_back(x);
    bubbleUp2(elem.size() - 1, elem, y);
}
void add(int x, std::vector<int>& elem,bool y) {
    /*
        Komplexität: best	 -> 0(1)
                     average -> O(logn(n))
                     worst   -> 0(logn(n))
    */
    //man setzt das neue Element an die letzte Position
    elem.push_back(x);
    bubbleUp(elem.size()-1, elem,y);
}
// returns total line length for the level
size_t LineLength(size_t level, size_t item_width, size_t spaces_between) {
    return IntegerPower2(level) * (item_width + spaces_between) - spaces_between;
}
std::vector<int> heap_array_min(std::vector<int> a,bool y) {
    std::vector<int> out{};
    for (int i = 0; i < a.size(); i++) {
        add(a[i], out,y);
        std::cout << a[i] << ' ';
    }
    std::cout <<'\n'<< '\n';
    return out;
}
std::vector<int> heap_array_max(std::vector<int> a, bool y) {
    std::vector<int> out{};
    for (int i = 0; i < a.size(); i++) {
        add2(a[i], out, y);
        std::cout << a[i] << ' ';
    }
    std::cout << '\n' << '\n';
    return out;
}
void bubbleDown(int index, int n,int len, std::vector<int> &elem) {
    /*
        Komplexität: best	 -> 0(1)
                     average -> O(n*logn(n))
                     worst   -> 0(n*logn(n))
    */
    //kind Array speichert die Indizes aller Kind Elemente eines Knotens
    int* kind = new int[n + 1];

    while (true)
    {
        for (int i = 1; i <= n; i++)
            if ((n * index + i) < len)
                kind[i] = n * index + i;
            else
                kind[i] = -1;
        // maxiKing  speichert das maximale Kind 
        // maxiKingIndex speichert den index 
        int maxiKing = -1, maxiKingIndex;
        //man findet das Maximum aller Kinder eines bestimmten Knotens
        for (int i = 1; i <= n; i++)
        {
            if (kind[i] != -1 && elem[kind[i]] > maxiKing)
            {
                maxiKingIndex = kind[i];
                maxiKing = elem[kind[i]];
            }
        }
        //Blattknoten
        if (maxiKing == -1)
            break;

        if (elem[index] > elem[maxiKingIndex])
            swap1(index, maxiKingIndex,elem);

        index = maxiKingIndex;
    }
}
int remove(std::vector<int> &elem) {
    /*
        Komplexität: best	 -> 0(1)
                     average -> O(n*logn(n))
                     worst   -> O(n*logn(n))
    */
    if (elem.size() == 0)
    {
        return 0;
    }

    //Speichert den Stammknoten um ihm zu retournieren
    int max = elem[0];
    elem[0] = elem[elem.size() - 1];
    //len--;
    bubbleDown(0, 2,elem.size()-1,elem);
    return max;
}
int printing(std::vector<int> A) {
    // The heap array split by levels
    std::vector<std::vector<int> > levels;

    // Height of the heap
    size_t levels_number = IntegerLogarithm2(A.size() + 1);
    levels.resize(levels_number);

    // Now fill the levels
    for (size_t i = 0; i < levels.size(); ++i) {
        size_t elements_number = IntegerPower2(i);
        levels[i].resize(elements_number);
        for (size_t j = elements_number - 1, p = 0; p < elements_number; ++j, ++p)
            levels[i][p] = A[j];
    }

    if (levels_number < 1) return 0;

    int magnitude = (abs(A[0]) <= 1 ? 1 : abs(A[0]));
    size_t tab_width = (size_t)floor(log(double(magnitude)) / log(10.0)) + 1;
    // size_t longest_line = LineLength(levels_number - 1, tab_width, tab_width);
    std::vector<std::string> text;
    text.reserve(levels_number * 2 - 1);

    // Do the aligned output to the strings array
    for (size_t i = 0; i < levels_number; ++i) {
        size_t outer_space_width = IntegerPower2(levels_number - 1 - i) - 1;
        size_t inner_space_width = outer_space_width * 2 + 1;
        std::string outer_space(outer_space_width * tab_width, ' ');
        std::string inner_space(inner_space_width * tab_width, ' ');

        std::ostringstream line;
        line << outer_space;

        if (i > 0) {
            std::ostringstream branchline;
            std::string joint(tab_width, '|');
            std::string branch(inner_space_width * tab_width, '-');

            branchline << outer_space;

            if (levels[i].size() > 0) {
                branchline << joint;
            }

            bool isline = true;
            for (size_t j = 1; j < levels[i].size(); ++j, isline = !isline) {
                if (isline)
                    branchline << branch << joint;
                else
                    branchline << inner_space << std::setfill(' ') <<
                    std::setw(tab_width) << joint;
            }

            branchline << outer_space;
            text.push_back(branchline.str());
        }

        if (levels[i].size() > 0) {
            line << std::setfill(' ') << std::setw(tab_width) << levels[i][0];
        }

        for (size_t j = 1; j < levels[i].size(); ++j) {
            line << inner_space << std::setfill(' ') <<
                std::setw(tab_width) << levels[i][j];
        }

        line << outer_space;
        text.push_back(line.str());
    }

    // Output the text
    for (auto& i : text)
        std::cout << i << std::endl;

    return 0;
}

int main()
{
    // The input heap array
    std::vector<int> C{};
    std::vector<int> A1 = { 41,54,13,73,68,98,63,110,76,19,18,59,110,110,110,110,110 };
    std::vector<int> A2 = { 41,54,13,73,68,98,63,110,76,19,18,59,0,0,0,0,0 };
    //for (int i = 1; i <= 27; i++) {
    //    C.push_back(i);
    //}
    //for (int i = 1; i <= 10; i++) {
    //    C.push_back(0);
    //}
    std::vector<int> B = heap_array_max(A2,false);
    std::vector<int> A = heap_array_min(A1, false);
    //remove(A);
    printing(A1);
    std::cout << '\n' <<"MIN"<<'\n' << '\n';
    printing(A);
    std::cout << '\n' << "MAX" << '\n' << '\n';
    printing(B);
    return 0;
}
